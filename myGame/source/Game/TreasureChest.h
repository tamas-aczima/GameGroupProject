#pragma once

#include "DrawableGameComponent.h"
#include <DirectXCollision.h>

using namespace Library;

namespace Library
{
	class Effect;
	class Mouse;
}

namespace Rendering
{
	class DiffuseLightingMaterial;

	class TreasureChest : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TreasureChest, DrawableGameComponent)

	public:
		TreasureChest(Game& game, Camera& camera, Mouse& mouse, int goldValue);
		~TreasureChest();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		DirectX::BoundingBox mBoundingBox;
		int const GoldValue() { return mGoldValue; }
		bool const collected() { return mCollected; }

		void chestCollected();

		XMFLOAT4X4* WorldMatrix() { return &mWorldMatrix; }

		void Interaction(const GameTime& gameTime);

	private:
		TreasureChest();
		TreasureChest(const TreasureChest& rhs);
		TreasureChest& operator=(const TreasureChest& rhs);

		Effect* mTextureEffect;
		DiffuseLightingMaterial* mMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		Mouse* mMouse;

		bool mShowing;
		
		int mGoldValue;
		bool mCollected;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}