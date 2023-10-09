////////////////////////////////////////////////////////////////////////////////
// Filename: horizontalblur_vs.hlsl
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
	float screenWidth;
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
PixelInputType HorizontalBlurVertexShader(VertexInputType input)
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
    
	// Determine the floating point size of a texel for a screen with this specific width.
	texelSize = 1.0f / screenWidth;

	// Create UV coordinates for the pixel and its four horizontal neighbors on either side.
	output.texCoord1 = input.tex + float2(texelSize * -6.0f, 0.0f);
	output.texCoord2 = float2(output.texCoord1.x + texelSize, output.texCoord1.y);
	output.texCoord3 = float2(output.texCoord2.x + texelSize, output.texCoord1.y);
	output.texCoord4 = float2(output.texCoord3.x + texelSize, output.texCoord1.y);
	output.texCoord5 = float2(output.texCoord4.x + texelSize, output.texCoord1.y);
	output.texCoord6 = float2(output.texCoord5.x + texelSize, output.texCoord1.y);
	output.texCoord7 = input.tex.xy;
	output.texCoord8 = float2(output.texCoord7.x + texelSize, output.texCoord1.y);
	output.texCoord9 = float2(output.texCoord8.x + texelSize, output.texCoord1.y);
	output.texCoord10 = float2(output.texCoord9.x + texelSize, output.texCoord1.y);
	output.texCoord11 = float2(output.texCoord10.x + texelSize, output.texCoord1.y);
	output.texCoord12 = float2(output.texCoord11.x + texelSize, output.texCoord1.y);
	output.texCoord13 = float2(output.texCoord12.x + texelSize, output.texCoord1.y);
	

    return output;
}