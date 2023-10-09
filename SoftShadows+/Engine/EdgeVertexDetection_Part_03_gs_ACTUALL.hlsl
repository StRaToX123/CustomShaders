

struct GeomInputType
{
    float4 position : SV_Position;
    nointerpolation uint isEdgeVertex : TEXCOORD2;
};

struct GeomOutputType
{
    float4 position : SV_Position;
    nointerpolation float3 Vertex_01_Pos : COLOR1;
    nointerpolation float3 Vertex_02_Pos : COLOR2;
    nointerpolation float3 Vertex_03_Pos : COLOR3;
    nointerpolation uint4 connections : TEXCOORD3;
};


[maxvertexcount(3)]
void GSMain(triangle GeomInputType input[3], inout TriangleStream<GeomOutputType> OutputStream)
{
    uint temp;

    GeomOutputType output = (GeomOutputType)0;

	output.Vertex_01_Pos = float3((((input[0].position.x / input[0].position.w) / 2.0f) + 0.5f) * 800.0f, (1.0f - (((-input[0].position.y / input[0].position.w) / 2.0f) + 0.5f)) * 600.0f, 1.0f);
	output.Vertex_02_Pos = float3((((input[1].position.x / input[1].position.w) / 2.0f) + 0.5f) * 800.0f, (1.0f - (((-input[1].position.y / input[1].position.w) / 2.0f) + 0.5f)) * 600.0f, 1.0f);
	output.Vertex_03_Pos = float3((((input[2].position.x / input[2].position.w) / 2.0f) + 0.5f) * 800.0f, (1.0f - (((-input[2].position.y / input[2].position.w) / 2.0f) + 0.5f)) * 600.0f, 1.0f);

    output.connections.x = 0;

    // are vertex 01 and vertex 02 edge vertices
    temp = input[0].isEdgeVertex * input[1].isEdgeVertex; // if boath are edge vertices temp.x is 1
    //////////////////////////////////
    output.connections.x = output.connections.x | temp; // if boath 01 and 02 are edge vertices then output.connections is now 0x000000000000000000000000000001
    //////////////////////////////////

    // are vertex 01 and vertex 03 edge vertices
    temp = input[0].isEdgeVertex * input[2].isEdgeVertex;
    //////////////////////////////////
    output.connections.x = output.connections.x | (2 * temp); // if boath 01 and 02 are edge vertices then output.connections is now 0x000000000000000000000000000010
    //////////////////////////////////

    // are vertex 02 and vertex 03 edge vertices
    temp = input[1].isEdgeVertex * input[2].isEdgeVertex;
    //////////////////////////////////
    output.connections.x = output.connections.x | (4 * temp); // if boath 02 and 03 are edge vertices then output.connections is now 0x000000000000000000000000000100
    //////////////////////////////////

    for (uint i = 0; i < 3; i++)
    {
        output.position = input[i].position;
        OutputStream.Append(output);
    }
}