// Simple shader that applies a post processing effect to the scene
cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float param0;
    float param1;
    float param2;
}

Texture2D textureMap0: register(t0);
Texture2D textureMap1: register(t1);
Texture2D textureMap2: register(t2);
Texture2D textureMap3: register(t3);

SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position: POSITION;
    float2 texcoord: TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position: SV_POSITION;
    float2 texcoord: TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texcoord = input.texcoord;
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = 0.0f;
    
    if(mode == 0)
    {
        finalColor = textureMap0.Sample(textureSampler, input.texcoord);
    }
    else if (mode == 1) //monochrome
    {
        float4 color = textureMap0.Sample(textureSampler, input.texcoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2) //invert
    {
        float4 color = textureMap0.Sample(textureSampler, input.texcoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3) //mirror
    {
        float2 texcoord = input.texcoord;
        texcoord.x *= param0;
        texcoord.y *= param1;
        finalColor = textureMap0.Sample(textureSampler, texcoord);
    }
    else if (mode == 4) //blur
    {
        float u = input.texcoord.x;
        float v = input.texcoord.y;
        finalColor = textureMap0.Sample(textureSampler, float2(u, v)) 
        + textureMap0.Sample(textureSampler, float2(u + param0, v)) 
        + textureMap0.Sample(textureSampler, float2(u - param1, v)) 
        + textureMap0.Sample(textureSampler, float2(u, v + param1))
        + textureMap0.Sample(textureSampler, float2(u, v - param1))
        + textureMap0.Sample(textureSampler, float2(u + param0, v + param1))
        + textureMap0.Sample(textureSampler, float2(u + param0, v - param1))
        + textureMap0.Sample(textureSampler, float2(u - param0, v + param1))
        + textureMap0.Sample(textureSampler, float2(u - param0, v - param1));
        finalColor *= 0.12f;
        
    }
    else if (mode == 5) //combine2
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texcoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texcoord);
        //finalColor = (color0 + color1) * 0.5f;
        color1.a *= param0;
        finalColor = (color0 * (1.0f - color1.a)) + (color1 * color1.a);
    }
    else if (mode == 6) //motion blur
    {
        float u = input.texcoord.x;
        float v = input.texcoord.y;
        float dist = distance(input.texcoord, float2(0.5f, 0.5f));
        float weight = saturate(lerp(0.0f, 1.0f, (dist - 0.25f) / 0.25f));
        float p0 = param0 * weight;
        float p1 = param1 * weight;
        
        finalColor = textureMap0.Sample(textureSampler, float2(u, v))
        + textureMap0.Sample(textureSampler, float2(u + p0, v))
        + textureMap0.Sample(textureSampler, float2(u - p1, v))
        + textureMap0.Sample(textureSampler, float2(u, v + p1))
        + textureMap0.Sample(textureSampler, float2(u, v - p1))
        + textureMap0.Sample(textureSampler, float2(u + p0, v + p1))
        + textureMap0.Sample(textureSampler, float2(u + p0, v - p1))
        + textureMap0.Sample(textureSampler, float2(u - p0, v + p1))
        + textureMap0.Sample(textureSampler, float2(u - p0, v - p1));
        finalColor *= 0.12f;
    }
    else if (mode == 7) //chromatic aberration
    {
        float2 distortion = float2(param0, -param1);
        float4 redChannel = textureMap0.Sample(textureSampler, input.texcoord + distortion.x* input.texcoord);
        float4 greenChannel = textureMap0.Sample(textureSampler, input.texcoord);
        float4 blueChannel = textureMap0.Sample(textureSampler, input.texcoord - distortion.y* input.texcoord);
        finalColor = float4(redChannel.r, greenChannel.g, blueChannel.b, 1.0f);
    }
    else if (mode == 8) //wave
    {
        float waveValue = input.texcoord.x * (3.141592f * param1);
        float2 texCoord = input.texcoord;
        texCoord.y += sin(waveValue) * param0;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    return finalColor;
}

//"None",
//"Monochrome",
//"Invert",
//Mirror,
//"Blur",
//"Combine2",
//"MotionBlur",
//"ChromaticAberration",
//"Wave"