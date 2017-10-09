#pragma once

#include "Common.h"
#include "Game.h"

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
}

namespace Rendering
{
	class TriangleDemo;
	class CubeDemo;
	class ModelDemo;

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

		TriangleDemo* mDemo;
		CubeDemo* mDemo2;
		ModelDemo* mDemo3;

		SpriteBatch* mSpriteBatch;
		SpriteFont* mSpriteFont;

		XMFLOAT2 mMouseTextPosition;
	};
}