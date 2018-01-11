#pragma once

#include "DrawableGameComponent.h"
#include <DirectXCollision.h>

using namespace Library;

namespace Library
{
	class Effect;
	class Mouse;
	class Mesh;
}

namespace Rendering
{
	class DiffuseLightingMaterial;

	class GoldKey : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GoldKey, DrawableGameComponent)

	public:
		GoldKey(Game& game, Camera& camera, Mouse& mouse);
		~GoldKey();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		DirectX::BoundingBox mBoundingBox;

		XMFLOAT4X4* WorldMatrix() { return &mWorldMatrix; }

	private:
		GoldKey();
		GoldKey(const GoldKey& rhs);
		GoldKey& operator=(const GoldKey& rhs);

		Effect* mTextureEffect;
		DiffuseLightingMaterial* mMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		Mouse* mMouse;
		float mAngle;

		void Interaction(const GameTime& gameTime);

		std::wstring mTextureName;

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}
