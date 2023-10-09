////////////////////////////////////////////////////////////////////////////////
// Filename: textureshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREUPSAMPLINGSHADER_H_
#define _TEXTUREUPSAMPLINGSHADER_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: TextureShaderClass
////////////////////////////////////////////////////////////////////////////////
class TextureUpSamplingShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct UpsamplingLimits
	{
		float lowerLimit;
		float upperLimit;
		XMFLOAT2 padding;
	};

public:
	TextureUpSamplingShader();
	TextureUpSamplingShader(const TextureUpSamplingShader&);
	~TextureUpSamplingShader();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool __vectorcall Render(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView* MatID_SRV, ID3D11ShaderResourceView* No_Edge_Stop_Blur, float &upsamplingLowerLimit, float &upsamplingUpperLimit);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool __vectorcall SetShaderParameters(ID3D11DeviceContext*, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView* MatID_SRV, ID3D11ShaderResourceView* No_Edge_Stop_Blur, float &upsamplingLowerLimit, float &upsamplingUpperLimit);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_limitsBuffer;
	ID3D11SamplerState* m_sampleState_Linear;
	ID3D11SamplerState* m_sampleState_Point;

};

#endif