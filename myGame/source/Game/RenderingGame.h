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
	class TextureMaterial;
}

namespace Rendering
{
	class Level;
	class TreasureChest;
	class Rock;
	class Player;
	class PlayerAnimation;
	class PlayerBoundingBox;
	class Wall;

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
		PlayerBoundingBox* playerBoundingBox;
		Wall* wall;
		Wall* wall2;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;

		Player* player;
		PlayerAnimation* mAnimation;

		XMFLOAT2 mMouseTextPosition;

		void showMessages();
		void Pick(float sx, float sy, float sz, Wall* model);
		bool isColliding = false;
		float distance;
		float distance2;
		float Distance(const XMVECTOR& point1, const XMVECTOR& point2);

		void CollisionDetection(float playerX, float playerZ);
		vector<Wall*> gameObjects;
		int objectView = 0;
		int LookingAtObject = 0;

		float zoomY = 20.0f;
		float zoomZ = 20.0f;
		float zoomX = 0;

		float zoomingSpeed = 0;
		float camRotX = 0;

		int mCurrentMouseX;
		int mLastMouseX;
		int mRotation;

		float mAngle = 0.2;

		int iCompMin = 0;
		int iCompMax;
		int iComp = iCompMin;

	};
}