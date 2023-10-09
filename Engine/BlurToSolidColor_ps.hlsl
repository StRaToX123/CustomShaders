////////////////////////////////////////////////////////////////////////////////
// Filename: BlurToSolidColor_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
SamplerState clampSampler : register(s0);


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
float4 BlurToSolidColorPixelShader(PixelInputType input) : SV_TARGET
{
	float4 color = float4(0,0,0,1);
	float4 textureColor;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.SampleLevel(clampSampler, input.tex, 0);

    uint isBackground_01 = step(0, textureColor.y);
    uint isBackground_02 = step(textureColor.y, 0);

    isBackground_01 = isBackground_01 * isBackground_02;

    isBackground_01 = 1 - isBackground_01;

    color = float4(0, isBackground_01, 0, isBackground_01);

    return color;
}