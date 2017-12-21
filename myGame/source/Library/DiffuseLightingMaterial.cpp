#include "DiffuseLightingMaterial.h"
#include "GameException.h"
#include "Mesh.h"

namespace Rendering
{
    RTTI_DEFINITIONS(DiffuseLightingMaterial)

    DiffuseLightingMaterial::DiffuseLightingMaterial()
        : Material("main11"),
        MATERIAL_VARIABLE_INITIALIZATION(WorldViewProjection), MATERIAL_VARIABLE_INITIALIZATION(World),
		MATERIAL_VARIABLE_INITIALIZATION(ColorTexture), MATERIAL_VARIABLE_INITIALIZATION(pos),
		MATERIAL_VARIABLE_INITIALIZATION(range), MATERIAL_VARIABLE_INITIALIZATION(dir),
		MATERIAL_VARIABLE_INITIALIZATION(cone), MATERIAL_VARIABLE_INITIALIZATION(att),
		MATERIAL_VARIABLE_INITIALIZATION(ambient), MATERIAL_VARIABLE_INITIALIZATION(diffuse)
    {
    }

    MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, WorldViewProjection)
    MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, World)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, ColorTexture)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, pos)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, range)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, dir)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, cone)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, att)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, ambient)
	MATERIAL_VARIABLE_DEFINITION(DiffuseLightingMaterial, diffuse)
	

    void DiffuseLightingMaterial::Initialize(Effect* effect)
    {
        Material::Initialize(effect);

        MATERIAL_VARIABLE_RETRIEVE(WorldViewProjection)
        MATERIAL_VARIABLE_RETRIEVE(World)
		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)
		MATERIAL_VARIABLE_RETRIEVE(pos)
		MATERIAL_VARIABLE_RETRIEVE(range)
		MATERIAL_VARIABLE_RETRIEVE(dir)
		MATERIAL_VARIABLE_RETRIEVE(cone)
		MATERIAL_VARIABLE_RETRIEVE(att)
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

    void DiffuseLightingMaterial::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
    {
        const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();
        std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
        assert(textureCoordinates->size() == sourceVertices.size());
        const std::vector<XMFLOAT3>& normals = mesh.Normals();
        assert(textureCoordinates->size() == sourceVertices.size());

		//generate the bounding box
		float min = -1e38f;
		float max = 1e38f;

		XMFLOAT3 vMinf3(max, max, max);
		XMFLOAT3 vMaxf3(min, min, min);

		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);

        std::vector<DiffuseLightingMaterialVertex> vertices;
        vertices.reserve(sourceVertices.size());
        for (UINT i = 0; i < sourceVertices.size(); i++)
        {
            XMFLOAT3 position = sourceVertices.at(i);
            XMFLOAT3 uv = textureCoordinates->at(i);
            XMFLOAT3 normal = normals.at(i);
            vertices.push_back(DiffuseLightingMaterialVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y), normal, XMFLOAT4(position.x, position.y, position.z, 1.0f)));

			//create the bounding box from the list of vertices
			XMVECTOR P = XMLoadFloat3(&position);
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
        }

		//final step to generate the bounding box

		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Center), 0.5f*(vMin + vMax));
		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Extents), 0.5f*(vMax - vMin));

        CreateVertexBuffer(device, &vertices[0], vertices.size(), vertexBuffer);
    }

    void DiffuseLightingMaterial::CreateVertexBuffer(ID3D11Device* device, DiffuseLightingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const
    {
        D3D11_BUFFER_DESC vertexBufferDesc;
        ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
        vertexBufferDesc.ByteWidth = VertexSize() * vertexCount;
        vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;		
        vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA vertexSubResourceData;
        ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
        vertexSubResourceData.pSysMem = vertices;
        if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
        {
            throw GameException("ID3D11Device::CreateBuffer() failed.");
        }
    }

    UINT DiffuseLightingMaterial::VertexSize() const
    {
        return sizeof(DiffuseLightingMaterialVertex);
    }
}