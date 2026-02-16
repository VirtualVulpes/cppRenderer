#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

int maxSteps = 40;
int lightSteps = 8;
float absorption = 0.9;
float cloudScale = 250.0;

float topHeight = 40.0;
float bottomHeight = 38.0;

#define MAX_DIST 100

vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0));

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform sampler2D noiseTexture;

uniform vec2 screenSize;
uniform vec2 zPlanes;
uniform mat4 projMatrix;

uniform vec3 cameraPos;
uniform mat4 viewMatrix;

bool intersection(vec3 ro, vec3 rd, out float tEnter, out float tExit)
{

    if (abs(rd.y) < 1e-5)
    {
        if (ro.y < bottomHeight || ro.y > topHeight) return false;

        tEnter = 0;
        tExit = MAX_DIST;
        return true;
    }

    vec3 invRd = vec3(1.0) / rd;

    float tBottom = (bottomHeight - ro.y) * invRd.y;
    float tTop = (topHeight - ro.y) * invRd.y;

    tEnter = min(tBottom, tTop);;
    tExit = max(tBottom, tTop);;

    return tExit >= max(tEnter, 0.0);
}

float sampleDensity(vec2 pos)
{
    vec4 data = texture(noiseTexture, pos / vec2(cloudScale));
    float density = clamp((data.r - data.g * .02 - data.b * .01 - data.a * .005), 0.0, 1.0);
    return density;
}

float beer(float density) { return exp(-density * absorption); }

float lightMarch(vec3 pos) {

    float tEnter, tExit;
    if(!intersection(pos, lightDirection, tEnter, tExit))
    {
        return 1.0;
    }

    float marchLength = tExit - max(tEnter, 0.0);
    float stepSize = marchLength / lightSteps;

    pos += lightDirection * vec3((max(tEnter, 0.0) + stepSize * 0.5));

    float density = 0.0;

    for (int i = 0; i < lightSteps; i++)
    {
        density += sampleDensity(vec2(pos.x, pos.z)) * stepSize;
        pos += lightDirection * vec3(stepSize);
    }

    return beer(density);
}

//void main() {
//    vec3 col = texture(screenTexture, TexCoords).rgb;
//    FragColor = vec4(col, 1.0);
//}

void main()
{
    vec2 uv = TexCoords * 2.0 - 1.0;
    vec3 ro = cameraPos;

    vec4 clip = vec4(uv, -1.0, 1.0);
    vec4 view = inverse(projMatrix) * clip;

    vec3 rd = normalize((inverse(viewMatrix) * vec4(view.xyz, 0.0)).xyz);

    vec4 sceneColor = texture(screenTexture, TexCoords);

    float tEnter, tExit;

    float rawDepth = texture(depthTexture, TexCoords).r;
    float ndc = rawDepth * 2.0 - 1.0;
    float linearDepth = (2.0 * zPlanes.x * zPlanes.y) / (zPlanes.y + zPlanes.x - ndc * (zPlanes.y - zPlanes.x));

    if (intersection(ro, rd, tEnter, tExit))
    {
        float jitter = fract(sin(dot(TexCoords * screenSize, vec2(12.9898,78.233))) * 43758.5453);

        float t0 = max(tEnter, 0.0);
        vec3 startPos = ro + rd * vec3(t0);
        vec3 endPos = ro + rd * vec3(tExit);

        float stepSize = distance(endPos, startPos) / maxSteps;

        float transmittance = 1.0;
        vec3 scatteredLight = vec3(0.0);

        for (int i = 0; i < maxSteps; i++)
        {
            vec3 pos = startPos + vec3(i + jitter) * rd * stepSize;
            if (abs(distance(ro, pos)) > linearDepth) break;

            float density = sampleDensity(pos.xz);
            float extinction = density * stepSize * absorption;

            if (extinction < 1e-4) continue;

            float light = lightMarch(pos);

            scatteredLight += vec3(transmittance * density * light * stepSize);

            transmittance *= exp(-extinction);

            if (transmittance < 0.01) break;
        }

        sceneColor.rgb = sceneColor.rgb * transmittance + scatteredLight;
    }

    FragColor = sceneColor;
    //FragColor = texture(screenTexture, TexCoords);
}

