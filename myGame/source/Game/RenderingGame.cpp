#include "RenderingGame.h"
#include <sstream>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include "GameException.h"
#include "FpsComponent.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Utility.h"
#include "ColorHelper.h"
#include "FirstPersonCamera.h"
#include "RenderStateHelper.h"
#include "Level.h"
#include "TreasureChest.h"
#include "Rock.h"
#include "Player.h"
#include "PlayerAnimation.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackGroundColor = ColorHelper::CornflowerBlue;

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand), mFpsComponent(nullptr), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
		 mRenderStateHelper(nullptr), mTreasureChest(nullptr)
	{
		mDepthStencilBufferEnabled = true;
		mMultiSamplingEnabled = true;
	}

	RenderingGame::~RenderingGame()
	{
	}

	void RenderingGame::Initialize()
	{
		if (FAILED(DirectInput8Create(mInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&mDirectInput, nullptr)))
		{
			throw GameException("DirectInput8Create() failed");
		}

		mKeyboard = new Keyboard(*this, mDirectInput);
		mComponents.push_back(mKeyboard);
		mServices.AddService(Keyboard::TypeIdClass(), mKeyboard);

		mMouse = new Mouse(*this, mDirectInput);
		mComponents.push_back(mMouse);
		mServices.AddService(Mouse::TypeIdClass(), mMouse);

		mCamera = new FirstPersonCamera(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera);

		//FpsComponent changes some render states because of SpriteBatch system, 
		//so we have to save states, draw FpsComponent, then restore states 
		mFpsComponent = new FpsComponent(*this);
		mFpsComponent->Initialize();
		//mComponents.push_back(mFpsComponent);

		mRenderStateHelper = new RenderStateHelper(*this);

		mLevel = new Level(*this, *mCamera);
		mComponents = mLevel->UpdateComponent(mComponents);

		mTreasureChest = new TreasureChest(*this, *mCamera);
		mComponents.push_back(mTreasureChest);

		mTreasureChest->SetPosition(0, 0, -50.0f, -1.57f, 0, 0, 1,1,1);

		mRock = new Rock(*this, *mCamera);
		mComponents.push_back(mRock);
		mRock->SetPosition(300, 0, -50, 0, 0, 0, 20,1,1);

		Rock* mRock2 = new Rock(*this, *mCamera);
		mComponents.push_back(mRock2);
		mRock2->SetPosition(300, 0, -300, 0, 0, 0, 20,1,1);

	    player = new Player(*this, *mCamera);
		mComponents.push_back(player);
		player->SetPosition(0,0,10.0f,0,0,0,1,1,1);

		mAnimation = new PlayerAnimation(*this, *mCamera);
		mComponents.push_back(mAnimation);
		mAnimation->SetPosition(0, 0, -10, 0, 0, 0, 1, 1, 1);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(0.0f, 15.0f, 20.0f);

		//Test message
		ScreenMessage::PushMessage("Rendering_game_Initialized");
		
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		mFpsComponent->Update(gameTime);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

		//Player movement------
		if (mKeyboard->IsKeyDown(DIK_D))
		{
			player->x += mCamera->MovementRate() * gameTime.ElapsedGameTime();
		}

		if (mKeyboard->IsKeyDown(DIK_A))
		{
			player->x -= mCamera->MovementRate() * gameTime.ElapsedGameTime();
		}

		if (mKeyboard->IsKeyDown(DIK_W))
		{
			player->z -= mCamera->MovementRate() * gameTime.ElapsedGameTime();
		}

		if (mKeyboard->IsKeyDown(DIK_S))
		{
			player->z += mCamera->MovementRate() * gameTime.ElapsedGameTime();
		}
		//----------------------------

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackGroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper->SaveAll();

		mSpriteBatch->Begin();

		mFpsComponent->Draw(mGameTime);

		std::wostringstream mouseLabel;
		mouseLabel << L"Mouse Position: " << mMouse->X() << ", " << mMouse->Y() << " Mouse Wheel: " << mMouse->Wheel();
		mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

		//Player Location
		//std::wostringstream playerLocation;
		//XMFLOAT2 messageLoc = XMFLOAT2(Game::DefaultScreenWidth - 100, 5);
		//playerLocation << "x " << player->getPosition().x << "\n" << "z " << player->getPosition().z;
		//mSpriteFont->DrawString(mSpriteBatch, playerLocation.str().c_str(), messageLoc, Colors::Red);
		
		//playerLocation << ScreenMessage::Allmessages().at(0).c_str();
		//mSpriteFont->DrawString(mSpriteBatch, playerLocation.str().c_str(), messageLoc, Colors::White);

		
		


		mSpriteBatch->End();

		//On screen message system-----------------------

		showMessages();

		//------------------------------------------------

		mRenderStateHelper->RestoreAll();

		HRESULT hr = mSwapChain->Present(0, 0);

		if (FAILED(hr))
		{
			throw GameException("IDXGISwapChain::Present() failed.", hr);
		}
	}

	void RenderingGame::Shutdown()
	{
		DeleteObject(mRenderStateHelper);
		DeleteObject(mKeyboard);
		DeleteObject(mMouse);
		DeleteObject(mFpsComponent);
		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);

		ReleaseObject(mDirectInput);

		Game::Shutdown();
	}
	
	void RenderingGame::showMessages()
	{
		std::vector<int> lines;
		lines.push_back(5); // first line location

		//init the lines location
		for (int i = 0; i < ScreenMessage::Allmessages().size(); i++)
		{
			lines.push_back(i + 30);
		}

		mSpriteBatch->Begin();

		for (int i = 0; i < ScreenMessage::Allmessages().size(); i++) // loop all messages
		{
			XMFLOAT2 messageLoc = XMFLOAT2(Game::DefaultScreenWidth - 200, lines.at(i));

			std::wostringstream empty;
			empty << ScreenMessage::Allmessages().at(i).c_str();
			mSpriteFont->DrawString(mSpriteBatch, empty.str().c_str() , messageLoc, Colors::Red);
		}

		mSpriteBatch->End();

		ScreenMessage::ClearMessages();

	}

}