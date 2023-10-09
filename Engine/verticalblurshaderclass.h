////////////////////////////////////////////////////////////////////////////////
// Filename: verticalblurshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _VERTICALBLURSHADERCLASS_H_
#define _VERTICALBLURSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
//#include <d3dx10math.h>
#include <DirectXMath.h>
//#include <d3dx11async.h>
#include "rendertextureclass.h"
#include <d3dcompiler.h>
#include <fstream>
using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: VerticalBlurShaderClass
////////////////////////////////////////////////////////////////////////////////
class VerticalBlurShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct ScreenSizeBufferType
	{
		float texelSizeY;
		XMFLOAT3 padding;
	};

	struct BlurWightsBufferType
	{
		float gausFirstPart;
		float gausStDeviationSquared;
		float Blur_Size;
		float Blur_Sample_Count;
	};

public:
	VerticalBlurShaderClass();
	VerticalBlurShaderClass(const VerticalBlurShaderClass&);
	~VerticalBlurShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool __vectorcall Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float texelSizeY,
		ID3D11ShaderResourceView* mat_id_texture, ID3D11ShaderResourceView* horizontal_secondary_render_target, float &Blur_Size, int &Blur_Sample_Count, float &gausStDeviationSquared, float &gausFirstPart);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool __vectorcall SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float texelSizeY,
		ID3D11ShaderResourceView* mat_id_texture, ID3D11ShaderResourceView* horizontal_secondary_render_target, float &Blur_Size, int &Blur_Sample_Count, float &gausStDeviationSquared, float &gausFirstPart);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStatePoint;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_screenSizeBuffer;
	ID3D11Buffer* m_blurWightsBuffer;
};

#endif