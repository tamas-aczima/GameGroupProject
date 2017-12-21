#pragma once
#include "Common.h"
#include "Material.h"

using namespace Library;

namespace Rendering
{
	struct Vertex    //Overloaded Vertex Structure
	{
		Vertex() {}
		Vertex(float x, float y, float z,
			float u, float v,
			float nx, float ny, float nz)
			: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz) {}

		XMFLOAT3 pos;
		XMFLOAT2 texCoord;
		XMFLOAT3 normal;
	};

	typedef struct _SpotLightMaterialVertex
	{
		XMFLOAT4 Position;
		XMFLOAT2 TextureCoordinates;
		XMFLOAT3 Normal;

		_SpotLightMaterialVertex() { }

		_SpotLightMaterialVertex(XMFLOAT4 position, XMFLOAT2 textureCoordinates, XMFLOAT3 normal)
			: Position(position), TextureCoordinates(textureCoordinates), Normal(normal) { }
	} SpotLightMaterialVertex;

	class SpotLightMaterial : public Material
	{
		RTTI_DECLARATIONS(SpotLightMaterial, Material)

			MATERIAL_VARIABLE_DECLARATION(WVP)
			MATERIAL_VARIABLE_DECLARATION(World)
			MATERIAL_VARIABLE_DECLARATION(position)
			MATERIAL_VARIABLE_DECLARATION(range)
			MATERIAL_VARIABLE_DECLARATION(dir)
			MATERIAL_VARIABLE_DECLARATION(cone)
			MATERIAL_VARIABLE_DECLARATION(ambient)
			MATERIAL_VARIABLE_DECLARATION(diffuse)



	public:
		SpotLightMaterial();

		virtual void Initialize(Effect* effect) override;
		virtual void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void CreateVertexBuffer(ID3D11Device* device, Vertex* vertices1, SpotLightMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT VertexSize() const override;
	};
}



