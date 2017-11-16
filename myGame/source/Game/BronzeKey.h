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
	class BronzeKey : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(BronzeKey, DrawableGameComponent)

	public:
		BronzeKey(Game& game, Camera& camera);
		~BronzeKey();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		BronzeKey();
		BronzeKey(const BronzeKey& rhs);
		BronzeKey& operator=(const BronzeKey& rhs);

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
