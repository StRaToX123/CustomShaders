////////////////////////////////////////////////////////////////////////////////
// Filename: depthshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EDGEVERTEXDETECTION_PART_03_
#define _EDGEVERTEXDETECTION_PART_03_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
//#include <d3dx10math.h>
#include <DirectXMath.h>
//#include <d3dx11async.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;


__declspec(align(16))
class EdgeVertexDetection_Part_03_CLASS
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

public:
	EdgeVertexDetection_Part_03_CLASS();
	EdgeVertexDetection_Part_03_CLASS(const EdgeVertexDetection_Part_03_CLASS&);
	~EdgeVertexDetection_Part_03_CLASS();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool __vectorcall Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool __vectorcall SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
};

#endif