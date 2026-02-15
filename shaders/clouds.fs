#version 410 core

in vec2 TexCoords;
out vec4 FragColor;

int maxSteps = 20;
int lightSteps = 8;
float absorption = 0.9;
float cloudScale = 1.0;

float topHeight = 10.0;
float bottomHeight = 8.0;

#define MAX_DIST 100

vec3 lightDirection = normalize(vec3(0.0, 1.0, 0.0));

uniform sampler2D noiseTexture;
uniform sampler2D screenTexture;

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

void main() {
    vec3 col = texture(screenTexture, TexCoords).rgb;
    FragColor = vec4(col, 1.0);
}

//void main()
//{
//    vec3 ro = GetCameraPositionWS();
//    vec3 rd = mul(unity_CameraInvProjection, vec4(IN.texcoord * 2 - 1, 0, -1)).rgb;
//    vec3 viewVector = mul(unity_CameraToWorld, vec4(rd, 0)).rgb;
//    rd = normalize(viewVector);
//
//    vec4 sceneColor = texture(_BlitTexture, sampler_LinearClamp, IN.texcoord);
//
//    float tEnter, tExit;
//
//    if (intersection(ro, rd, tEnter, tExit))
//    {
//        float jitter = frac(sin(dot(IN.texcoord * _ScreenParams.xy, vec2(12.9898,78.233))) * 43758.5453);
//
//        float t0 = max(tEnter, 0.0);
//        vec3 startPos = ro + rd * vec3(t0);
//        vec3 endPos = ro + rd * vec3(tExit);
//
//        float stepSize = distance(endPos, startPos) / _MaxSteps;
//
//        float transmittance = 1.0;
//        vec3 scatteredLight = vec3(0.0);
//
//        float rawDepth = SAMPLE_TEXTURE2D(_CameraDepthTexture, sampler_CameraDepthTexture, IN.texcoord).r;
//        float depth = LinearEyeDepth(rawDepth, _ZBufferParams) * length(viewVector);
//
//        for (int i = 0; i < _MaxSteps; i++)
//        {
//            vec3 pos = startPos + vec3(i + jitter) * rd * stepSize;
//            if (distance(ro, pos) > depth) break;
//
//            float density = sampleDensity(pos);
//            float extinction = density * stepSize * _Absorption;
//
//            if (extinction < 1e-4) continue;
//
//            float light = lightMarch(pos);
//
//            scatteredLight += vec3(transmittance * density * light * stepSize);
//
//            transmittance *= exp(-extinction);
//
//            if (transmittance < 0.01) break;
//        }
//
//        sceneColor.rgb = sceneColor.rgb * transmittance + scatteredLight;
//    }
//
//    FragColor = saturate(sceneColor);
//}

