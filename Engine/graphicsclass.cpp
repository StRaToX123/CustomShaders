////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_CubeModel = 0;
	m_GroundModel = 0;
	m_SphereModel = 0;
	m_Light = 0;
	m_RenderTexture = 0;
	m_RenderTexture2 = 0;
	m_DepthShader = 0;
	m_EdgeVertexDetection_Part_01 = 0;
	m_EdgeVertexDetection_Part_02 = 0;
	m_EdgeVertexDetection_Part_03 = 0;
	m_blurToSolidColor = 0;
	m_textureUpSamplingCorrection = 0;
	m_BlackWhiteRenderTexture = 0;
	m_ShadowShader = 0;
	m_DownSampleTexure = 0;
	m_SmallWindow = 0;
	m_TextureShader = 0;
	m_HorizontalBlurTexture = 0;
	m_HorizontalBlurShader = 0;
	m_VerticalBlurTexture = 0;
	m_VerticalBlurShader = 0;
	m_UpSampleTexure = 0;
	m_FullScreenWindow = 0;
	m_SoftShadowShader = 0;
	m_correctionTexture = 0;
	m_correctionTextureSRV = 0;
	m_correctionTextureUAV = 0;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND* hwnd)
{
	bool result;
	HRESULT hr;


	windowSizeX = screenWidth;
	windowSizeY = screenHeight;
    

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(windowSizeX, windowSizeY, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -10.0f);
	m_Camera->RenderBaseViewMatrix();

	// Create the cube model object.
	m_CubeModel = new ModelClass;
	if(!m_CubeModel)
	{
		return false;
	}

	// Initialize the cube model object.
	result = m_CubeModel->Initialize(m_D3D->GetDevice(), "../Engine/data/cube.txt", L"../Engine/data/wall01.dds", hwnd, 0.8f);
	//result = m_CubeModel->InitializeMesh(m_D3D->GetDevice(), "../Engine/data/Cube.obj", hwnd, 0.8f);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the cube model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the cube model.
	m_CubeModel->SetPosition(-2.0f, 2.0f, 0.0f);

	// Create the sphere model object.
	m_SphereModel = new ModelClass;
	if(!m_SphereModel)
	{
		return false;
	}

	// Initialize the sphere model object.
	result = m_SphereModel->Initialize(m_D3D->GetDevice(), "../Engine/data/sphere.txt", L"../Engine/data/ice.dds", hwnd, 0.5f);
	//result = m_SphereModel->InitializeMesh(m_D3D->GetDevice(), "../Engine/data/Sphere.obj", hwnd, 0.5f);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the sphere model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the sphere model.
	m_SphereModel->SetPosition(2.0f, 2.0f, 0.0f);

	// Create the ground model object.
	m_GroundModel = new ModelClass;
	if(!m_GroundModel)
	{
		return false;
	}

	// Initialize the ground model object.
	result = m_GroundModel->Initialize(m_D3D->GetDevice(), "../Engine/data/plane01.txt", L"../Engine/data/metal001.dds", hwnd, 0.2f);
	//result = m_GroundModel->InitializeMesh(m_D3D->GetDevice(), "../Engine/data/Plane.obj", hwnd, 0.2f);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the ground model object.", L"Error", MB_OK);
		return false;
	}

	// Set the position for the ground model.
	m_GroundModel->SetPosition(0.0f, 1.0f, 0.0f);

	// Create the light object.
	m_Light = new LightClass;
	if(!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetLookAt(0.0f, 0.0f, 0.0f);
	m_Light->GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the render to texture object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the render to texture object.
	m_RenderTexture2 = new RenderTextureClass;
	if (!m_RenderTexture2)
	{
		return false;
	}
	
	// Initialize the render to texture object.
	result = m_RenderTexture2->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the render to texture 2 object.", L"Error", MB_OK);
		return false;
	}

	// Create the render to texture object.
	m_RenderTexture3 = new RenderTextureClass;
	if (!m_RenderTexture3)
	{
		return false;
	}

	// Initialize the render to texture object.
	result = m_RenderTexture3->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the render to texture 3 object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the depth shader object.
	m_DepthShader = new DepthShaderClass;
	if(!m_DepthShader)
	{
		return false;
	}
	
	// Initialize the depth shader object.
	result = m_DepthShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the depth shader object.", L"Error", MB_OK);
		return false;
	}

	/*
	// Create the EdgeVertexDetection_Part_01 shader object.
	m_EdgeVertexDetection_Part_01 = new EdgeVertexDetection_Part_01_CLASS;
	if (!m_EdgeVertexDetection_Part_01)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = m_EdgeVertexDetection_Part_01->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the EdgeVertexDetection_Part_01 shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the EdgeVertexDetection_Part_02 shader for the Cube Model.
	m_EdgeVertexDetection_Part_02 = new EdgeVertexDetection_Part_02_CLASS;
	if (!m_EdgeVertexDetection_Part_02)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = m_EdgeVertexDetection_Part_02->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the EdgeVertexDetection_Part_02 shader object.", L"Error", MB_OK);
		return false;
	}
	
	// Create the EdgeVertexDetection_Part_02 shader for the Cube Model.
	m_EdgeVertexDetection_Part_03 = new EdgeVertexDetection_Part_03_CLASS;
	if (!m_EdgeVertexDetection_Part_03)
	{
		return false;
	}

	// Initialize the depth shader object.
	result = m_EdgeVertexDetection_Part_03->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the EdgeVertexDetection_Part_03 shader object.", L"Error", MB_OK);
		return false;
	}
	*/
	/*
	// Create the BlurToSolidColor shader object
	m_blurToSolidColor = new BlurToSolidColor;
	if (!m_blurToSolidColor)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_blurToSolidColor->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the blur to solid color shader.", L"Error", MB_OK);
		return false;
	}
	*/
	// Create the BlurToSolidColor shader object
	m_textureUpSamplingCorrection = new TextureUpSamplingShader;
	if (!m_textureUpSamplingCorrection)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_textureUpSamplingCorrection->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if (!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the texture UpSampling Correction Shader.", L"Error", MB_OK);
		return false;
	}
	
	// Create the black and white render to texture object.
	m_BlackWhiteRenderTexture = new RenderTextureClass;
	if(!m_BlackWhiteRenderTexture)
	{
		return false;
	}

	// Initialize the black and white render to texture object.
	result = m_BlackWhiteRenderTexture->Initialize(m_D3D->GetDevice(), downSampleSizeX , downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the black and white render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the shadow shader object.
	m_ShadowShader = new ShadowShaderClass;
	if(!m_ShadowShader)
	{
		return false;
	}

	// Initialize the shadow shader object.
	result = m_ShadowShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the shadow shader object.", L"Error", MB_OK);
		return false;
	}

	/*
	// Create the down sample render to texture object.
	m_DownSampleTexure = new RenderTextureClass;
	if(!m_DownSampleTexure)
	{
		return false;
	}

	// Initialize the down sample render to texture object.
	result = m_DownSampleTexure->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, 100.0f, 1.0f);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the down sample render to texture object.", L"Error", MB_OK);
		return false;
	}
	*/
	// Create the small ortho window object.
	m_SmallWindow = new OrthoWindowClass;
	if(!m_SmallWindow)
	{
		return false;
	}

	// Initialize the small ortho window object.
	result = m_SmallWindow->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the small ortho window object.", L"Error", MB_OK);
		return false;
	}
	/*
	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
	*/
	// Create the horizontal blur render to texture object.
	m_HorizontalBlurTexture = new RenderTextureClass;
	if(!m_HorizontalBlurTexture)
	{
		return false;
	}

	// Initialize the horizontal blur render to texture object.
	result = m_HorizontalBlurTexture->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the horizontal blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the horizontal blur shader object.
	m_HorizontalBlurShader = new HorizontalBlurShaderClass;
	if(!m_HorizontalBlurShader)
	{
		return false;
	}

	// Initialize the horizontal blur shader object.
	result = m_HorizontalBlurShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the horizontal blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur render to texture object.
	m_VerticalBlurTexture = new RenderTextureClass;
	if(!m_VerticalBlurTexture)
	{
		return false;
	}

	// Initialize the vertical blur render to texture object.
	result = m_VerticalBlurTexture->Initialize(m_D3D->GetDevice(), downSampleSizeX, downSampleSizeY, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the vertical blur render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the vertical blur shader object.
	m_VerticalBlurShader = new VerticalBlurShaderClass;
	if(!m_VerticalBlurShader)
	{
		return false;
	}

	// Initialize the vertical blur shader object.
	result = m_VerticalBlurShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the vertical blur shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the up sample render to texture object.
	m_UpSampleTexure = new RenderTextureClass;
	if(!m_UpSampleTexure)
	{
		return false;
	}

	// Initialize the up sample render to texture object.
	result = m_UpSampleTexure->Initialize(m_D3D->GetDevice(), windowSizeX, windowSizeY, SCREEN_DEPTH, 0.1f);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the up sample render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the full screen ortho window object.
	m_FullScreenWindow = new OrthoWindowClass;
	if(!m_FullScreenWindow)
	{
		return false;
	}

	// Initialize the full screen ortho window object.
	result = m_FullScreenWindow->Initialize(m_D3D->GetDevice(), windowSizeX, windowSizeY);
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the full screen ortho window object.", L"Error", MB_OK);
		return false;
	}

	// Create the soft shadow shader object.
	m_SoftShadowShader = new SoftShadowShaderClass;
	if(!m_SoftShadowShader)
	{
		return false;
	}

	// Initialize the soft shadow shader object.
	result = m_SoftShadowShader->Initialize(m_D3D->GetDevice(), (*(hwnd)));
	if(!result)
	{
		MessageBox((*(hwnd)), L"Could not initialize the soft shadow shader object.", L"Error", MB_OK);
		return false;
	}

	////////////////////////////////////////
	// Creating uav for correctionTexture //
	////////////////////////////////////////

	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hresult;
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC shaderUAVdesc;


	// Initialize the render target texture description.
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	// Setup the render target texture description.
	textureDesc.Width = downSampleSizeX;
	textureDesc.Height = downSampleSizeY;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create the render target texture.
	hresult = m_D3D->GetDevice()->CreateTexture2D(&textureDesc, NULL, &m_correctionTexture);
	if (FAILED(hresult))
	{
		return false;
	}


	// Setup the description of the shader resource view.
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	// Create the shader resource view.
	hresult = m_D3D->GetDevice()->CreateShaderResourceView(m_correctionTexture, &shaderResourceViewDesc, &m_correctionTextureSRV);
	if (FAILED(hresult))
	{
		return false;
	}

	ZeroMemory(&shaderUAVdesc, sizeof(shaderUAVdesc));

	shaderUAVdesc.Format = textureDesc.Format;
	shaderUAVdesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	shaderUAVdesc.Texture2D.MipSlice = 0;

	hresult = m_D3D->GetDevice()->CreateUnorderedAccessView(m_correctionTexture, &shaderUAVdesc, &m_correctionTextureUAV);
	if (FAILED(hresult))
	{
		return false;
	}

	return true;
}


