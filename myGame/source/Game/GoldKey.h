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
	class GoldKey : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(GoldKey, DrawableGameComponent)

	public:
		GoldKey(Game& game, Camera& camera);
		~GoldKey();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		GoldKey();
		GoldKey(const GoldKey& rhs);
		GoldKey& operator=(const GoldKey& rhs);

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