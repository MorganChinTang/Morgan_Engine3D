// simple shader that takes a position and can add color


// defines the expected vertex data per vertex
struct VS_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
};

// output that gets interpolated in the rasterizer
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

// gets applied to every vertex
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    return output;
}

// lerped vertex data that is converted to a pixel on the screen
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.color;
}