void GraphicsClass::Shutdown()
{
	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the cube model object.
	if (m_CubeModel)
	{
		m_CubeModel->Shutdown();
		delete m_CubeModel;
		m_CubeModel = 0;
	}

	// Release the sphere model object.
	if (m_SphereModel)
	{
		m_SphereModel->Shutdown();
		delete m_SphereModel;
		m_SphereModel = 0;
	}

	// Release the ground model object.
	if (m_GroundModel)
	{
		m_GroundModel->Shutdown();
		delete m_GroundModel;
		m_GroundModel = 0;
	}

	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture)
	{
		m_RenderTexture->Shutdown();
		delete m_RenderTexture;
		m_RenderTexture = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture2)
	{
		m_RenderTexture2->Shutdown();
		delete m_RenderTexture2;
		m_RenderTexture2 = 0;
	}

	// Release the render to texture object.
	if (m_RenderTexture3)
	{
		m_RenderTexture3->Shutdown();
		delete m_RenderTexture3;
		m_RenderTexture3 = 0;
	}

	// Release the depth shader object.
	if (m_DepthShader)
	{
		m_DepthShader->Shutdown();
		delete m_DepthShader;
		m_DepthShader = 0;
	}

	// Release the EdgeVertexDetection_Part_01 shader object.
	if (m_EdgeVertexDetection_Part_01)
	{
		m_EdgeVertexDetection_Part_01->Shutdown();
		delete m_EdgeVertexDetection_Part_01;
		m_EdgeVertexDetection_Part_01 = 0;
	}

	// Release the EdgeVertexDetection_Part_01 shader object.
	if (m_EdgeVertexDetection_Part_02)
	{
		m_EdgeVertexDetection_Part_02->Shutdown();
		delete m_EdgeVertexDetection_Part_02;
		m_EdgeVertexDetection_Part_02 = 0;
	}

	if (m_EdgeVertexDetection_Part_03)
	{
		m_EdgeVertexDetection_Part_03->Shutdown();
		delete m_EdgeVertexDetection_Part_03;
		m_EdgeVertexDetection_Part_03 = 0;
	}

	if (m_blurToSolidColor)
	{
		m_blurToSolidColor->Shutdown();
		delete m_blurToSolidColor;
		m_blurToSolidColor = 0;
	}

	if (m_textureUpSamplingCorrection)
	{
		m_textureUpSamplingCorrection->Shutdown();
		delete m_textureUpSamplingCorrection;
		m_textureUpSamplingCorrection = 0;
	}

	// Release the black and white render to texture.
	if (m_BlackWhiteRenderTexture)
	{
		m_BlackWhiteRenderTexture->Shutdown();
		delete m_BlackWhiteRenderTexture;
		m_BlackWhiteRenderTexture = 0;
	}

	// Release the shadow shader object.
	if (m_ShadowShader)
	{
		m_ShadowShader->Shutdown();
		delete m_ShadowShader;
		m_ShadowShader = 0;
	}

	// Release the down sample render to texture object.
	if (m_DownSampleTexure)
	{
		m_DownSampleTexure->Shutdown();
		delete m_DownSampleTexure;
		m_DownSampleTexure = 0;
	}

	// Release the small ortho window object.
	if (m_SmallWindow)
	{
		m_SmallWindow->Shutdown();
		delete m_SmallWindow;
		m_SmallWindow = 0;
	}

	/*
	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}
	*/
	// Release the horizontal blur render to texture object.
	if (m_HorizontalBlurTexture)
	{
		m_HorizontalBlurTexture->Shutdown();
		delete m_HorizontalBlurTexture;
		m_HorizontalBlurTexture = 0;
	}

	// Release the horizontal blur shader object.
	if (m_HorizontalBlurShader)
	{
		m_HorizontalBlurShader->Shutdown();
		delete m_HorizontalBlurShader;
		m_HorizontalBlurShader = 0;
	}

	// Release the vertical blur render to texture object.
	if (m_VerticalBlurTexture)
	{
		m_VerticalBlurTexture->Shutdown();
		delete m_VerticalBlurTexture;
		m_VerticalBlurTexture = 0;
	}

	// Release the vertical blur shader object.
	if (m_VerticalBlurShader)
	{
		m_VerticalBlurShader->Shutdown();
		delete m_VerticalBlurShader;
		m_VerticalBlurShader = 0;
	}

	// Release the up sample render to texture object.
	if (m_UpSampleTexure)
	{
		m_UpSampleTexure->Shutdown();
		delete m_UpSampleTexure;
		m_UpSampleTexure = 0;
	}

	// Release the full screen ortho window object.
	if (m_FullScreenWindow)
	{
		m_FullScreenWindow->Shutdown();
		delete m_FullScreenWindow;
		m_FullScreenWindow = 0;
	}

	// Release the soft shadow shader object.
	if (m_SoftShadowShader)
	{
		m_SoftShadowShader->Shutdown();
		delete m_SoftShadowShader;
		m_SoftShadowShader = 0;
	}
	
	if (m_correctionTextureUAV)
	{
		m_correctionTextureUAV->Release();
		m_correctionTextureUAV = 0;
	}

	if (m_correctionTextureSRV)
	{
		m_correctionTextureSRV->Release();
		m_correctionTextureSRV = 0;
	}

	if (m_correctionTexture)
	{
		m_correctionTexture->Release();
		m_correctionTexture = 0;
	}

	pRTVs[0] = NULL;
	pRTVs[1] = NULL;


	return;
}


