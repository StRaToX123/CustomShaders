

struct GeomInputType
{
    float4 position : SV_Position;
    nointerpolation uint isEdgeVertex : TEXCOORD2;
};

struct GeomOutputType
{
    float4 position : SV_Position;
    nointerpolation float3 Vertex_01_Pos : COLOR1;
    nointerpolation uint Vertex_01_isEdgeVertex : TEXCOORD3;
    nointerpolation float3 Vertex_02_Pos : COLOR2;
    nointerpolation uint Vertex_02_isEdgeVertex : TEXCOORD4;
    nointerpolation float3 Vertex_03_Pos : COLOR3;
    nointerpolation uint Vertex_03_isEdgeVertex : TEXCOORD5;
};


[maxvertexcount(3)]
void GSMain(triangle GeomInputType input[3], inout TriangleStream<GeomOutputType> OutputStream)
{

    GeomOutputType output = (GeomOutputType)0;

	output.Vertex_01_Pos = float3(((input[0].position.x / input[0].position.w) / 2.0f) + 0.5f, ((-input[0].position.y / input[0].position.w) / 2.0f) + 0.5f, 1.0f);
	output.Vertex_02_Pos = float3(((input[1].position.x / input[1].position.w) / 2.0f) + 0.5f, ((-input[1].position.y / input[1].position.w) / 2.0f) + 0.5f, 1.0f);
	output.Vertex_03_Pos = float3(((input[2].position.x / input[2].position.w) / 2.0f) + 0.5f, ((-input[2].position.y / input[2].position.w) / 2.0f) + 0.5f, 1.0f);

    output.Vertex_01_isEdgeVertex = input[0].isEdgeVertex;
    output.Vertex_02_isEdgeVertex = input[1].isEdgeVertex;
    output.Vertex_03_isEdgeVertex = input[2].isEdgeVertex;

    for (uint i = 0; i < 3; i++)
    {
        output.position = input[i].position;
		
        OutputStream.Append(output);
    }
}