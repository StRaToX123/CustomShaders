////////////////////////////////////////////////////////////////////////////////
// Filename: depthshaderclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "edgevertexdetection_part_02.h"


EdgeVertexDetection_Part_02_CLASS::EdgeVertexDetection_Part_02_CLASS()
{
	m_byteBuffer = 0;
	m_byteBuffer_SRV = 0;
	//m_EdgeVertedDetection_Part_02_Data_Buffer = 0;
	m_EdgeVertexDetection_Part_02_cs = 0;
	//m_EdgeVertexDetection_Part_02_Data_Buffer_UAV = 0;
	float_array = new float[(16 * 3) + 4]; // +2 because one float is for the vertexCount and the other for the object Color
	m_sampleStateClamp = 0;
}


EdgeVertexDetection_Part_02_CLASS::EdgeVertexDetection_Part_02_CLASS(const EdgeVertexDetection_Part_02_CLASS& other)
{
}


EdgeVertexDetection_Part_02_CLASS::~EdgeVertexDetection_Part_02_CLASS()
{
}


bool EdgeVertexDetection_Part_02_CLASS::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;


	// Initialize the vertex and pixel shaders.
	result = InitializeShader(device, hwnd, L"../Engine/EdgeVertexDetection_Part_02_cs.hlsl");
	if (!result)
	{
		return false;
	}

	return true;
}


void EdgeVertexDetection_Part_02_CLASS::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
	ShutdownShader();

	return;
}