bool GraphicsClass::Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ, float &Blur_Size, int &Blur_Sample_Count, float &Blur_Standard_Deviation, float &upsamplingLowerLimit, float &upsamplingUpperLimit)
{
	bool result;
	static float lightPositionX = -5.0f;


	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	// Update the position of the light each frame.
	lightPositionX += 0.05f;
	if(lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	// Update the position of the light.
	m_Light->SetPosition(lightPositionX, 8.0f, -5.0f);

	// Render the graphics scene.
	result = Render(Blur_Size, Blur_Sample_Count, Blur_Standard_Deviation, upsamplingLowerLimit, upsamplingUpperLimit);
	if(!result)
	{
		return false;
	}

	return true;
}

float GraphicsClass::GetDownSampleSizeX()
{
	return downSampleSizeX;
}

float GraphicsClass::GetDownSmapleSizeY()
{
	return downSampleSizeY;
}


bool GraphicsClass::EdgeVertexDetection_Part_01_Function()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	float posX, posY, posZ;
	bool result;

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f, true);

	// Get the world matrix from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the cube model with the depth shader.
	//m_SphereModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_01->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_CubeModel->GetColor());
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the sphere model with the depth shader.
	//m_SphereModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_01->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_SphereModel->GetColor());
	if (!result)
	{
		return false;
	}

	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the ground model with the depth shader.
	//m_GroundModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_01->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_GroundModel->GetColor());
	if(!result)
	{
		return false;
	}
	

	return true;
}

