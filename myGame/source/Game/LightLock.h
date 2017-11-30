#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class TextureMaterial;
	class SpotLight;
}

namespace Rendering
{
	class LightLock : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(LightLock, DrawableGameComponent)

	public:
		LightLock(Game& game, Camera& camera);
		~LightLock();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		LightLock();
		LightLock(const LightLock& rhs);
		LightLock& operator=(const LightLock& rhs);

		Effect* mTextureEffect;
		TextureMaterial* mTextureMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		SpotLight* mSpotLight;
	};
}


