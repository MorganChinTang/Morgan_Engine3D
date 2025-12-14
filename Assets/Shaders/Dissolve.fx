// Character Dissolve Effect file that blends 2 textures based on pixel height in the world

cbuffer TransformBuffer : register(b0)
{
    matrix world;
    matrix wvp;
    matrix lwvp;
    float3 viewPosition;
    
}

cbuffer LightBuffer : register(b1)
{
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
    float3 lightDirection;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialEmissive;
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialShininess;
}

cbuffer SettingsBuffer : register(b3)
{
    bool useShadowMap;
    float depthBias;
    float lowHeight;
    float blendHeight;
}

Texture2D lowTextureMap : register(t0);
Texture2D highTextureMap : register(t1);
Texture2D shadowMap : register(t2);

SamplerState textureSampler : register(s0);

// Get random value based on 2D position
float hash(float2 p)
{
    return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

// Crete perlin noise
float noise(float2 p)
{
    float2 i = floor(p);
    float2 f = frac(p);
    f = f * f * (3.0 - 2.0 * f);
    
    float a = hash(i);
    float b = hash(i + float2(1.0, 0.0));
    float c = hash(i + float2(0.0, 1.0));
    float d = hash(i + float2(1.0, 1.0));
    
    float ab = lerp(a, b, f.x);
    float cd = lerp(c, d, f.x);
    return lerp(ab, cd, f.y);
}

// Create fractal noise
float fractalNoise(float2 p)
{
    float value = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;
    float maxValue = 0.0f;
    
    for(int i = 0; i < 4; i++)
    {
        value += amplitude * noise(p * frequency);
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }
    
    return value / maxValue;
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float3 worldNormal : NORMAL;
    float3 dirToLight : TEXCOORD0;
    float3 dirToView : TEXCOORD1;
    float2 texCoord : TEXCOORD2;
    float4 lightNDCPosition : TEXCOORD3;
    float4 worldPosition : TEXCOORD4;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = mul(float4(input.position, 1.0f), wvp);
    output.worldNormal = mul(input.normal, (float3x3) world);
    output.worldPosition = mul(float4(input.position, 1.0f), world);
    output.dirToLight = -lightDirection;
    output.dirToView = normalize(viewPosition - output.worldPosition.xyz);
    output.texCoord = input.texCoord;
    if (useShadowMap)
    {
        output.lightNDCPosition = mul(float4(input.position, 1.0f), lwvp);
    }
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 n = normalize(input.worldNormal);
    float3 light = normalize(input.dirToLight);
    float3 view = normalize(input.dirToView);
    
    //emisive
    float4 emissive = MaterialEmissive;
    
    //ambient
    float4 ambient = lightAmbient * MaterialAmbient;
    
    //diffuse
    float d = saturate(dot(light, n));
    float4 diffuse = d * lightDiffuse * MaterialDiffuse;
    
    //specular
    float r = reflect(-light, n);
    float base = saturate(dot(r, view));
    float s = pow(base, MaterialShininess);
    float4 specular = s * lightSpecular * MaterialSpecular;
    
    // Blend two textures based on world height
    float4 lowMapColor = lowTextureMap.Sample(textureSampler, input.texCoord);
    float4 highMapColor = highTextureMap.Sample(textureSampler, input.texCoord);
    float4 diffuseColor = lowMapColor;

    
    if (input.worldPosition.y > lowHeight + blendHeight)
    {
        //diffuseColor = highMapColor;
        diffuseColor.a = 0.1f;
    }
    else
    {
        float t = saturate((input.worldPosition.y - lowHeight) / blendHeight);

        // Three layers of noise to create a non uniform jagged edge
        float edgeNoise = fractalNoise(input.worldPosition.xz * 2.0f);
        float edgeNoise2 = fractalNoise(input.worldPosition.xz * 5.0f + 100.0f);
        float edgeNoise3 = fractalNoise(input.worldPosition.xz * 12.0f + 200.0f);
        
        // Blend the three noise patterns
        float combinedNoise = lerp(edgeNoise, edgeNoise2, 0.4f);
        combinedNoise = lerp(combinedNoise, edgeNoise3, 0.35f);
        
        // Amplify the noise variation to create dramatic ups and downs
        combinedNoise = (combinedNoise - 0.5f) * 1.4f + 0.5f;
        combinedNoise = saturate(combinedNoise);
        float noiseThreshold = combinedNoise * 0.4f + 0.3f;
        
        // Thickness of the edge effect
        float edgeGradient = smoothstep(noiseThreshold - 0.08f, noiseThreshold + 0.08f, t);
        float distToThreshold = t - noiseThreshold;
        if(distToThreshold > 0.08f)
        {
            // In between top and the noisy edge
            diffuseColor = lowMapColor;
            diffuseColor.a = 0.1f;
        }
        else if(distToThreshold > -0.08f)
        {
            // The edge effect
            float glowIntensity = 1.0f - edgeGradient;
            // Cyan glow color
            diffuseColor.rgb = float3(0.0f, 1.0f, 1.0f);
            diffuseColor.a = glowIntensity;
        }
        else
        {
            // Below the edge
            diffuseColor = lowMapColor;
        }
    }
    
    
    float4 finalColor = (emissive + ambient + diffuse) * diffuseColor + specular;
    if (useShadowMap)
    {
        float actualDepth = 1.0f - (input.lightNDCPosition.z / input.lightNDCPosition.w);
        float2 shadowUV = input.lightNDCPosition.xy / input.lightNDCPosition.w;
        float u = (shadowUV.x + 1.0f) * 0.5f;
        float v = 1.0f - ((shadowUV.y + 1.0f) * 0.5f);
        if (saturate(u) == u && saturate(v) == v)
        {
            float4 savedColor = shadowMap.Sample(textureSampler, float2(u, v));
            float savedDepth = savedColor.r;
            if (savedDepth > actualDepth + depthBias)
            {
                finalColor = (emissive + ambient) * diffuseColor;
            }
        }
    }
    
    return finalColor;
}