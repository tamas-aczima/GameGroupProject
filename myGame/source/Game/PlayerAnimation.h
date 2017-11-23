#pragma once

#include "DrawableGameComponent.h"
#include "RenderStateHelper.h"

using namespace Library;

namespace Library
{
	class Keyboard;
	class Effect;
	class SkinnedModelMaterial;
	class Model;
	class AnimationPlayer;
	class AnimationClip;
}

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Rendering
{
	class PlayerAnimation : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(PlayerAnimation, DrawableGameComponent)

	public:
		PlayerAnimation(Game& game, Camera& camera);
		~PlayerAnimation();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		PlayerAnimation();
		PlayerAnimation(const PlayerAnimation& rhs);
		PlayerAnimation& operator=(const PlayerAnimation& rhs);

		void UpdateOptions();
		void UpdateAmbientLight(const GameTime& gameTime);
		void UpdateSpecularLight(const GameTime& gameTime);

		static const float LightModulationRate;
		static const float LightMovementRate;

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;

		Keyboard* mKeyboard;
		XMCOLOR mAmbientColor;
		XMCOLOR mSpecularColor;
		float mSpecularPower;
		XMFLOAT4X4 mWorldMatrix;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<ID3D11ShaderResourceView*> mColorTextures;

		Model* mSkinnedModel;
		Model* mWalkAnimation;
		Model* mJumpAnimation;
		AnimationPlayer* mAnimationPlayer;
		AnimationPlayer* mIdlePlayer;
		AnimationPlayer* mWalkPlayer;
		AnimationPlayer* mJumpPlayer;

		XMFLOAT2 mTextPosition;
		bool mManualAdvanceMode;
	};
}