#pragma once

#include "DrawableGameComponent.h"
#include "DirectXCollision.h"

using namespace Library;
using namespace DirectX;

namespace Library
{
	class Keyboard;
	class Mouse;
	class Effect;
	class SkinnedModelMaterial;
	class Model;
	class AnimationPlayer;
	class AnimationClip;
	class TextureMaterial;
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

		XMVECTOR positionVector;

		XMFLOAT3 getPosition();

		XMFLOAT2 GetLocalForward();

		DirectX::BoundingBox boundingBox;
		bool CheckCollisions(BoundingBox box);
		bool isColliding;

	private:
		Player();
		Player(const Player& rhs);
		Player& operator=(const Player& rhs);

		Effect* mEffect;
		SkinnedModelMaterial* mMaterial;
		
		XMCOLOR mAmbientColor;

		XMFLOAT4X4 mWorldMatrix;

		XMFLOAT4X4 mRotationMatrix;

		std::vector<ID3D11Buffer*> mVertexBuffers;
		std::vector<ID3D11Buffer*> mIndexBuffers;
		std::vector<UINT> mIndexCounts;
		std::vector<ID3D11ShaderResourceView*> mColorTextures;

		Model* mSkinnedModel;
		Model* mWalkForwardAnimation;
		Model* mWalkBackAnimation;
		Model* mJumpAnimation;
		AnimationPlayer* mAnimationPlayer;
		AnimationPlayer* mIdlePlayer;
		AnimationPlayer* mWalkForwardPlayer;
		AnimationPlayer* mWalkBackPlayer;
		AnimationPlayer* mJumpPlayer;
		bool mIsWalkingForward;
		bool mIsWalkingBack;
		bool mIsJumping;
		bool mIdlePlaying;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		Keyboard* mKeyboard;
		Mouse* mMouse;

		int mCurrentMouseX;
		int mLastMouseX;
		int mRotation;

		float mAngle;
		float mAngleInRadians;
		XMVECTOR mLocalForward;

		
	};
}