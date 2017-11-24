#pragma once

#include "DrawableGameComponent.h"

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

namespace Rendering
{
	class Player : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Player, DrawableGameComponent)

	public:
		Player(Game& game, Camera& camera);
		~Player();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;
		void SetUpPosition(float X, float Y, float Z);

		float x;
		float y;
		float z;

		XMFLOAT3 getPosition();

	private:
		Player();
		Player(const Player& rhs);
		Player& operator=(const Player& rhs);

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;

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

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		Keyboard* mKeyboard;

		float mAngle;

	};
}