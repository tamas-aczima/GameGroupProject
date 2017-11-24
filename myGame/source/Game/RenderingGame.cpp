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
		player->Initialize();
		player->SetUpPosition(0, 0, 0);
		mComponents.push_back(player);
	
		//player->SetPosition(0,0,10.0f,0,0,0,1,1,1);

		/*mAnimation = new PlayerAnimation(*this, *mCamera);
		mComponents.push_back(mAnimation);
		mAnimation->SetPosition(0, 0, -10, 0, 0, 0, 1, 1, 1);*/

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(player->getPosition().x, 20.0f, player->getPosition().z + 20.0f);

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

		if (!mCamera->getIsEditing())
		{
			//Update the camera position
			mCamera->SetPosition(player->getPosition().x, zoomY, player->getPosition().z + zoomZ);
			
			// Camera Zoom In / Out --------------------------------------------------------------------	
			if (!isFPS)
			{
				zoomingSpeed = (float)mMouse->Wheel() / 1000;
				mMouse->WheelToZero();

				//Zoom IN
				if ((float)mMouse->Wheel() / 1000 > 0 && mCamera->Position().y > 13 && mCamera->Position().z > player->getPosition().z + 9)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoomZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
				}

				//Zoom Out
				if ((float)mMouse->Wheel() / 1000 < 0 && mCamera->Position().y < 20 && mCamera->Position().z < player->getPosition().z + 20)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoomZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
				}
			}

			//------------------------------------------------------------------------------------------			
			
			//Switch to First person Camera
			if (mKeyboard->WasKeyPressedThisFrame(DIK_C))
			{
				switch (isFPS)
				{
				case true:
					// 3d Person CAMERA---
					zoomY = 20.0f;
					zoomZ = 20;
					//--------------------

					mCamera->set_FPS_OFF();
					isFPS = false;

					break;

				case false:
					//FIRST PERSON CAMERA-----
					zoomY = 5;
					zoomZ = -2;
					//--------------------------

					mCamera->set_FPS_ON();
					isFPS = true;

					break;
				}
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
		}
	

		//Enable/Disabel Editing Mode
		if (mKeyboard->WasKeyPressedThisFrame(DIK_O))
		{
			switch (mCamera->getIsEditing())
			{
			case true:
				mCamera->set_IsEditing_OFF();
				break;

			case false:
				mCamera->set_IsEditing_ON();
				break;
			}
		}

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
		std::wostringstream playerLocation;
		XMFLOAT2 messageLoc = XMFLOAT2(5, 30);
		playerLocation << "x " << player->getPosition().x << "    " << "z " << player->getPosition().z;
		mSpriteFont->DrawString(mSpriteBatch, playerLocation.str().c_str(), messageLoc, Colors::White);
		
		std::wostringstream CameraPositon;
		XMFLOAT2 cameraPosLabelLoc = XMFLOAT2(5, 90);
		CameraPositon << L"Camera Position: Y: " << mCamera->Position().y << ", " << " Z: " << mCamera->Position().z;
		mSpriteFont->DrawString(mSpriteBatch, CameraPositon.str().c_str(), cameraPosLabelLoc);
	


		mSpriteBatch->End();

		//On screen message system-----------------------

		//showMessages();

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
		if (ScreenMessage::Allmessages().size() > 0)
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
				mSpriteFont->DrawString(mSpriteBatch, empty.str().c_str(), messageLoc, Colors::Red);
			}

			mSpriteBatch->End();
			ScreenMessage::ClearMessages();
		}
		

	}

}