////////////////////////////////////////////////////////////////////////////////
// Filename: horizontalblurshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _HORIZONTALBLURSHADERCLASS_H_
#define _HORIZONTALBLURSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <fstream>
#include "rendertextureclass.h"
using namespace std;
using namespace DirectX;

////////////////////////////////////////////////////////////////////////////////
// Class name: HorizontalBlurShaderClass
////////////////////////////////////////////////////////////////////////////////
class HorizontalBlurShaderClass
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
		float texelSizeX;
		float aspectRatioInv;
		XMFLOAT2 padding;
	};

	struct BlurWightBufferType
	{
		float gausFirstPart;
		float gausStDeviationSquared;
		float Blur_Size;
		float Blur_Sample_Count;
	};

public:
	HorizontalBlurShaderClass();
	HorizontalBlurShaderClass(const HorizontalBlurShaderClass&);
	~HorizontalBlurShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool __vectorcall Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float texelSizeX, float aspectRatioInv,
		ID3D11ShaderResourceView* mat_id_texture, float &Blur_Size, int &Blur_Sample_Count, float &gausStDeviationSquared, float &gausFirstPart);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

	bool __vectorcall SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix, ID3D11ShaderResourceView* texture, float texelSizeX, float aspectRatioInv,
		ID3D11ShaderResourceView* mat_id_texture, float &Blur_Size, int &Blur_Sample_Count, float &gausStDeviationSquared, float &gausFirstPart);


	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11SamplerState* m_sampleStatePoint;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_screenSizeBuffer;
	ID3D11Buffer* m_blurWeightsBuffer;
};

#endif