bool GraphicsClass::EdgeVertexDetection_Part_02_Function()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	float posX, posY, posZ;
	bool result;

	ID3D11ShaderResourceView* prevBackBuffer = m_D3D->GetBackBufferSRV(false);

	//m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f, true);

	// Get the world matrix from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the cube model with the depth shader.
	//m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_02->Render(m_D3D->GetDeviceContext(), 
														m_D3D->GetHWND(), prevBackBuffer, 
															worldMatrix, 
																viewMatrix, 
																	projectionMatrix, 
																		m_CubeModel->GetVertexCount(), 
																			m_CubeModel->GetDispatchSize(),
																				m_CubeModel->GetVertexData_UAV(),
																					m_CubeModel->GetColor(),
																						GetDownSampleSizeX(),
																							GetDownSmapleSizeY());
	if (!result)
	{
		return false;
	}
	
	m_CubeModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());

	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the sphere model with the depth shader.
	//m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_02->Render(m_D3D->GetDeviceContext(),
														m_D3D->GetHWND(), 
															prevBackBuffer, 
																worldMatrix, 
																	viewMatrix, 
																		projectionMatrix, 
																			m_SphereModel->GetVertexCount(), 
																				m_SphereModel->GetDispatchSize(), 
																					m_SphereModel->GetVertexData_UAV(),
																						m_SphereModel->GetColor(),
																							GetDownSampleSizeX(),
																								GetDownSmapleSizeY());
	if (!result)
	{
		return false;
	}

	m_SphereModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());

	/*
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the ground model with the depth shader.
	//m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_02->Render(m_D3D->GetDeviceContext(), m_D3D->GetHWND(), prevBackBuffer, worldMatrix, viewMatrix, projectionMatrix, m_GroundModel->GetVertexCount(), m_GroundModel->GetDispatchSize(), m_GroundModel->GetVertexData_UAV());
	if (!result)
	{
		return false;
	}
	*/

	return true;
}

