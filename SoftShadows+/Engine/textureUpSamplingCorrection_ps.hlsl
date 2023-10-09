////////////////////////////////////////////////////////////////////////////////
// Filename: texture.ps
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
Texture2D MatIDTexture : register(t1);
Texture2D NoEdgeStopBlur : register(t2);
SamplerState SampleType_Linear : register(s0);
SamplerState SampleType_Point : register(s1);


cbuffer resolutions : register(b0)
{
    float lower_limit;
    float upper_limit;
    float2 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 TexturePixelShader(PixelInputType input) : SV_TARGET
{
	float4 textureColor_Linear;
	float4 textureColor_Point;
    float4 col;
    float temp;
    float temp_02;
    float temp_03;

    textureColor_Linear.xyz = MatIDTexture.Sample(SampleType_Linear, input.tex).xyz;
    textureColor_Point.xyz = MatIDTexture.Sample(SampleType_Point, input.tex).xyz;

    col.xyz = step(textureColor_Linear.xyz, textureColor_Point.xyz);
    col.xyz = col.xyz * step(textureColor_Point.xyz, textureColor_Linear.xyz);
    // temp will be 0 if we're not an edge pixel and 1 if we are
    temp = 1.0f - (col.x * col.y* col.z);

    textureColor_Linear = shaderTexture.Sample(SampleType_Linear, input.tex);
    textureColor_Point.xyz = shaderTexture.Sample(SampleType_Point, input.tex).xyz;
    
    col.xyz = step(textureColor_Linear.xyz, textureColor_Point.xyz);
    col.xyz = col.xyz * step(textureColor_Point.xyz, textureColor_Linear.xyz);
    // temp_02 will be 0 if the edge pixel is not next to a shadowed part and 1 if it is
    temp_02 = 1.0f - (col.x * col.y* col.z);

    // temp is now 1 for pixels that are on the edge of objects and also leading towards edge stoped shadows (so we dont consider all edge pixels) 
    temp = temp * temp_02;

    textureColor_Point.x = NoEdgeStopBlur.Sample(SampleType_Point, input.tex).x;
    // if the non edge-stoped blur is somewhere between the range of 0.45 and 0.55 then we will use the non edge stoped blur value for final textureColor_Linear
    // other-wise we will use black 
    temp_02 = step(textureColor_Point.x, upper_limit);
    temp_02 = temp_02 * step(lower_limit, textureColor_Point.x);

    col = lerp(float4(textureColor_Point.x, textureColor_Point.x, textureColor_Point.x, 1.0f), float4(0.0f, 0.0f, 0.0f, 1.0f), temp_02);


    col = lerp(textureColor_Linear, col, temp);


    return col;
}