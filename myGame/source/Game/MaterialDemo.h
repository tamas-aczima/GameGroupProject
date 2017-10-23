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
	class MaterialDemo : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(MaterialDemo, DrawableGameComponent)

	public:
		MaterialDemo(Game& game, Camera& camera);
		~MaterialDemo();

		virtual void Initialize() override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		MaterialDemo();
		MaterialDemo(const MaterialDemo& rhs);
		MaterialDemo& operator=(const MaterialDemo& rhs);

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