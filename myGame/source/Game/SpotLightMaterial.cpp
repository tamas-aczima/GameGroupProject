#include "SpotLightMaterial.h"
#include "GameException.h"
#include "Mesh.h"

namespace Rendering
{
	RTTI_DEFINITIONS(SpotLightMaterial)

		SpotLightMaterial::SpotLightMaterial()
		: Material("main11"),	
		MATERIAL_VARIABLE_INITIALIZATION(WVP),
		MATERIAL_VARIABLE_INITIALIZATION(World),
		MATERIAL_VARIABLE_INITIALIZATION(position),
		MATERIAL_VARIABLE_INITIALIZATION(range),
		MATERIAL_VARIABLE_INITIALIZATION(dir),
		MATERIAL_VARIABLE_INITIALIZATION(cone),
		MATERIAL_VARIABLE_INITIALIZATION(ambient),
		MATERIAL_VARIABLE_INITIALIZATION(diffuse)

	{
	}

		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, WVP)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, World)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, position)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, range)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, dir)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, cone)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, ambient)
		MATERIAL_VARIABLE_DEFINITION(SpotLightMaterial, diffuse)


		void SpotLightMaterial::Initialize(Effect* effect)
	{
		Material::Initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(WVP)
			MATERIAL_VARIABLE_RETRIEVE(World)
			MATERIAL_VARIABLE_RETRIEVE(position)
			MATERIAL_VARIABLE_RETRIEVE(range)
			MATERIAL_VARIABLE_RETRIEVE(dir)
			MATERIAL_VARIABLE_RETRIEVE(cone)
			MATERIAL_VARIABLE_RETRIEVE(ambient)
			MATERIAL_VARIABLE_RETRIEVE(diffuse)

			D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

	

		CreateInputLayout("main11", "p0", inputElementDescriptions, ARRAYSIZE(inputElementDescriptions));
	}

	void SpotLightMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(textureCoordinates->size() == sourceVertices.size());

		std::vector<SpotLightMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			vertices.push_back(SpotLightMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal));
		}
		CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	void SpotLightMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		const std::vector<XMFLOAT3>& normals = mesh.Normals();
		assert(textureCoordinates->size() == sourceVertices.size());

		std::vector<SpotLightMaterialVertex> vertices;
		vertices.reserve(sourceVertices.size());
		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			XMFLOAT3 normal = normals.at(i);
			vertices.push_back(SpotLightMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal));
		}

		//CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
	}

	//void SpotLightMaterial::CreateVertexBuffer(ID3D11Device* device, SpotLightMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
	//{
	//	D3D11_BUFFER_DESC vertexBufferDesc;
	//	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	//	vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
	//	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	//	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	//	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	//	vertexSubResourceData.pSysMem = vertices;
	//	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
	//	{
	//		throw GameException("ID3D11Device::CreateBuffer() failed.");
	//	}
	//}

	UINT SpotLightMaterial::VertexSize() const
	{
		return sizeof(SpotLightMaterialVertex);
	}
}