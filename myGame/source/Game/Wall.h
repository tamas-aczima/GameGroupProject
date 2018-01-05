#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class SpotLight;
	class DepthMapMaterial;
	class DepthMap;
	class ShadowMappingMaterial;
}

namespace Rendering
{
	enum ShadowMappingTechnique
	{
		ShadowMappingTechniqueSimple = 0,
	};

	const std::string ShadowMappingTechniqueNames[] = { "shadow_mapping"};
	const std::string ShadowMappingDisplayNames[] = { "Shadow Mapping Simple"};
	const std::string DepthMappingTechniqueNames[] = { "create_depthmap", "create_depthmap", "create_depthmap_w_bias", };

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
		ShadowMappingMaterial* mMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		SpotLight* mSpotLight;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		Effect* mDepthMapEffect;
		DepthMapMaterial* mDepthMapMaterial;
		DepthMap* mDepthMap;
		bool mDrawDepthMap;
	};
}