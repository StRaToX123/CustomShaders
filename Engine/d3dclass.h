////////////////////////////////////////////////////////////////////////////////
// Filename: d3dclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "d3dx11.lib")
//#pragma comment(lib, "d3dx10.lib")


//////////////
// INCLUDES //
//////////////
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
//#include <d3dx10math.h>
#include <DirectXMath.h>
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: D3DClass
////////////////////////////////////////////////////////////////////////////////
__declspec(align(16)) class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND*, bool, float, float);
	void Shutdown();
	
	void BeginScene(float, float, float, float, bool);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	ID3D11ShaderResourceView* GetDepthBufferSRV(bool same_frame);
	ID3D11ShaderResourceView* GetBackBufferSRV(bool same_frame);
	HWND* GetHWND();

	void GetProjectionMatrix(XMMATRIX&);
	void GetWorldMatrix(XMMATRIX&);
	void GetOrthoMatrix(XMMATRIX&);

	void GetVideoCardInfo(char*, int&);

	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11DepthStencilView* GetDepthStencilView();
	D3D11_VIEWPORT* GetViewPort();
	
	void SetBackBufferRenderTarget();
	void ResetViewport();

	void TurnZBufferOn();
	void TurnZBufferOff();

	void TurnBackFaceCullingOn();
	void TurnFrontFaceCullingOn();

	void TurnOnAlphaBlending();
	void TurnOffAlphaBlending();

	

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11Texture2D* m_backBufferTx2D;
	ID3D11ShaderResourceView* m_backBufferSRV;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11Texture2D* m_depthStencilBufferSRV;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11ShaderResourceView* m_depthStencilSRV;
	ID3D11RasterizerState* m_rasterState_BackFaceCulling;
	ID3D11RasterizerState* m_rasterState_FrontFaceCulling;
	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;
	D3D11_VIEWPORT m_viewport;
	ID3D11DepthStencilState* m_depthDisabledStencilState;
	int resolutionWidth;
	int resolutionHeight;
	HWND* m_hwnd;
	ID3D11BlendState* m_alphaEnableBlendingState;
	ID3D11BlendState* m_alphaDisableBlendingState;
};

#endif