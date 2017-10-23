#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class TextureMaterial;
}

namespace Rendering
{
	class TreasureChest : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(TreasureChest, DrawableGameComponent)

	public:
		TreasureChest(Game& game, Camera& camera);
		~TreasureChest();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		TreasureChest();
		TreasureChest(const TreasureChest& rhs);
		TreasureChest& operator=(const TreasureChest& rhs);

		Effect* mTextureEffect;
		TextureMaterial* mTextureMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}