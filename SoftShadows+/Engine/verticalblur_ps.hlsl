////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblur_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
Texture2D textureMatID : register(t1);
SamplerState SampleType : register(s0);
SamplerState SampleType_MatID : register(s1);

cbuffer weights
{
	float weight0;
	float weight1;
	float weight2;
	float weight3;
	float weight4;
	float weight5;
	float weight6;
	float3 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1; // - 6
	float2 texCoord2 : TEXCOORD2; // - 5
	float2 texCoord3 : TEXCOORD3; // - 4
	float2 texCoord4 : TEXCOORD4; // - 3
	float2 texCoord5 : TEXCOORD5; // - 2
	float2 texCoord6 : TEXCOORD6; // - 1
	float2 texCoord7 : TEXCOORD7; // current pixel 
	float2 texCoord8 : TEXCOORD8; // + 1
	float2 texCoord9 : TEXCOORD9; // + 2
	float2 texCoord10 : TEXCOORD10; // + 3
	float2 texCoord11 : TEXCOORD11; // + 4
	float2 texCoord12 : TEXCOORD12; // + 5
	float2 texCoord13 : TEXCOORD13; // + 6
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 VerticalBlurPixelShader(PixelInputType input) : SV_TARGET
{
	float sameMatID;
	float sameColor; // if same color is 1 then we discard the final color and return the original if its 0 then all the samples aren't the same color and we return the acumulated color variable
	float4 color;
	float4 firstSampledColor;
	float4 sampledValue;
	float4 sampledMatID_currentPixel;



	sampledMatID_currentPixel = textureMatID.SampleLevel(SampleType_MatID, input.texCoord7, 0);

	sameColor = 1;

	// Initialize the color to black.
	color = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// we're gonna use sameMatID from now on as temp
	firstSampledColor = float4(shaderTexture.Sample(SampleType, input.texCoord7).xyz, 1.0f); // firstSampledColor.w is used as a temp variable for comparing sameColor
	color += firstSampledColor * weight0; // add the weight of the same pixel

	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord6, 0); // weight of the first left pixel
	sameMatID = step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord6);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight1 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord5, 0); // add weight of the second left pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord5);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight2 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord4, 0); // add weight of the third left pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord4);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight3 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord3, 0); // add weight of the forth left pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);
    
	sampledValue = shaderTexture.Sample(SampleType, input.texCoord3);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight4 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord2, 0); // add weight of fifth left pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord2);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight5 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord1, 0); // add weight of sixth left pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord1);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight6 * sameMatID;
	color = color + sampledValue;

	///////////////////////////////////////////////////////////////////////////
	// !!! sameMatID resets for the right side but the sameColor doesn't !!! //
	///////////////////////////////////////////////////////////////////////////

	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord8, 0); // add weight of first right pixel
	sameMatID = step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord8);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight1 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord9, 0); // add weight of the second right pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord9);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight2 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord10, 0); // add weight of the third right pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord10);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight3 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord11, 0); // add weight of the forth right pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord11);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight4 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord12, 0); // add weight of the fifth right pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord12);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight5 * sameMatID;
	color = color + sampledValue;



	sampledValue = textureMatID.SampleLevel(SampleType_MatID, input.texCoord13, 0); // add weight of the sixth right pixel
	sameMatID = sameMatID * step(sampledValue.x, sampledMatID_currentPixel.x);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.x, sampledValue.x);
	sameMatID = sameMatID * step(sampledValue.y, sampledMatID_currentPixel.y);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.y, sampledValue.y);
	sameMatID = sameMatID * step(sampledValue.z, sampledMatID_currentPixel.z);
	sameMatID = sameMatID * step(sampledMatID_currentPixel.z, sampledValue.z);

	sampledValue = shaderTexture.Sample(SampleType, input.texCoord13);
	// check if it's the same color as the original pixel
	firstSampledColor.w = step(sampledValue.x, firstSampledColor.x);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.x, sampledValue.x);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.y, firstSampledColor.y);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.y, sampledValue.y);
	firstSampledColor.w = firstSampledColor.w * step(sampledValue.z, firstSampledColor.z);
	firstSampledColor.w = firstSampledColor.w * step(firstSampledColor.z, sampledValue.z);

	sameColor = lerp(sameColor, sameColor * firstSampledColor.w, step(1.0f, sameMatID));


	sampledValue = sampledValue * weight6 * sameMatID;
	color = color + sampledValue;


	// Set the alpha channel to one.
    color = lerp(color , firstSampledColor, sameColor);
	color.a = 1.0f;
	

    return color;
}
