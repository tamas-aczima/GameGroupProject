#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class Keyboard;
	class RenderStateHelper;
	class SpotLight;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	class DiffuseLightingMaterial;
	class Mirror;
	class Door;

	class LightLockDiffuseLight : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(LightLockDiffuseLight, DrawableGameComponent)

	public:
		LightLockDiffuseLight(Game& game, Camera& camera, SpotLight& spotLight);
		LightLockDiffuseLight(Game& game, Camera& camera, SpotLight& spotLight, Mirror& mirror, Door& door1, Door& door2);
		~LightLockDiffuseLight();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		//add to support multiple model in the scene
		void SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ);

	private:
		LightLockDiffuseLight();
		LightLockDiffuseLight(const LightLockDiffuseLight& rhs);
		LightLockDiffuseLight& operator=(const LightLockDiffuseLight& rhs);

		//void UpdateAmbientLight(const GameTime& gameTime);

		Effect* mEffect;
		DiffuseLightingMaterial* mMaterial;
		DiffuseLightingMaterial* mMaterial2;
		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3D11ShaderResourceView* mTextureShaderResourceView2;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;
		UINT mIndexCount2;

		XMCOLOR mAmbientColor;
		SpotLight* mSpotLight;
		Mirror* mMirror;
		Door* mDoor1;
		Door* mDoor2;
		Keyboard* mKeyboard;
		XMFLOAT4X4 mWorldMatrix;
		bool mOpenDoor;

		RenderStateHelper* mRenderStateHelper;
		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;
		SpriteBatch* mSpriteUI;
		XMFLOAT2 mTextPosition;
	};
}



