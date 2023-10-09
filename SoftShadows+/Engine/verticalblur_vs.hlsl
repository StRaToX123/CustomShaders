////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblur_vs.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer ScreenSizeBuffer
{
	float screenHeight;
	float3 padding;
};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
	float2 texCoord10 : TEXCOORD10;
	float2 texCoord11 : TEXCOORD11;
	float2 texCoord12 : TEXCOORD12;
	float2 texCoord13 : TEXCOORD13;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType VerticalBlurVertexShader(VertexInputType input)
{
    PixelInputType output;
	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;
    
	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / screenHeight;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(0.0f, texelSize * -6.0f);
	output.texCoord2 = float2(output.texCoord1.x, output.texCoord1.y + texelSize);
	output.texCoord3 = float2(output.texCoord1.x, output.texCoord2.y + texelSize);
	output.texCoord4 = float2(output.texCoord1.x, output.texCoord3.y + texelSize);
	output.texCoord5 = float2(output.texCoord1.x, output.texCoord4.y + texelSize);
	output.texCoord6 = float2(output.texCoord1.x, output.texCoord5.y + texelSize);
	output.texCoord7 = input.tex.xy;
	output.texCoord8 = float2(output.texCoord1.x, output.texCoord7.y + texelSize);
	output.texCoord9 = float2(output.texCoord1.x, output.texCoord8.y + texelSize);
	output.texCoord10 = float2(output.texCoord1.x, output.texCoord9.y + texelSize);
	output.texCoord11 = float2(output.texCoord1.x, output.texCoord10.y + texelSize);
	output.texCoord12 = float2(output.texCoord1.x, output.texCoord11.y + texelSize);
	output.texCoord13 = float2(output.texCoord1.x, output.texCoord12.y + texelSize);
	

    return output;
}