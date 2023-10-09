////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
//#include <d3dx10math.h>
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include <vector>
#include "ComputeShadeTypes.h"

// Assimp
#include "assimp\Importer.hpp"
#include "assimp\scene.h"
#include "assimp\postprocess.h"
#include "assimp\material.h"


using namespace std;
using namespace Assimp;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
	
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;
		unsigned int isEdgeVertex;
	};

	

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*, HWND* hwnd, float color);
	bool InitializeMesh(ID3D11Device* device, char* modelFilename, HWND* hwnd, float color);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	ModelType* ReturnM_Model_Pointer();
	void RemoveVertexData_UAV_FromPipeline(ID3D11DeviceContext* deviceContext);

	int GetIndexCount();
	int GetVertexCount();
	int GetDispatchSize();
	ID3D11ShaderResourceView* GetTexture();
	float GetColor();
	
	void SetPosition(float, float, float);
	void GetPosition(float&, float&, float&);
	ID3D11UnorderedAccessView* GetVertexData_UAV();

	bool LoadMesh(const char* Filename, HWND* hwnd);

private:
	bool InitializeBuffers(ID3D11Device*, HWND* hwnd);
	bool InitializeBuffersMesh(ID3D11Device*, HWND* hwnd, VertexType*& LoadMesh_Vertices, unsigned int*& LoadMesh_Indices);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();

private:
	VertexType* LoadMesh_Vertices;
	unsigned int* LoadMesh_Indices;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	ID3D11UnorderedAccessView* m_vertexData_UAV;
	ID3D11UnorderedAccessView* nullUAV;
	int m_dispatchSize;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	ModelType* m_model;
	float m_positionX, m_positionY, m_positionZ;
	float m_Color;
};

#endif