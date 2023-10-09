

//////////////
// TYPEDEFS //
//////////////
struct GeomOutputType
{
    float4 position : SV_Position;
    nointerpolation float3 Vertex_01_Pos : COLOR1;
    nointerpolation float3 Vertex_02_Pos : COLOR2;
    nointerpolation float3 Vertex_03_Pos : COLOR3;
    nointerpolation uint4 connections : TEXCOORD3;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader																  //
////////////////////////////////////////////////////////////////////////////////
float4 EdgeVertexDetection_Part_03_Pixel_Main(GeomOutputType input) : SV_TARGET
{
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	float PixelY = 600.0f - input.position.y;
	// Chech how close the pixel is to the the line created by vertices 1 and 2
	float m = (input.Vertex_02_Pos.y - input.Vertex_01_Pos.y) / (input.Vertex_02_Pos.x - input.Vertex_01_Pos.x);
	float b = input.Vertex_01_Pos.y - (m * input.Vertex_01_Pos.x);
	float A = (0.0 - 1.0) * m;
	float C = (0.0 - 1.0) * b;
	float final_distance = ((A * input.position.x) + PixelY + C) / sqrt((A * A) + 1);
	final_distance = abs(final_distance);

	////////////////////
	color = lerp(float4(1,1,1,1), float4(0,0,0,1), step(final_distance, 2.0f));
	color = color * (input.connections.x & 1);
	color.w = 1.0f;
	////////////////////
	
	
	// Chech how close the pixel is to the the line created by vertices 1 and 3
	m = (input.Vertex_03_Pos.y - input.Vertex_01_Pos.y) / (input.Vertex_03_Pos.x - input.Vertex_01_Pos.x);
	b = input.Vertex_01_Pos.y - (m * input.Vertex_01_Pos.x);
	A = (0.0 - 1.0) * m;
	C = (0.0 - 1.0) * b;
	final_distance = ((A * input.position.x) + PixelY + C) / sqrt((A * A) + 1);
	final_distance = abs(final_distance);
	
	////////////////////
	color = lerp(color, float4(0,0,0,1), step(final_distance, 2.0f));
	color = color * ((input.connections.x & 2) >> 1);
	color.w = 1.0f;
	////////////////////

	// Chech how close the pixel is to the the line created by vertices 2 and 3
	m = (input.Vertex_03_Pos.y - input.Vertex_02_Pos.y) / (input.Vertex_03_Pos.x - input.Vertex_02_Pos.x);
	b = input.Vertex_02_Pos.y - (m * input.Vertex_02_Pos.x);
	A = (0.0 - 1.0) * m;
	C = (0.0 - 1.0) * b;
	final_distance = ((A * input.position.x) + PixelY + C) / sqrt((A * A) + 1);
	final_distance = abs(final_distance);
	
	////////////////////
	color = lerp(color, float4(0,0,0,1), step(final_distance, 2.0f));
	color = color * ((input.connections.x & 4) >> 2);
	color.w = 1.0f;
	////////////////////



    return color;
}
