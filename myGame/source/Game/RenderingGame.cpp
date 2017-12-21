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
#include "VectorHelper.h"
#include "SpotLight.h"
#include "ProxyModel.h"
#include "Mirror.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackGroundColor = ColorHelper::CornflowerBlue;
	const float RenderingGame::LightModulationRate = UCHAR_MAX;
	const XMFLOAT2 RenderingGame::LightRotationRate = XMFLOAT2(XM_PI, XM_PI);

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand), mFpsComponent(nullptr), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
		 mRenderStateHelper(nullptr), mTreasureChest(nullptr), mSpotLight1(nullptr), mSpotLight2(nullptr), mProxyModel1(nullptr), mProxyModel2(nullptr)
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

		//spotlight1 coming from lightsource 
		mSpotLight1 = new SpotLight(*this);
		mSpotLight1->SetRadius(100.0f);
		mSpotLight1->SetPosition(0.0f, 10.0f, 36.0f);
		mSpotLight1->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight1->SetInnerAngle(0.005f);
		mSpotLight1->SetOuterAngle(60.0f);

		//arrow for lightsource
		mProxyModel1 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);//PointLightProxy.obj", 1.0f);
		mProxyModel1->Initialize();
		mProxyModel1->SetPosition(0.0f, 10.0f, 36.0f);
		mProxyModel1->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mComponents.push_back(mProxyModel1);

		//set rotation for spotlight
		XMMATRIX lightRotationMatrix = XMMatrixIdentity();
		lightRotationMatrix = XMMatrixRotationY(-2.36f);

		mSpotLight1->ApplyRotation(lightRotationMatrix);
		mProxyModel1->ApplyRotation(lightRotationMatrix);

		//spotlight2 for mirror
		mSpotLight2 = new SpotLight(*this);
		mSpotLight2->SetRadius(100.0f);
		mSpotLight2->SetPosition(33.0f, 10.0f, 73.0f);
		mSpotLight2->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight2->SetInnerAngle(0.005f);
		mSpotLight2->SetOuterAngle(60.0f);
		mSpotLight2->ApplyRotation(XMMatrixRotationY(1.57f));

		//arrow for mirror
		mProxyModel2 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);//PointLightProxy.obj", 1.0f);
		mProxyModel2->Initialize();
		mProxyModel2->SetPosition(33.0f, 10.0f, 73.0f);
		mProxyModel2->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mComponents.push_back(mProxyModel2);

		mMirror1 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight1, *mSpotLight2);
		mComponents.push_back(mMirror1);
		mMirror1->SetPosition(33.0f, 10.0f, 73.0f);
		mMirror1->ApplyRotation(XMMatrixRotationY(1.57f));

		mLevel = new Level(*this, *mCamera, *mSpotLight1, *mSpotLight2, *mMirror1);
		mComponents = mLevel->UpdateComponent(mComponents);

	    player = new Player(*this, *mCamera);
		player->Initialize();
		player->SetUpPosition(13, 0, 45);
		mComponents.push_back(player);

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(player->getPosition().x, zoomY, player->getPosition().z - camPosZ);

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
			//Camera movement------
			if (!mCamera->GetIsFPS())
			{
				mCurrentMouseX = mMouse->X();
				if (mCurrentMouseX > mLastMouseX)
				{
					mCamera->Rotate(-0.0175f);
				}
				else if (mCurrentMouseX < mLastMouseX)
				{
					mCamera->Rotate(0.0175f);
				}
				mLastMouseX = mCurrentMouseX;

				//--------Y Rotation ----
				mCurrentMouseY = mMouse->Y();

				if (mCurrentMouseY > mLastMouseY)
				{
					//if (isDown)
					//{
						mCamera->RotateY(-0.0015f);
						//isDown = false;
						//isUp = true;
					//}
					
				}
				else if (mCurrentMouseY < mLastMouseY)
				{
					//if (isUp)
					//{
						mCamera->RotateY(0.0015f);
						//isDown = true;
						//isUp = false;
					//}
					
				}
				mLastMouseY = mCurrentMouseY;
				
				

				//---------------------

				zoomX = zoom * player->GetLocalForward().x;
				camPosZ = -zoom * player->GetLocalForward().y;
			}
			else
			{
				mCurrentMouseX = mMouse->X();
				if (mCurrentMouseX > mLastMouseX)
				{
					mCamera->Rotate(-0.0175f);
				}
				else if (mCurrentMouseX < mLastMouseX)
				{
					mCamera->Rotate(0.0175f);
				}
				mLastMouseX = mCurrentMouseX;

				zoomX = 2 * player->GetLocalForward().x;
				camPosZ = 2 * player->GetLocalForward().y;
			}
			
			//----------------------------

			//Update the camera position
			mCamera->SetPosition(player->getPosition().x + zoomX , zoomY, player->getPosition().z - camPosZ);
			
			// Camera Zoom In / Out --------------------------------------------------------------------	
			if (!mCamera->GetIsFPS())
			{
				zoomingSpeed = (float)mMouse->Wheel() / 1000;
				mMouse->WheelToZero();

				//Zoom IN
				if ((float)mMouse->Wheel() / 1000 > 0 && mCamera->Position().y > 13 && mCamera->Position().z < player->getPosition().z - 9)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoom -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
					camPosZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
				}

				//Zoom Out
				if ((float)mMouse->Wheel() / 1000 < 0 && mCamera->Position().y < 17 && mCamera->Position().z > player->getPosition().z - 17)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoom -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
					camPosZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
				}


			}

			//------------------------------------------------------------------------------------------			
			
			//Switch to First person Camera
			if (mKeyboard->WasKeyPressedThisFrame(DIK_C))
			{
				switch (mCamera->GetIsFPS())
				{
				case true:
					// 3d Person CAMERA---
					zoomY = 17.0f;
					camPosZ = 17.0f;
					//--------------------

					mCamera->set_FPS_OFF();
					player->setFPS_OFF();

					break;

				case false:
					//FIRST PERSON CAMERA-----
					zoomY = 7;
					camPosZ = 2;
					//--------------------------

					mCamera->set_FPS_ON();
					player->setFPS_ON();

					break;
				}
			}
			
			//Player movement------
			if (mKeyboard->IsKeyDown(DIK_W))
			{
				player->x -= player->GetLocalForward().x * gameTime.ElapsedGameTime() * 10;
				player->z -= player->GetLocalForward().y * gameTime.ElapsedGameTime() * 10;
			}

			if (mKeyboard->IsKeyDown(DIK_S))
			{
				player->x += player->GetLocalForward().x * gameTime.ElapsedGameTime() * 10;
				player->z += player->GetLocalForward().y * gameTime.ElapsedGameTime() * 10;
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
		mouseLabel << L"Mouse Position: " << mMouse->X() << ", " << mCurrentMouseY << " Mouse Wheel: " << mMouse->Wheel();
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

		//player forward
		std::wostringstream playerForward;
		XMFLOAT2 forwardLoc = XMFLOAT2(5, 110);
		playerForward << "Player forward x: " << player->GetLocalForward().x << ", z: " << player->GetLocalForward().y;
		mSpriteFont->DrawString(mSpriteBatch, playerForward.str().c_str(), forwardLoc);
		

		std::wostringstream CameraRot;
		XMFLOAT2 RotLoc = XMFLOAT2(5, 150);
		CameraRot << "Rotation " << mRotation;
		mSpriteFont->DrawString(mSpriteBatch, CameraRot.str().c_str(), RotLoc);


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
		DeleteObject(mSpotLight1);
		DeleteObject(mSpotLight2);
		DeleteObject(mProxyModel1);
		DeleteObject(mProxyModel2);
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