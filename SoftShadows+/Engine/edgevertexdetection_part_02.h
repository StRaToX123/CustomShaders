////////////////////////////////////////////////////////////////////////////////
// Filename: depthshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _EDGEVERTEXDETECTION_PART_02_
#define _EDGEVERTEXDETECTION_PART_02_


#define DEBUG

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
//#include <d3dx10math.h>
#include <DirectXMath.h>
//#include <d3dx11async.h>
#include <d3dcompiler.h>
#include <fstream>
#include "ComputeShadeTypes.h"

using namespace std;
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: DepthShaderClass
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16))
class EdgeVertexDetection_Part_02_CLASS
{

public:
	EdgeVertexDetection_Part_02_CLASS();
	EdgeVertexDetection_Part_02_CLASS(const EdgeVertexDetection_Part_02_CLASS&);
	~EdgeVertexDetection_Part_02_CLASS();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool __vectorcall Render(ID3D11DeviceContext* , HWND* , ID3D11ShaderResourceView*, XMMATRIX , XMMATRIX , XMMATRIX, int, int, ID3D11UnorderedAccessView*, float color, float downSampleSizeX, float downSampleSizeY);

private:
	bool InitializeShader(ID3D11Device* , HWND , WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
	HRESULT CompileComputeShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob, HWND hwnd);


private:
	ID3D11Buffer* m_byteBuffer;
	ID3D11ShaderResourceView* m_byteBuffer_SRV;
	ID3D11ComputeShader* m_EdgeVertexDetection_Part_02_cs;
	ID3D11SamplerState* m_sampleStateClamp;
	float* float_array;
};

#endif