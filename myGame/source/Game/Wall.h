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
	class Wall : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Wall, DrawableGameComponent)

	public:
		Wall(Game& game, Camera& camera, SpotLight& spotLight);
		~Wall();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		Wall();
		Wall(const Wall& rhs);
		Wall& operator=(const Wall& rhs);

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