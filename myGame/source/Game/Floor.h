#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class SpotLight;
}

namespace Rendering
{
	class DiffuseLightingMaterial;
	class Floor : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Floor, DrawableGameComponent)

	public:
		Floor(Game& game, Camera& camera, SpotLight& spotLight);
		~Floor();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Floor();
		Floor(const Floor& rhs);
		Floor& operator=(const Floor& rhs);

		Effect* mEffect;
		DiffuseLightingMaterial* mMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		SpotLight* mSpotLight;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}