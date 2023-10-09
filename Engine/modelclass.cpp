////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_Texture = 0;
	m_model = 0;
	m_vertexData_UAV = 0;
	nullUAV = NULL;
	LoadMesh_Vertices = 0;
	LoadMesh_Indices = 0;
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}

ModelClass::ModelType* ModelClass::ReturnM_Model_Pointer()
{
	return this->m_model;
}

bool ModelClass::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename, HWND* hwnd, float color)
{
	bool result;

	m_Color = color;

	// Load in the model data,
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device, hwnd);
	if(!result)
	{
		return false;
	}

	// Load the texture for this model.
	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeMesh(ID3D11Device* device, char* modelFilename, HWND* hwnd, float color)
{
	bool result;

	m_Color = color;

	// Load in the model data,
	result = LoadMesh(modelFilename, hwnd);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffersMesh(device, hwnd, LoadMesh_Vertices, LoadMesh_Indices);
	if (!result)
	{
		return false;
	}


	return true;
}


void ModelClass::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();

	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}

int ModelClass::GetVertexCount()
{
	return m_vertexCount;
}

int ModelClass::GetDispatchSize()
{
	return m_dispatchSize;
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}


bool ModelClass::InitializeBuffers(ID3D11Device* device, HWND* hwnd)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// CS EdgeVertexDetection_Part_02 runs on a workGroup of [64, 1, 1] so out DataArray needs to be a multiple of 64
	m_dispatchSize = m_vertexCount / 64;
	m_dispatchSize++;

	for (i = 0; i< m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].isEdgeVertex = 0;

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;

	ZeroMemory(&uav_desc, sizeof(uav_desc));

	uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.FirstElement = 0;
	uav_desc.Buffer.NumElements = m_vertexCount * 9; // number of r32 elements in the entire buffer, there are 9 r32 elements per vertex (see vertexType)
	uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	result = device->CreateUnorderedAccessView(m_vertexBuffer, &uav_desc, &m_vertexData_UAV);
	if (FAILED(result))
	{
		MessageBox((*(hwnd)), L"EdgeVertexDetection_Part_02_CLASS InitializeShaders function has failed to create the UAV", L"Error", MB_OK);
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;


	return true;
}


bool ModelClass::InitializeBuffersMesh(ID3D11Device* device, HWND* hwnd, VertexType*& LoadMesh_Vertices, unsigned int*& LoadMesh_Indices)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;


	// CS EdgeVertexDetection_Part_02 runs on a workGroup of [64, 1, 1] so out DataArray needs to be a multiple of 64
	m_dispatchSize = m_vertexCount / 64;
	m_dispatchSize++;


	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_UNORDERED_ACCESS;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = LoadMesh_Vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if (FAILED(result))
	{
		MessageBox((*(hwnd)), L"ModelClass InitializeBuffersMesh has FAILED to create a vertex buffer", L"Error", MB_OK);
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc;

	ZeroMemory(&uav_desc, sizeof(uav_desc));

	uav_desc.Format = DXGI_FORMAT_R32_TYPELESS;
	uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	uav_desc.Buffer.FirstElement = 0;
	uav_desc.Buffer.NumElements = m_vertexCount * 9; // number of r32 elements in the entire buffer, there are 9 r32 elements per vertex (see vertexType)
	uav_desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;

	result = device->CreateUnorderedAccessView(m_vertexBuffer, &uav_desc, &m_vertexData_UAV);
	if (FAILED(result))
	{
		MessageBox((*(hwnd)), L"ModelClass InitializeBuffersMesh has FAILED to create a vertex buffer UAV", L"Error", MB_OK);
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = LoadMesh_Indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if (FAILED(result))
	{
		MessageBox((*(hwnd)), L"ModelClass InitializeBuffersMesh has FAILED to create a index buffer", L"Error", MB_OK);
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] LoadMesh_Vertices;
	LoadMesh_Vertices = 0;

	delete[] LoadMesh_Indices;
	LoadMesh_Indices = 0;


	return true;
}

void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	if (m_vertexData_UAV)
	{
		m_vertexData_UAV->Release();
		m_vertexData_UAV = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	if (LoadMesh_Vertices)
	{
		delete[] LoadMesh_Vertices;
		LoadMesh_Vertices = 0;
	}

	if (LoadMesh_Indices)
	{
		delete[] LoadMesh_Indices;
		LoadMesh_Indices = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;


	// Create the texture object.
	m_Texture = new TextureClass;
	if(!m_Texture)
	{
		return false;
	}

	// Initialize the texture object.
	result = m_Texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}

	return true;
}


void ModelClass::ReleaseTexture()
{
	// Release the texture object.
	if(m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}

	return;
}


bool ModelClass::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);
	
	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_vertexCount];
	if(!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(i=0; i<m_vertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].tu >> m_model[i].tv;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void ModelClass::ReleaseModel()
{
	if(m_model)
	{
		delete [] m_model;
		m_model = 0;
	}

	return;
}

void ModelClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;
	return;
}