bool GraphicsClass::EdgeVertexDetection_Part_03_Function()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	float posX, posY, posZ;
	bool result;

	//ID3D11ShaderResourceView* prevBackBuffer = m_D3D->GetBackBufferSRV(false);

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f, true);

	// Get the world matrix from the d3d object.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Generate the view matrix based on the camera's position.
	//m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);

	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the cube model with the depth shader.
	//m_CubeModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_03->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
	
	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the sphere model with the depth shader.
	//m_SphereModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_03->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}


	// Reset the world matrix.
	m_D3D->GetWorldMatrix(worldMatrix);

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(posX, posY, posZ);
	worldMatrix = XMMatrixTranslation(posX, posY, posZ);

	// Render the ground model with the depth shader.
	//m_GroundModel->RemoveVertexData_UAV_FromPipeline(m_D3D->GetDeviceContext());
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	result = m_EdgeVertexDetection_Part_03->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	/*
	ID3D11ShaderResourceView* nullSRV[] = { NULL };
	ID3D11SamplerState* nullSampler[] = { NULL };
	m_D3D->GetDeviceContext()->PSSetShaderResources(0, 1, nullSRV);
	m_D3D->GetDeviceContext()->PSSetSamplers(0, 1, nullSampler);
	*/
	

	return true;
}



