#pragma once

#include "Common.h"
#include "Game.h"
#include "ScreenMessage.h"

using namespace Library;

namespace DirectX
{
	class SpriteBatch;
	class SpriteFont;
}

namespace Library
{
	class FpsComponent;
	class Keyboard;
	class Mouse;
	class FirstPersonCamera;
	class FpsComponent;
	class RenderStateHelper;
	class SpotLight;
}

namespace Rendering
{
	class Level;
	class TreasureChest;
	class Rock;
	class Player;
	class PlayerAnimation;

	class RenderingGame : public Game
	{
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTile, int showCommand);
		~RenderingGame();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	protected:
		virtual void Shutdown() override;

	private:
		static const XMVECTORF32 BackGroundColor;

		LPDIRECTINPUT8 mDirectInput;
		Keyboard* mKeyboard;
		Mouse* mMouse;
		FirstPersonCamera* mCamera;
		FpsComponent* mFpsComponent;
		RenderStateHelper* mRenderStateHelper;


		Level* mLevel;
		TreasureChest* mTreasureChest;
		Rock* mRock;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;

		Player* player;
		PlayerAnimation* mAnimation;

		XMFLOAT2 mMouseTextPosition;

		void showMessages();

		float zoomY = 20.0f;
		float zoomZ = 20.0f;
		float zoomX = 0;

		float zoomingSpeed = 0;
		float camRotX = 0;

		int mCurrentMouseX;
		int mLastMouseX;
		int mRotation;

		float mAngle = 0.2;


	};
}