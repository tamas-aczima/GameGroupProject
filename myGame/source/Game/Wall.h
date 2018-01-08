#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"

using namespace Library;

namespace Library
{
	class Effect;
	class SpotLight;
	class DepthMapMaterial;
	class DepthMap;
	class ShadowMappingMaterial;
	class Projector;
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

		void InitializeProjectedTextureScalingMatrix();

		RenderStateHelper mRenderStateHelper;

		Effect* mEffect;
		ShadowMappingMaterial* mMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		ID3D11Buffer* mPlanePositionVertexBuffer;
		ID3D11Buffer* mPlanePositionUVNormalVertexBuffer;
		ID3D11Buffer* mPlaneIndexBuffer;
		UINT mPlaneVertexCount;
		XMFLOAT4X4 mPlaneWorldMatrix;

		SpotLight* mSpotLight;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		Effect* mDepthMapEffect;
		DepthMapMaterial* mDepthMapMaterial;
		DepthMap* mDepthMap;
		bool mDrawDepthMap;
		Projector* mProjector;
		ID3D11RasterizerState* mDepthBiasState;
		static const UINT DepthMapWidth;
		static const UINT DepthMapHeight;

		XMFLOAT4X4 mProjectedTextureScalingMatrix;
	};
}