void ModelClass::GetPosition(float& x, float& y, float& z)
{
	x = m_positionX;
	y = m_positionY;
	z = m_positionZ;
	return;
}


float ModelClass::GetColor()
{
	return m_Color;
}

ID3D11UnorderedAccessView* ModelClass::GetVertexData_UAV()
{
	return m_vertexData_UAV;
}

void ModelClass::RemoveVertexData_UAV_FromPipeline(ID3D11DeviceContext* deviceContext)
{
	deviceContext->CSSetUnorderedAccessViews(1, 1, &nullUAV, NULL);
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

bool ModelClass::LoadMesh(const char* Filename, HWND* hwnd)
{
	Importer importer;
	const aiScene* scene = importer.ReadFile(Filename, NULL);
	
	if (!scene)
	{
		string temp = "LoadMesh aiImportFile FAILED for (";
		temp.append(Filename);
		temp.append(") [");
		temp = temp + importer.GetErrorString();
		temp.append("]");

		wstring stemp = s2ws(temp);
		LPCWSTR result = stemp.c_str();

		MessageBox((*(hwnd)), result, L"Error", MB_OK);
		return false;
	}
	


	aiMesh* mesh = scene->mMeshes[0];


	if (LoadMesh_Vertices)
	{
		delete[] LoadMesh_Vertices;
		LoadMesh_Vertices = 0;
	}
	if (LoadMesh_Indices)
	{
		delete[] LoadMesh_Indices;
		LoadMesh_Indices = 0;
	}

	struct MappedVertices
	{
		VertexType vt;
	    vector<unsigned int> mapping;
	};

	vector<VertexType> vertices;
	vector<MappedVertices> mapped_vertices;
	vector<unsigned int> indexes;

	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		VertexType temp;

		aiVector3D pos = mesh->mVertices[i];
		temp.position.x = pos.x;
		temp.position.y = pos.y;
		temp.position.z = pos.z;

		aiVector3D norm = mesh->mNormals[i];
		temp.normal.x = norm.x;
		temp.normal.y = norm.y;
		temp.normal.z = norm.z;

		temp.texture.x = 0.0f;
		temp.texture.y = 0.0f;

		temp.isEdgeVertex = 0;


		vertices.push_back(temp);
	}

	indexes.reserve(vertices.size());

	// mapping all the duplicate vertices
	MappedVertices mv;
	mv.vt = vertices[0];
	mv.mapping.push_back(0);
	mapped_vertices.push_back(mv);

	for (int i = 1; i < vertices.size(); i++)
	{
		bool is_mapped = false;
		for (int j = 0; j < mapped_vertices.size(); j++)
		{
			if ((vertices[i].position.x == mapped_vertices[j].vt.position.x) 
					&& (vertices[i].position.y == mapped_vertices[j].vt.position.y)
						&& (vertices[i].position.z == mapped_vertices[j].vt.position.z))

			{
				mapped_vertices[j].mapping.push_back(i);
				is_mapped = true;
				break;
			}
		}

		if (is_mapped == false)
		{
			mv.vt = vertices[i];
			mv.mapping.clear();
			mv.mapping.push_back(i);
			mapped_vertices.push_back(mv);
		}
	}

	// Creating the indexes array
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		for (int p = 0; p < 3; p++)
		{
			unsigned int found = 0;
			for (int j = 0; j < mapped_vertices.size(); j++)
			{
				bool call_break = false;
				for (int k = 0; k < mapped_vertices[j].mapping.size(); k++)
				{
					if (mesh->mFaces[i].mIndices[p] == mapped_vertices[j].mapping[k])
					{
						found = j;
						call_break = true;
						break;
					}
				}

				if (call_break == true)
				{
					break;
				}
			}

			indexes.push_back(found);
		}
	}


	LoadMesh_Vertices = new VertexType[mapped_vertices.size()];
	for (int i = 0; i < mapped_vertices.size(); i++)
	{
		LoadMesh_Vertices[i] = mapped_vertices[i].vt;
	}


	LoadMesh_Indices = new unsigned int[indexes.size()];
	for (int i = 0; i < indexes.size(); i++)
	{
		LoadMesh_Indices[i] = indexes[i];
	}

	m_vertexCount = mapped_vertices.size();
	m_indexCount = indexes.size();

}