bool __vectorcall EdgeVertexDetection_Part_02_CLASS::Render(ID3D11DeviceContext* deviceContext, 
																	HWND* hwnd,
																		ID3D11ShaderResourceView* prevBackBuffer, 
																			XMMATRIX worldMatrix, 
																				XMMATRIX viewMatrix,
																					XMMATRIX projectionMatrix,
																						int vertexCount,
																							int dispatchSize,
																								ID3D11UnorderedAccessView* vertexData_UAV,
																									float color,
																										float downSampleSizeX,
																											float downSampleSizeY)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;


	// Transpose the matrices to prepare them for the shader.
	worldMatrix = XMMatrixTranspose(worldMatrix);
	viewMatrix = XMMatrixTranspose(viewMatrix);
	projectionMatrix = XMMatrixTranspose(projectionMatrix);

	
	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(m_byteBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, worldMatrix);
	(*(float_array)) = tmp.m[0][0];
	(*(float_array + 1)) = tmp.m[0][1];
	(*(float_array + 2)) = tmp.m[0][2];
	(*(float_array + 3)) = tmp.m[0][3];
	(*(float_array + 4)) = tmp.m[1][0];
	(*(float_array + 5)) = tmp.m[1][1];
	(*(float_array + 6)) = tmp.m[1][2];
	(*(float_array + 7)) = tmp.m[1][3];
	(*(float_array + 8)) = tmp.m[2][0];
	(*(float_array + 9)) = tmp.m[2][1];
	(*(float_array + 10)) = tmp.m[2][2];
	(*(float_array + 11)) = tmp.m[2][3];
	(*(float_array + 12)) = tmp.m[3][0];
	(*(float_array + 13)) = tmp.m[3][1];
	(*(float_array + 14)) = tmp.m[3][2];
	(*(float_array + 15)) = tmp.m[3][3];

	XMStoreFloat4x4(&tmp, viewMatrix);
	(*(float_array + 16)) = tmp.m[0][0];
	(*(float_array + 17)) = tmp.m[0][1];
	(*(float_array + 18)) = tmp.m[0][2];
	(*(float_array + 19)) = tmp.m[0][3];
	(*(float_array + 20)) = tmp.m[1][0];
	(*(float_array + 21)) = tmp.m[1][1];
	(*(float_array + 22)) = tmp.m[1][2];
	(*(float_array + 23)) = tmp.m[1][3];
	(*(float_array + 24)) = tmp.m[2][0];
	(*(float_array + 25)) = tmp.m[2][1];
	(*(float_array + 26)) = tmp.m[2][2];
	(*(float_array + 27)) = tmp.m[2][3];
	(*(float_array + 28)) = tmp.m[3][0];
	(*(float_array + 29)) = tmp.m[3][1];
	(*(float_array + 30)) = tmp.m[3][2];
	(*(float_array + 31)) = tmp.m[3][3];

	XMStoreFloat4x4(&tmp, projectionMatrix);
	(*(float_array + 32)) = tmp.m[0][0];
	(*(float_array + 33)) = tmp.m[0][1];
	(*(float_array + 34)) = tmp.m[0][2];
	(*(float_array + 35)) = tmp.m[0][3];
	(*(float_array + 36)) = tmp.m[1][0];
	(*(float_array + 37)) = tmp.m[1][1];
	(*(float_array + 38)) = tmp.m[1][2];
	(*(float_array + 39)) = tmp.m[1][3];
	(*(float_array + 40)) = tmp.m[2][0];
	(*(float_array + 41)) = tmp.m[2][1];
	(*(float_array + 42)) = tmp.m[2][2];
	(*(float_array + 43)) = tmp.m[2][3];
	(*(float_array + 44)) = tmp.m[3][0];
	(*(float_array + 45)) = tmp.m[3][1];
	(*(float_array + 46)) = tmp.m[3][2];
	(*(float_array + 47)) = tmp.m[3][3];

	(*(float_array + 48)) = (float)vertexCount;

	(*(float_array + 49)) = color;

	(*(float_array + 50)) = downSampleSizeX;

	(*(float_array + 51)) = downSampleSizeY;


	memcpy(mappedResource.pData, (void*)float_array, sizeof(float) * 52);

	// Unlock the constant buffer.
	deviceContext->Unmap(m_byteBuffer, 0);

	// Set shader constant buffer in the compute shader
	deviceContext->CSSetShaderResources(0, 1, &m_byteBuffer_SRV);

	// Set shader texture resource in the compute shader
	deviceContext->CSSetShaderResources(1, 1, &prevBackBuffer);

	// Set shader UAV resource in the compute shader
	deviceContext->CSSetUnorderedAccessViews(1, 1, &vertexData_UAV, 0);

	// Set SampleState
	deviceContext->CSSetSamplers(0, 1, &m_sampleStateClamp);

	// Set Compute Shader
	deviceContext->CSSetShader(m_EdgeVertexDetection_Part_02_cs, NULL, 0);

	///////////////////////////////////
	///////////
	deviceContext->Dispatch(dispatchSize, 1, 1);
	///////////
	///////////////////////////////////



	return true;
}


