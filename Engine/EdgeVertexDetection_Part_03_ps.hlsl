

//////////////
// TYPEDEFS //
//////////////
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

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 EdgeVertexDetection_Part_03_Pixel_Main(GeomOutputType input) : SV_TARGET
{
    float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);

	float4 temp_01 = float4(floor(input.position.x), floor(input.position.y), round(input.position.x), round(input.position.y));
	float4 temp_02 = float4(floor(input.Vertex_01_Pos.x * 800.0f), 
								floor(input.Vertex_01_Pos.y * 600.0f), 
									round(input.Vertex_01_Pos.x * 800.0f), 
										round(input.Vertex_01_Pos.y * 600.0f));


	if(temp_02.z == temp_02.x)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z + 1, temp_02.w);
	}

	if(temp_02.w == temp_02.y)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z, temp_02.w + 1);
	}


    if (input.Vertex_01_isEdgeVertex > 0)
    {
		if (((temp_01.x == temp_02.x) && (temp_01.y == temp_02.y)) // floor floor floor floor
					|| ((temp_01.x == temp_02.x) && (temp_01.y == temp_02.w)) // floor floor floor ceiling
						|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.y)) // floor floor ceiling floor
							|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.w)) // floor floor ceiling ceiling
								|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.y)) // floor ceiling floor floor
									|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.w)) // floor ceiling floor ceiling
										|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.y)) // floor ceiling ceiling floor
											|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.w)) // floor ceiling ceiling ceiling
												|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.y)) // ceiling floor floor floor
													|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.w)) // ceiling floor floor ceiling
														|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.y)) // ceiling floor ceiling floor
															|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.w)) // ceiling floor ceiling ceiling
																|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.y)) // ceiling ceiling floor floor
																	|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.w)) // ceiling ceiling floor ceiling
																		|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.y)) // ceiling ceiling ceiling floor
																			|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.w))) // ceiling ceiling ceiling ceiling
		{
			color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		}
    }
        
    temp_02 = float4(floor(input.Vertex_02_Pos.x * 800.0f),
						floor(input.Vertex_02_Pos.y * 600.0f),
							round(input.Vertex_02_Pos.x * 800.0f),
								round(input.Vertex_02_Pos.y * 600.0f));

	if(temp_02.z == temp_02.x)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z + 1, temp_02.w);
	}

	if(temp_02.w == temp_02.y)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z, temp_02.w + 1);
	}


	if (input.Vertex_02_isEdgeVertex > 0)
	{
		if (((temp_01.x == temp_02.x) && (temp_01.y == temp_02.y)) // floor floor floor floor
						|| ((temp_01.x == temp_02.x) && (temp_01.y == temp_02.w)) // floor floor floor ceiling
							|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.y)) // floor floor ceiling floor
								|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.w)) // floor floor ceiling ceiling
									|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.y)) // floor ceiling floor floor
										|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.w)) // floor ceiling floor ceiling
											|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.y)) // floor ceiling ceiling floor
												|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.w)) // floor ceiling ceiling ceiling
													|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.y)) // ceiling floor floor floor
														|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.w)) // ceiling floor floor ceiling
															|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.y)) // ceiling floor ceiling floor
																|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.w)) // ceiling floor ceiling ceiling
																	|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.y)) // ceiling ceiling floor floor
																		|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.w)) // ceiling ceiling floor ceiling
																			|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.y)) // ceiling ceiling ceiling floor
																				|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.w))) // ceiling ceiling ceiling ceiling
		{
			color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

	temp_02 = float4(floor(input.Vertex_03_Pos.x * 800.0f),
							floor(input.Vertex_03_Pos.y * 600.0f),
								round(input.Vertex_03_Pos.x * 800.0f),
									round(input.Vertex_03_Pos.y * 600.0f));

	if(temp_02.z == temp_02.x)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z + 1, temp_02.w);
	}

	if(temp_02.w == temp_02.y)
	{
		temp_02 = float4(temp_02.x, temp_02.y, temp_02.z, temp_02.w + 1);
	}


	if (input.Vertex_03_isEdgeVertex > 0)
	{
		if (((temp_01.x == temp_02.x) && (temp_01.y == temp_02.y)) // floor floor floor floor
								|| ((temp_01.x == temp_02.x) && (temp_01.y == temp_02.w)) // floor floor floor ceiling
									|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.y)) // floor floor ceiling floor
										|| ((temp_01.x == temp_02.x) && (temp_01.w == temp_02.w)) // floor floor ceiling ceiling
											|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.y)) // floor ceiling floor floor
												|| ((temp_01.x == temp_02.z) && (temp_01.y == temp_02.w)) // floor ceiling floor ceiling
													|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.y)) // floor ceiling ceiling floor
														|| ((temp_01.x == temp_02.z) && (temp_01.w == temp_02.w)) // floor ceiling ceiling ceiling
															|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.y)) // ceiling floor floor floor
																|| ((temp_01.z == temp_02.x) && (temp_01.y == temp_02.w)) // ceiling floor floor ceiling
																	|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.y)) // ceiling floor ceiling floor
																		|| ((temp_01.z == temp_02.x) && (temp_01.w == temp_02.w)) // ceiling floor ceiling ceiling
																			|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.y)) // ceiling ceiling floor floor
																				|| ((temp_01.z == temp_02.z) && (temp_01.y == temp_02.w)) // ceiling ceiling floor ceiling
																					|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.y)) // ceiling ceiling ceiling floor
																						|| ((temp_01.z == temp_02.z) && (temp_01.w == temp_02.w))) // ceiling ceiling ceiling ceiling
		{
			color = float4(1.0f, 0.0f, 0.0f, 1.0f);
		}
	}

    return color;
}
