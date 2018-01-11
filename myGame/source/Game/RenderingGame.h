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
	class ProxyModel;
}

namespace Rendering
{
	class Level;
	class TreasureChest;
	class Rock;
	class Player;
	class PlayerAnimation;
	class Mirror;

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

		//spotlight stuff
		SpotLight* mSpotLight1; // lightsource1
		SpotLight* mSpotLight2; // mirror1
		Mirror* mMirror1;       // mirror1
		SpotLight* mSpotLight3; // lightsource2
		SpotLight* mSpotLight4; // mirror2
		Mirror* mMirror2;       // mirror2
		SpotLight* mSpotLight5; // mirror3
		Mirror* mMirror3;		// mirror3
		SpotLight* mSpotLight6; // mirror4
		Mirror* mMirror4;		// mirror4
		SpotLight* mSpotLight7; // lightsource3
		SpotLight* mSpotLight8; // mirror5
		Mirror* mMirror5;		// mirror5
		SpotLight* mSpotLight9;	// mirror6
		Mirror* mMirror6;		// mirror6
		SpotLight* mSpotLight10;// mirror7
		Mirror* mMirror7;		// mirror7
		
		TreasureChest* mChest;
		TreasureChest* mChest1;
		TreasureChest* mChest2;
		TreasureChest* mChest3;
		TreasureChest* mChest4;

		//void Interaction(const GameTime& gameTime, TreasureChest* chest);

		std::vector<SpotLight*> mSpotLights;
		static const float LightModulationRate;
		static const XMFLOAT2 LightRotationRate;
		ProxyModel* mProxyModel1;
		ProxyModel* mProxyModel2;

		Level* mLevel;
		TreasureChest* mTreasureChest;
		Rock* mRock;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;

		Player* player;
		PlayerAnimation* mAnimation;

		XMFLOAT2 mMouseTextPosition;

		void showMessages();

		//Collision
		void CollisionDetection(float playerX, float playerZ);

		bool isColliding = false;
		float distance;
		float distance2;
		float Distance(const XMVECTOR& point1, const XMVECTOR& point2);
		int objectView = 0;
		int LookingAtObject = 0;
		//-----------------------------

		float zoomY = 17.0f;
		float zoom = 17.0f;//20
		float zoomX = 0;
		float camPosZ = 17.0f;//20

		float zoomingSpeed = 0;
		float camRotX = 0;

		int mCurrentMouseX;
		int mCurrentMouseY;
		int mLastMouseX;
		int mLastMouseY;
		int mRotation;

		float mAngle = 0.2;
		bool isUp = true;
		bool isDown = false;


	};
}