bool EdgeVertexDetection_Part_02_CLASS::InitializeShader(ID3D11Device* device, 
															HWND hwnd,
																WCHAR* csFilename)
{
	HRESULT result;
	
	ID3DBlob* EdgeVertexDetection_Part_02_CS_Blob;
	result = CompileComputeShader(L"../Engine/EdgeVertexDetection_Part_02_cs.hlsl", "main", device, &EdgeVertexDetection_Part_02_CS_Blob, hwnd);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"CompileComputeShader has FAILED", L"Error", MB_OK);
		return false;
	}

	// Create compute shader
	result = device->CreateComputeShader(EdgeVertexDetection_Part_02_CS_Blob->GetBufferPointer(), EdgeVertexDetection_Part_02_CS_Blob->GetBufferSize(), nullptr, &m_EdgeVertexDetection_Part_02_cs);

	EdgeVertexDetection_Part_02_CS_Blob->Release();
	EdgeVertexDetection_Part_02_CS_Blob = 0;

	if (FAILED(result))
	{
		MessageBox(hwnd, L" Create Compute Shade EdgeVertexDetection_Part_02 FAILED", L"Error", MB_OK);
		return false;
	}


	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	D3D11_BUFFER_DESC byteBufferDesc;
	ZeroMemory(&byteBufferDesc, sizeof(byteBufferDesc));
	byteBufferDesc.ByteWidth = ((sizeof(float) * 16) * 3) + (sizeof(float) * 2); // 3 XMMATRIX + vertexCount FLOAT
	byteBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	byteBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	byteBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	byteBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	byteBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the cs shader constant buffer from within this class.
	result = device->CreateBuffer(&byteBufferDesc, NULL, &m_byteBuffer);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"EdgeVertexDetection_Part_02_CLASS Initialize Shaders function has failed to create the byteAdressBuffer", L"Error", MB_OK);
		return false;
	}

	// Create an SRV for the ByteAdressBuffer
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	ZeroMemory(&srv_desc, sizeof(srv_desc));
	srv_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srv_desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
	srv_desc.BufferEx.FirstElement = 0;
	srv_desc.BufferEx.NumElements = byteBufferDesc.ByteWidth / 4;

	result = device->CreateShaderResourceView(m_byteBuffer, &srv_desc, &m_byteBuffer_SRV);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"EdgeVertexDetection_Part_02_CLASS Initialize Shaders function has failed to an SRV out of the ByteAdressBuffer", L"Error", MB_OK);
		return false;
	}

	// Create a clamp texture sampler state description.
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.5f;
	samplerDesc.BorderColor[1] = 0.5f;
	samplerDesc.BorderColor[2] = 0.5f;
	samplerDesc.BorderColor[3] = 0.5f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = 0;

	// Create the texture sampler state.
	result = device->CreateSamplerState(&samplerDesc, &m_sampleStateClamp);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}


void EdgeVertexDetection_Part_02_CLASS::ShutdownShader()
{
	if (m_EdgeVertexDetection_Part_02_cs)
	{
		m_EdgeVertexDetection_Part_02_cs->Release();
		m_EdgeVertexDetection_Part_02_cs = 0;
	}

	if (m_byteBuffer)
	{
		m_byteBuffer->Release();
		m_byteBuffer = 0;
	}

	if (m_byteBuffer_SRV)
	{
		m_byteBuffer_SRV->Release();
		m_byteBuffer_SRV = 0;
	}
	
	delete[] float_array;
	float_array = nullptr;

	if (m_sampleStateClamp)
	{
		m_sampleStateClamp->Release();
		m_sampleStateClamp = 0;
	}
	
	return;
}


void EdgeVertexDetection_Part_02_CLASS::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

HRESULT EdgeVertexDetection_Part_02_CLASS::CompileComputeShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob, HWND hwnd)
{
	if (!srcFile || !entryPoint || !device || !blob)
	{
		return E_INVALIDARG;
	}

	*blob = nullptr;

	//UINT flags = 0;// = D3DCOMPILE_ENABLE_STRICTNESS;

//#if defined( DEBUG ) || defined( _DEBUG )

	//flags |= D3DCOMPILE_DEBUG;
	//flags |= D3DCOMPILE_SKIP_OPTIMIZATION;

//#endif

	// We generally prefer to use the higher CS shader profile when possible as CS 5.0 is better performance on 11-class hardware
	LPCSTR profile;
	if (device->GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0)
	{
		profile = "cs_5_0";
	}
	else
	{
		profile = "cs_4_0";
	}

	const D3D_SHADER_MACRO defines[] = { "EXAMPLE_DEFINE", "1", NULL, NULL };

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			//OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			char* text = (char*)errorBlob->GetBufferPointer();
			wchar_t wtext[500];
			mbstowcs(wtext, text, strlen(text) + 1);
			LPWSTR ptr = wtext;
			MessageBox(hwnd, wtext, L"Error", MB_OK);
			text = nullptr;
			errorBlob->Release();
		}

		if (shaderBlob)
		{
			shaderBlob->Release();
		}


		return hr;
	}

	*blob = shaderBlob;

	return hr;

}

