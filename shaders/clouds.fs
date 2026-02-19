#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

int maxSteps = 80;
int lightSteps = 10;
float absorption = 0.9;
float cloudScale = 1000.0;

float topHeight = 150.0;
float bottomHeight = 100.0;

#define MAX_DIST 100

vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0));

uniform sampler2D screenTexture;
uniform sampler2D depthTexture;
uniform sampler2D noiseTexture;

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

float sampleDensity(vec3 pos)
{
    float rData = texture(noiseTexture, pos.xz / vec2(cloudScale)).r;
    float gData = texture(noiseTexture, pos.xz / vec2(cloudScale)).g;
    float bData = texture(noiseTexture, pos.xz / vec2(cloudScale / 16)).b;
    float aData = texture(noiseTexture, pos.xz / vec2(cloudScale / 32)).b;

    vec4 data = vec4(rData, gData, bData, aData);
    float density = clamp((data.r - data.g * 0.7), 0.0, 1.0);

    float heightPercent = (pos.y - bottomHeight) / (topHeight - bottomHeight);
    float heightMask = smoothstep(0.0, 0.45, heightPercent) * smoothstep(1.0, 0.2, heightPercent);
    heightMask;

    return density * heightMask;
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
        density += sampleDensity(pos) * stepSize;
        pos += lightDirection * vec3(stepSize);
    }

    return beer(density) + ((1 - beer(density)) * 0.4);
}

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
        float jitter = fract(sin(dot(TexCoords * gl_FragCoord.xy, vec2(12.9898,78.233))) * 43758.5453);

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

            float density = sampleDensity(pos);
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
}

