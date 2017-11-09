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
	class Wall : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Wall, DrawableGameComponent)

	public:
		Wall(Game& game, Camera& camera);
		~Wall();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Wall();
		Wall(const Wall& rhs);
		Wall& operator=(const Wall& rhs);

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