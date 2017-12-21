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
	class Lava : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Lava, DrawableGameComponent)

	public:
		Lava(Game& game, Camera& camera);
		~Lava();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Lava();
		Lava(const Lava& rhs);
		Lava& operator=(const Lava& rhs);

		Effect* mTextureEffect;
		TextureMaterial* mTextureMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}