////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
//#include "lightshaderclass.h"
#include "lightclass.h"
#include "rendertextureclass.h"
#include "depthshaderclass.h"
#include "edgevertexdetection_part_01.h"
#include "edgevertexdetection_part_02.h"
#include "edgevertexdetection_part_03.h"
#include "BlurToSolidColor.h"
#include "TextureUpsamplingShader.h"
#include "shadowshaderclass.h"
#include "orthowindowclass.h"
#include "textureshaderclass.h"
#include "horizontalblurshaderclass.h"
#include "verticalblurshaderclass.h"
#include "softshadowshaderclass.h"
#include <list>




/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
const int SHADOWMAP_WIDTH = 4096;
const int SHADOWMAP_HEIGHT = 4096;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND*);
	void Shutdown();
	bool Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float &Blur_Size, int &Blur_Sample_Count, float &Blur_Standard_Deviation, float &upsamplingLowerLimit, float &upsamplingUpperLimit);
	D3DClass* Get_D3D();

	const int downSampleSizeX = 800;
	const int downSampleSizeY = 600;
	const float texelSizeX = 1.0f / downSampleSizeX;
	const float texelSizeY = 1.0f / downSampleSizeY;
	const float aspectRatio = (float)downSampleSizeX / (float)downSampleSizeY;
	const float aspectRatioInv = (float)downSampleSizeY / (float)downSampleSizeX;
	int windowSizeX = 0;
	int windowSizeY = 0;
	

private:
	bool EdgeVertexDetection_Part_01_Function();
	bool EdgeVertexDetection_Part_02_Function();
	bool EdgeVertexDetection_Part_03_Function();
	
	bool Render(float &Blur_Size, int &Blur_Sample_Count, float &Blur_Standard_Deviation, float &upsamplingLowerLimit, float &upsamplingUpperLimit);

	float GetDownSampleSizeX();
	float GetDownSmapleSizeY();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	ModelClass *m_CubeModel, *m_GroundModel, *m_SphereModel;
	LightClass* m_Light;
	RenderTextureClass *m_RenderTexture, *m_RenderTexture2, *m_RenderTexture3, *m_BlackWhiteRenderTexture, *m_DownSampleTexure;
	RenderTextureClass *m_HorizontalBlurTexture, *m_VerticalBlurTexture, *m_UpSampleTexure;
	DepthShaderClass* m_DepthShader;
	EdgeVertexDetection_Part_01_CLASS* m_EdgeVertexDetection_Part_01;
	EdgeVertexDetection_Part_02_CLASS* m_EdgeVertexDetection_Part_02;
	EdgeVertexDetection_Part_03_CLASS* m_EdgeVertexDetection_Part_03;
	BlurToSolidColor* m_blurToSolidColor;
	TextureUpSamplingShader* m_textureUpSamplingCorrection;
	ShadowShaderClass* m_ShadowShader;
	OrthoWindowClass *m_SmallWindow, *m_FullScreenWindow;
	TextureShaderClass* m_TextureShader;
	HorizontalBlurShaderClass* m_HorizontalBlurShader;
	VerticalBlurShaderClass* m_VerticalBlurShader;
	SoftShadowShaderClass* m_SoftShadowShader;
	ID3D11Texture2D* m_correctionTexture;
	ID3D11ShaderResourceView* m_correctionTextureSRV;
	ID3D11UnorderedAccessView* m_correctionTextureUAV;

	XMMATRIX r_worldMatrix, r_viewMatrix, r_baseViewMatrix, r_lightViewMatrix, r_projectionMatrix, r_lightProjectionMatrix, r_translateMatrix, r_orthoMatrix;
	float r_posX, r_posY, r_posZ;
	bool r_result;


	unsigned int resolutionWidth;
	unsigned int resolutionHeight;

	ID3D11ShaderResourceView* const nullShaderResourceView[3] = { NULL, NULL, NULL };
	ID3D11SamplerState* const nullSampleState[1] = { NULL };
	ID3D11RenderTargetView* pRTVs[2];
};


#endif