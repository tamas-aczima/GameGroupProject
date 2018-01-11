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

	class BasicDoor : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(BasicDoor, DrawableGameComponent)

	public:
		BasicDoor(Game& game, Camera& camera, Mouse& mouse);
		~BasicDoor();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		DirectX::BoundingBox mBoundingBox;
		bool const collected() { return mCollected; }

		void chestCollected();

		void Interaction(const GameTime& gameTime);

		XMFLOAT4X4* WorldMatrix() { return &mWorldMatrix; }

	private:
		BasicDoor();
		BasicDoor(const BasicDoor& rhs);
		BasicDoor& operator=(const BasicDoor& rhs);

		Effect* mTextureEffect;
		DiffuseLightingMaterial* mMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		Mouse* mMouse;
		float mAngle;
		bool mCollected;

		std::wstring mTextureName;

		void CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}

