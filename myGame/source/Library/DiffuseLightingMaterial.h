#pragma once

#include "Common.h"
#include "Material.h"
#include <DirectXCollision.h>

using namespace Library;

namespace Rendering
{
    typedef struct _DiffuseLightingMaterialVertex
    {
        XMFLOAT4 Position;
        XMFLOAT2 TextureCoordinates;
        XMFLOAT3 Normal;
		XMFLOAT4 worldPos;

        _DiffuseLightingMaterialVertex() { }

        _DiffuseLightingMaterialVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates, XMFLOAT3 normal, XMFLOAT4 worldPosition)
            : Position(position), TextureCoordinates(textureCoordinates), Normal(normal), worldPos(worldPosition) { }
    } DiffuseLightingMaterialVertex;

    class DiffuseLightingMaterial : public Material
    {
        RTTI_DECLARATIONS(DiffuseLightingMaterial, Material)

        MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
        MATERIAL_VARIABLE_DECLARATION(World)
		MATERIAL_VARIABLE_DECLARATION(ColorTexture)
		MATERIAL_VARIABLE_DECLARATION(pos)
		MATERIAL_VARIABLE_DECLARATION(range)
		MATERIAL_VARIABLE_DECLARATION(dir)
		MATERIAL_VARIABLE_DECLARATION(cone)
		MATERIAL_VARIABLE_DECLARATION(att)
		MATERIAL_VARIABLE_DECLARATION(ambient)
		MATERIAL_VARIABLE_DECLARATION(diffuse)

    public:
        DiffuseLightingMaterial();		

        virtual void Initialize(Effect* effect) override;
        virtual void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
        void CreateVertexBuffer(ID3D11Device* device, DiffuseLightingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
        virtual UINT VertexSize() const override;

		DirectX::BoundingBox mBoundingBox;
    };
}

