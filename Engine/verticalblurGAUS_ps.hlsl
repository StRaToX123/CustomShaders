////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblur_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
Texture2D textureMatID : register(t1);
Texture2D horizontalSecondaryOutput : register(t2);
SamplerState SampleType_MatID : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
	float texelSizeY;
	float3 padding;
};

cbuffer blurWeightsGaus : register(b1)
{
	float gausFirstPart;
	float gausStDeviationSquared;
	float Blur_Size;
	float Blur_Sample_Count_Halfed;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_Position;
    float2 tex : TEXCOORD0;
};

struct Output
{
	float4 color_main : SV_Target0;
	float4 color_secondary : SV_Target1;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
Output VerticalBlurPixelShader(PixelInputType input)
{
	Output output;
	float col_main = 0;
	float col_secondary = 0;
	float sum_main = 0;
	float sum_secondary = 0;
	float uv = 0;
	float offset = 0;
	float gausSecondPart = 0;

	float sameMatID;
	float sameColor;
	float sampledColor_currentPixel;
	float sampledMatID_currentPixel;
	float sampledValue;
	float sampledValueTwo;

	sampledMatID_currentPixel = textureMatID.SampleLevel(SampleType_MatID, input.tex.xy, 0).x;
	sameColor = 1;
	sameMatID = 1;
	sampledColor_currentPixel = shaderTexture.Sample(SampleType_MatID, input.tex.xy).x;




	//iterate over blur samples
	for(float index = -1.0f; index > (-1.0f * Blur_Sample_Count_Halfed); index--)
	{
		//get the offset of the sample
		offset = index * texelSizeY * Blur_Size;
		//get uv coordinate of sample
		uv = input.tex.y + offset;
		// sample the offseted pixel's MatID
		sampledValue = textureMatID.SampleLevel(SampleType_MatID, float2(input.tex.x, uv), 0).x; 
		// LATENCY HIDING
		//calculate the result of the gaussian function
		gausSecondPart = pow(2.71828182846f, -((offset*offset)/(2*gausStDeviationSquared))); // 2 VGPRS extra
		// LATENCY HIDING
		// if we go offscreen then we need to set sameMatID to 0.0f so that we don't contribue to anything anymore
		// we're gonna use offset as a temp
		// if  (uv >= 0.0f)
		offset = step(0.0f, uv);
		sameMatID = lerp(0.0f, sameMatID, offset);

		sampledValueTwo = horizontalSecondaryOutput.SampleLevel(SampleType_MatID, float2(input.tex.x, uv), 0).x;

		// going back to regularly scheduled programming
		sameMatID = sameMatID * step(sampledValue, sampledMatID_currentPixel);
		sameMatID = sameMatID * step(sampledMatID_currentPixel, sampledValue);


		gausSecondPart = gausSecondPart * gausFirstPart;
		sum_secondary += gausSecondPart;
		// we're gonna do another sample here so we can hide the latency with all of the previous samples step checks
		uv = shaderTexture.Sample(SampleType_MatID, float2(input.tex.x, uv)).x;
		
		// check if it's the same color as the original pixel
		offset = step(uv, sampledColor_currentPixel);
		offset = offset * step(sampledColor_currentPixel, uv);
		
		sameColor = lerp(sameColor, sameColor * offset, step(1.0f, sameMatID));

		sum_main += gausSecondPart * sameMatID;
		//multiply color with influence from gaussian function and add it to sum color
		//we're using offset as temp here
		// first create do the secondary color
		offset = uv * gausSecondPart * sameMatID;
		col_main += offset;
		offset = sampledValueTwo * gausSecondPart;
		col_secondary += offset;
	}

	sameMatID = 1.0f;

	for(float index = 0.0f; index < Blur_Sample_Count_Halfed; index++)
	{
		//get the offset of the sample
		offset = index * texelSizeY * Blur_Size;
		//get uv coordinate of sample
		uv = input.tex.y + offset;
		// sample the offseted pixel's MatID
		sampledValue = textureMatID.SampleLevel(SampleType_MatID, float2(input.tex.x, uv), 0).x; 
		// LATENCY HIDING
		//calculate the result of the gaussian function
		gausSecondPart = pow(2.71828182846f, -((offset*offset)/(2*gausStDeviationSquared))); // 2 VGPRS extra
		// LATENCY HIDING
		// if we go offscreen then we need to set sameMatID to 0.0f so that we don't contribue to anything anymore
		// we're gonna use offset as a temp
		// if  (uv <= 0.0f)
		offset = step(uv, 1.0f);
		sameMatID = lerp(0.0f, sameMatID, offset);

		sampledValueTwo = horizontalSecondaryOutput.SampleLevel(SampleType_MatID, float2(input.tex.x, uv), 0).x;

		// going back to regularly scheduled programming
		sameMatID = sameMatID * step(sampledValue, sampledMatID_currentPixel);
		sameMatID = sameMatID * step(sampledMatID_currentPixel, sampledValue);


		gausSecondPart = gausSecondPart * gausFirstPart;
		sum_secondary += gausSecondPart;
		// we're gonna do another sample here so we can hide the latency with all of the previous samples step checks
		uv = shaderTexture.Sample(SampleType_MatID, float2(input.tex.x, uv)).x;
		
		// check if it's the same color as the original pixel
		offset = step(uv, sampledColor_currentPixel);
		offset = offset * step(sampledColor_currentPixel, uv);
		
		sameColor = lerp(sameColor, sameColor * offset, step(1.0f, sameMatID));

		sum_main += gausSecondPart * sameMatID;
		//multiply color with influence from gaussian function and add it to sum color
		//we're using offset as temp here
		// first create do the secondary color
		offset = uv * gausSecondPart * sameMatID;
		col_main += offset;
		offset = sampledValueTwo * gausSecondPart;
		col_secondary += offset;
	}

	//divide the sum of values by the amount of samples
	col_main = col_main / sum_main;
	// Set the alpha channel to one.
    output.color_main.x = lerp(col_main , sampledColor_currentPixel, sameColor);
	output.color_main.y = output.color_main.x;
	output.color_main.z = output.color_main.x;
	output.color_main.w = 1.0f;


	col_secondary = col_secondary / sum_secondary;
	output.color_secondary.x = col_secondary;
	output.color_secondary.y = col_secondary;
	output.color_secondary.z = col_secondary;
	output.color_secondary.w = 1.0f;

	return output;
}
