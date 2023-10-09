////////////////////////////////////////////////////////////////////////////////
// Filename: depth.ps
////////////////////////////////////////////////////////////////////////////////


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_Position;
	float depth : SV_Depth;
    float4 color : COLOR1;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float DepthPixelShader(PixelInputType input) : SV_Depth
{
    // THIS PASS IS USED TO WRITE DEPTH DATA TO THE DEPTH AND STENCIL BUFFER WICH WILL BE USE IN THE NEXT PASS TO PERFORM SHADOW MAPPING
    // THATS WHY IT DOESNT MATTER WHAT WE DO HERE IN THE PIXEL SHADER
    // WE WILL LET DX WRITE DEPTH DATA ON IT'S OWN
    //return input.color;
	return input.depth;
}
