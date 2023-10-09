#ifndef _COMPUTER_SHADER_IMPLEMENTATION_

#define _COMPUTER_SHADER_IMPLEMENTATION_




//#define _WIN32_WINNT 0x600  // minimum Windows Vista
#include <stdio.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")

#define DEBUG


HRESULT MyCompileComputeShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ ID3D11Device* device, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !device || !blob)
	{
		return E_INVALIDARG;
	}
		
	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;

#if defined( DEBUG ) || defined( _DEBUG )

	flags |= D3DCOMPILE_DEBUG;

#endif

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

	const D3D_SHADER_MACRO defines[] ={ "EXAMPLE_DEFINE", "1", NULL, NULL };

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, profile, flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
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




#endif // !_COMPUTER_SHADER_IMPLEMENTATION_