bool GraphicsClass::Render(float &Blur_Size, int &Blur_Sample_Count, float &Blur_Standard_Deviation, float &upsamplingLowerLimit, float &upsamplingUpperLimit)
{

	/////////////////////////////
	// CREATE A MAT-ID TEXTURE //
	/////////////////////////////


	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);


	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(r_viewMatrix);

	m_D3D->GetProjectionMatrix(r_projectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the cube model with the depth shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	r_result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, XMFLOAT4(0.2f, 0.0f, 0.0f, 1.0f));
	if (!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the sphere model with the depth shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	r_result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f));
	if (!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	m_GroundModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the sphere model with the depth shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	r_result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));
	if (!r_result)
	{
		return false;
	}


	//////////////////////////////////////////////
	// FILL DEPTH BUFFER FROM LIGHT PERSPECTIVE //
	//////////////////////////////////////////////

	m_D3D->SetBackBufferRenderTarget();

	m_D3D->ResetViewport();

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f, true);

	m_D3D->TurnFrontFaceCullingOn();



	// Generate the light view matrix based on the light's position.
	m_Light->GenerateViewMatrix();

	// Get the view and orthographic matrices from the light object.
	m_Light->GetViewMatrix(r_lightViewMatrix);
	m_Light->GetProjectionMatrix(r_lightProjectionMatrix);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the cube model with the depth shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	r_result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), r_worldMatrix, r_lightViewMatrix, r_lightProjectionMatrix, XMFLOAT4(0.4f, 0.6f, 0.77f, 1.0f));
	if (!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the sphere model with the depth shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	r_result = m_DepthShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), r_worldMatrix, r_lightViewMatrix, r_lightProjectionMatrix, XMFLOAT4(0.4f, 0.6f, 0.77f, 1.0f));
	if (!r_result)
	{
		return false;
	}

	m_D3D->TurnBackFaceCullingOn();

	////////////////////////////////////
	// RENDER BLACK AND WHITE SHADOWS //
	////////////////////////////////////

	// Set the render target to be the render to texture.
	m_BlackWhiteRenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_BlackWhiteRenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	

	ID3D11ShaderResourceView* DepthMap = m_D3D->GetDepthBufferSRV(false);

	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the cube model using the shadow shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	r_result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, r_lightViewMatrix,
		r_lightProjectionMatrix, DepthMap, m_Light->GetPosition());
	if (!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the sphere model using the shadow shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	r_result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, r_lightViewMatrix,
		r_lightProjectionMatrix, DepthMap, m_Light->GetPosition());
	if (!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the ground model using the shadow shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	r_result = m_ShadowShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, r_lightViewMatrix,
		r_lightProjectionMatrix, DepthMap, m_Light->GetPosition());
	if (!r_result)
	{
		return false;
	}

	/*
	////////////////////////////
	// DOWNSAMPLE THE TEXTURE //
	////////////////////////////

	// Set the render target to be the render to texture.
	m_DownSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_DownSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	


	// Get the world and view matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(r_worldMatrix);

	// Get the base matrix
	m_Camera->GetBaseViewMatrix(r_baseViewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	m_DownSampleTexure->GetOrthoMatrix(r_orthoMatrix);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());

	// Render the small ortho window using the texture shader and the render to texture of the scene as the texture resource.
	r_result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_SmallWindow->GetIndexCount(), r_worldMatrix, r_baseViewMatrix, r_orthoMatrix,
											m_BlackWhiteRenderTexture->GetShaderResourceView());
	if (!r_result)
	{
		return false;
	}
	*/
	


	////////////////////////////////////
	// RENDER HORIZONTAL BLUR TEXTURE //
	////////////////////////////////////
	
	float gausStDeviationSquared = Blur_Standard_Deviation * Blur_Standard_Deviation;
	float gausFirstPart = (1 / sqrt(2 * 3.14159265359*gausStDeviationSquared));
	
	//m_HorizontalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext()); 

	// Clear the render to texture.
	m_HorizontalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	m_RenderTexture2->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	pRTVs[0] = m_HorizontalBlurTexture->GetRenderTargetView();
	pRTVs[1] = m_RenderTexture2->GetRenderTargetView();
	m_D3D->GetDeviceContext()->OMSetRenderTargets(2, &pRTVs[0], NULL);

	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();


	// Get the world and view matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(r_worldMatrix);

	// Get the base matrix
	m_Camera->GetBaseViewMatrix(r_baseViewMatrix);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_HorizontalBlurTexture->GetOrthoMatrix(r_orthoMatrix);

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());

	m_CubeModel->GetPosition(r_posX, r_posY, r_posZ);

	// Render the small ortho window using the horizontal blur shader and the down sampled render to texture resource.
	r_result = m_HorizontalBlurShader->Render(m_D3D->GetDeviceContext(), 
		m_SmallWindow->GetIndexCount(),
		r_worldMatrix, 
		r_baseViewMatrix, 
		r_orthoMatrix,
		m_BlackWhiteRenderTexture->GetShaderResourceView(), 
		texelSizeX,
		aspectRatioInv,
		m_RenderTexture->GetShaderResourceView(),
		Blur_Size,
		Blur_Sample_Count,
		gausStDeviationSquared,
		gausFirstPart);

	if (!r_result)
	{
		return false;
	}
	
	pRTVs[0] = NULL;
	pRTVs[1] = NULL;
	m_D3D->GetDeviceContext()->OMSetRenderTargets(2, &pRTVs[0], NULL);
	

	//////////////////////////////////
	// RENDER VERTICAL BLUR TEXTURE //
	//////////////////////////////////


	//m_VerticalBlurTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_VerticalBlurTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);
	m_RenderTexture3->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	pRTVs[0] = m_VerticalBlurTexture->GetRenderTargetView();
	pRTVs[1] = m_RenderTexture3->GetRenderTargetView();

	m_D3D->GetDeviceContext()->OMSetRenderTargets(2, &pRTVs[0], NULL);

	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_VerticalBlurTexture->GetOrthoMatrix(r_orthoMatrix);

	// Put the small ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_SmallWindow->Render(m_D3D->GetDeviceContext());

	// Render the small ortho window using the vertical blur shader and the horizontal blurred render to texture resource.
	r_result = m_VerticalBlurShader->Render(m_D3D->GetDeviceContext(), 
		m_SmallWindow->GetIndexCount(), 
		r_worldMatrix, 
		r_baseViewMatrix, 
		r_orthoMatrix,
		m_HorizontalBlurTexture->GetShaderResourceView(), // horizontals main output
		texelSizeY, 
		m_RenderTexture->GetShaderResourceView(), // mat-id
		m_RenderTexture2->GetShaderResourceView(), // horizontals secondary output
		Blur_Size,
		Blur_Sample_Count,
		gausStDeviationSquared,
		gausFirstPart);
	
	if (!r_result)
	{
		return false;
	}



	m_D3D->GetDeviceContext()->PSSetShaderResources(0, 3, nullShaderResourceView);
	m_D3D->GetDeviceContext()->PSSetSamplers(1, 1, nullSampleState);
	pRTVs[0] = NULL;
	pRTVs[1] = NULL;
	m_D3D->GetDeviceContext()->OMSetRenderTargets(2, &pRTVs[0], NULL);

	
	////////////////////////////
	// UPSAMPLING THE TEXTURE //
	////////////////////////////

	// Set the render target to be the render to texture.
	m_UpSampleTexure->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the render to texture.
	m_UpSampleTexure->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);



	// Get the ortho matrix from the render to texture since texture has different dimensions.
	m_UpSampleTexure->GetOrthoMatrix(r_orthoMatrix);

	// Put the full screen ortho window vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_FullScreenWindow->Render(m_D3D->GetDeviceContext());

	// Render the full screen ortho window using the texture shader and the small sized final blurred render to texture resource.
	r_result = m_textureUpSamplingCorrection->Render(m_D3D->GetDeviceContext(), m_FullScreenWindow->GetIndexCount(), r_worldMatrix, r_baseViewMatrix, r_orthoMatrix,
											m_VerticalBlurTexture->GetShaderResourceView(), m_RenderTexture->GetShaderResourceView(), m_RenderTexture3->GetShaderResourceView(), upsamplingLowerLimit, upsamplingUpperLimit);
	if (!r_result)
	{
		return false;
	}
	
	m_D3D->GetDeviceContext()->PSSetShaderResources(0, 1, nullShaderResourceView);
	m_D3D->GetDeviceContext()->PSSetShaderResources(1, 1, nullShaderResourceView);
	m_D3D->GetDeviceContext()->PSSetShaderResources(2, 1, nullShaderResourceView);
	m_D3D->GetDeviceContext()->PSSetSamplers(1, 1, nullSampleState);

	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();
	
	

	/////////////////////////////
	// PUTTING IT ALL TOGETHER //
	/////////////////////////////
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f, true);


	// Setup the translation matrix for the cube model.
	m_CubeModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);
	
	// Render the cube model using the soft shadow shader.
	m_CubeModel->Render(m_D3D->GetDeviceContext());
	r_result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_CubeModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix,
												m_CubeModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(), 
													m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!r_result)
	{
		return false;
	}

	// Setup the translation matrix for the sphere model.
	m_SphereModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the sphere model using the soft shadow shader.
	m_SphereModel->Render(m_D3D->GetDeviceContext());
	r_result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_SphereModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, 
												m_SphereModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(),
													m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!r_result)
	{
		return false;
	}


	// Setup the translation matrix for the ground model.
	m_GroundModel->GetPosition(r_posX, r_posY, r_posZ);
	r_worldMatrix = XMMatrixTranslation(r_posX, r_posY, r_posZ);

	// Render the ground model using the soft shadow shader.
	m_GroundModel->Render(m_D3D->GetDeviceContext());
	r_result = m_SoftShadowShader->Render(m_D3D->GetDeviceContext(), m_GroundModel->GetIndexCount(), r_worldMatrix, r_viewMatrix, r_projectionMatrix, 
												m_GroundModel->GetTexture(), m_UpSampleTexure->GetShaderResourceView(), m_Light->GetPosition(),
													m_Light->GetAmbientColor(), m_Light->GetDiffuseColor());
	if(!r_result)
	{
		return false;
	}

	m_D3D->GetDeviceContext()->PSSetShaderResources(1, 1, nullShaderResourceView);
	m_D3D->GetDeviceContext()->PSSetShaderResources(2, 1, nullShaderResourceView);

	// Present the rendered scene to the screen.
	//m_D3D->EndScene(); <--- will get called by imgui in the systemclass.cpp frame function since the gui needs to render last



	return true;
}

D3DClass* GraphicsClass::Get_D3D()
{
	return m_D3D;
}