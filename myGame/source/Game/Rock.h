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

	class Rock : public DrawableGameComponent
	{

		RTTI_DECLARATIONS(Rock,DrawableGameComponent)

	public:
		Rock(Game& game, Camera& camera);
		~Rock();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Rock();
		Rock(const Rock& rhs);
		Rock& operator = (const Rock& rhs);

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



