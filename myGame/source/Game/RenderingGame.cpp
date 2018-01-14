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
#include "Wall.h"
#include "Camera.h"
#include "GoldKey.h"
#include "DiffuseLightingMaterial.h"
#include "Game.h"
#include "TreasureChest.h"
#include "BasicDoor.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackGroundColor = ColorHelper::Black;
	const float RenderingGame::LightModulationRate = UCHAR_MAX;
	const XMFLOAT2 RenderingGame::LightRotationRate = XMFLOAT2(XM_PI, XM_PI);

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand), mFpsComponent(nullptr), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
		 mRenderStateHelper(nullptr), mSpotLight1(nullptr), mSpotLight2(nullptr), mSpotLight3(nullptr), mSpotLight4(nullptr), 
		mSpotLight5(nullptr), mSpotLight6(nullptr), mSpotLight7(nullptr), mSpotLight8(nullptr), mSpotLight9(nullptr), 
		mSpotLight10(nullptr), mMirror1(nullptr), mMirror2(nullptr), mMirror3(nullptr), mMirror4(nullptr), 
		mMirror5(nullptr), mMirror6(nullptr), mMirror7(nullptr), mProxyModel1(nullptr), mProxyModel2(nullptr), mProxyModel3(nullptr),
		mChest(nullptr), mChest1(nullptr), mChest2(nullptr), mChest3(nullptr) ,mChest4(nullptr), key1(nullptr), key2(nullptr), key3(nullptr), key4(nullptr),
		door1(nullptr), door2(nullptr), mLevel(nullptr)
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

		mGameLogoImage = (HBITMAP)LoadImageW(NULL, L"Content\\Textures\\Gold.bmp", IMAGE_BITMAP, 256, 256, LR_LOADFROMFILE);

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
		//mComponents.push_back(mFpsComponent);

		mRenderStateHelper = new RenderStateHelper(*this);

		//spotlight1 coming from lightsource1 
		mSpotLight1 = new SpotLight(*this);
		mSpotLight1->SetRadius(100.0f);
		mSpotLight1->SetPosition(0.0f, 10.0f, 36.0f);
		mSpotLight1->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight1->SetInnerAngle(0.005f);
		mSpotLight1->SetOuterAngle(60.0f);
		mSpotLight1->ApplyRotation(XMMatrixRotationY(-2.36f));
		mSpotLights.push_back(mSpotLight1);

		//arrow for lightsource1
		mProxyModel1 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);
		mProxyModel1->Initialize();
		mProxyModel1->SetPosition(0.0f, 10.0f, 36.0f);
		mProxyModel1->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mProxyModel1->ApplyRotation(XMMatrixRotationY(-2.36f));
		mComponents.push_back(mProxyModel1);	

		//spotlight2 for mirror1
		mSpotLight2 = new SpotLight(*this);
		mSpotLight2->SetRadius(100.0f);
		mSpotLight2->SetPosition(33.0f, 10.0f, 73.0f);
		mSpotLight2->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight2->SetInnerAngle(0.005f);
		mSpotLight2->SetOuterAngle(60.0f);
		mSpotLight2->ApplyRotation(XMMatrixRotationY(0.78f));
		mSpotLights.push_back(mSpotLight2);

		mMirror1 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight1, *mSpotLight2, 1);
		mComponents.push_back(mMirror1);
		mMirror1->SetPosition(33.0f, 10.0f, 73.0f);
		mMirror1->ApplyRotation(XMMatrixRotationY(0.78f));

		//spotlight3 coming from lightsource2
		mSpotLight3 = new SpotLight(*this);
		mSpotLight3->SetRadius(100.0f);
		mSpotLight3->SetPosition(-225.0f, 12.0f, 10.0f);
		mSpotLight3->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight3->SetInnerAngle(0.005f);
		mSpotLight3->SetOuterAngle(60.0f);
		mSpotLight3->ApplyRotation(XMMatrixRotationY(-2.36f));
		mSpotLights.push_back(mSpotLight3);

		//arrow for lightsource2
		mProxyModel2 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);
		mProxyModel2->Initialize();
		mProxyModel2->SetPosition(-225.0f, 12.0f, 10.0f);
		mProxyModel2->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mProxyModel2->ApplyRotation(XMMatrixRotationY(-2.36f));
		mComponents.push_back(mProxyModel2);

		//spotlight4 for mirror2
		mSpotLight4 = new SpotLight(*this);
		mSpotLight4->SetRadius(100.0f);
		mSpotLight4->SetPosition(-190.0f, 12.0f, 40.0f);
		mSpotLight4->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight4->SetInnerAngle(0.005f);
		mSpotLight4->SetOuterAngle(60.0f);
		mSpotLight4->ApplyRotation(XMMatrixRotationY(1.57f));
		mSpotLights.push_back(mSpotLight4);

		mMirror2 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight3, *mSpotLight4, 2);
		mComponents.push_back(mMirror2);
		mMirror2->SetPosition(-190.0f, 12.0f, 40.0f);
		mMirror2->ApplyRotation(XMMatrixRotationY(1.57f));

		//spotlight5 for mirror3
		mSpotLight5 = new SpotLight(*this);
		mSpotLight5->SetRadius(100.0f);
		mSpotLight5->SetPosition(-234.0f, 12.0f, 80.0f);
		mSpotLight5->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight5->SetInnerAngle(0.005f);
		mSpotLight5->SetOuterAngle(60.0f);
		mSpotLight5->ApplyRotation(XMMatrixRotationY(-1.57f));
		mSpotLights.push_back(mSpotLight5);

		mMirror3 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight4, *mSpotLight5, 3);
		mComponents.push_back(mMirror3);
		mMirror3->SetPosition(-234.0f, 12.0f, 80.0f);
		mMirror3->ApplyRotation(XMMatrixRotationY(-1.57f));

		//spotlight6 for mirror4
		mSpotLight6 = new SpotLight(*this);
		mSpotLight6->SetRadius(100.0f);
		mSpotLight6->SetPosition(-183.0f, 12.0f, 115.0f);
		mSpotLight6->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight6->SetInnerAngle(0.005f);
		mSpotLight6->SetOuterAngle(60.0f);
		mSpotLight6->ApplyRotation(XMMatrixRotationY(1.57f));
		mSpotLights.push_back(mSpotLight6);

		mMirror4 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight5, *mSpotLight6, 4);
		mComponents.push_back(mMirror4);
		mMirror4->SetPosition(-183.0f, 12.0f, 115.0f);
		mMirror4->ApplyRotation(XMMatrixRotationY(1.57f));

		//spotlight7 for lightsource3
		mSpotLight7 = new SpotLight(*this);
		mSpotLight7->SetRadius(200.0f);
		mSpotLight7->SetPosition(-495.0f, 12.0f, 120.0f);
		mSpotLight7->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight7->SetInnerAngle(0.005f);
		mSpotLight7->SetOuterAngle(160.0f);
		mSpotLight7->ApplyRotation(XMMatrixRotationY(-0.78f));
		mSpotLights.push_back(mSpotLight7);

		//arrow for lightsource3
		mProxyModel3 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);
		mProxyModel3->Initialize();
		mProxyModel3->SetPosition(-495.0f, 12.0f, 120.0f);
		mProxyModel3->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mProxyModel3->ApplyRotation(XMMatrixRotationY(-0.78f));
		mComponents.push_back(mProxyModel3);

		//spotlight8 for mirror5
		mSpotLight8 = new SpotLight(*this);
		mSpotLight8->SetRadius(200.0f);
		mSpotLight8->SetPosition(-400.0f, 12.0f, 15.0f);
		mSpotLight8->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight8->SetInnerAngle(0.005f);
		mSpotLight8->SetOuterAngle(60.0f);
		mSpotLight8->ApplyRotation(XMMatrixRotationY(3.14f));
		mSpotLights.push_back(mSpotLight8);

		mMirror5 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight7, *mSpotLight8, 5);
		mComponents.push_back(mMirror5);
		mMirror5->SetPosition(-400.0f, 12.0f, 15.0f);
		mMirror5->ApplyRotation(XMMatrixRotationY(3.14f));

		//spotlight9 for mirror6
		mSpotLight9 = new SpotLight(*this);
		mSpotLight9->SetRadius(200.0f);
		mSpotLight9->SetPosition(-425.0f, 12.0f, 116.5f);
		mSpotLight9->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight9->SetInnerAngle(0.005f);
		mSpotLight9->SetOuterAngle(60.0f);
		//mSpotLight9->ApplyRotation(XMMatrixRotationY(1.57f));
		mSpotLights.push_back(mSpotLight8);

		mMirror6 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight8, *mSpotLight9, 6);
		mComponents.push_back(mMirror6);
		mMirror6->SetPosition(-425.0f, 12.0f, 116.5f);
		//mMirror6->ApplyRotation(XMMatrixRotationY(1.57f));

		//spotlight10 for mirror7
		mSpotLight10 = new SpotLight(*this);
		mSpotLight10->SetRadius(200.0f);
		mSpotLight10->SetPosition(-505.0f, 12.0f, 15.0f);
		mSpotLight10->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight10->SetInnerAngle(0.005f);
		mSpotLight10->SetOuterAngle(60.0f);
		mSpotLight10->ApplyRotation(XMMatrixRotationY(3.14f));
		mSpotLights.push_back(mSpotLight10);

		mMirror7 = new Mirror(*this, *mCamera, *mMouse, *mSpotLight9, *mSpotLight10, 7);
		mComponents.push_back(mMirror7);
		mMirror7->SetPosition(-505.0f, 12.0f, 15.0f);
		mMirror7->ApplyRotation(XMMatrixRotationY(3.14f));

		mLevel = new Level(*this, *mCamera, *mSpotLight1, *mSpotLight2, *mSpotLight3, 
			*mSpotLight4, *mSpotLight5, *mSpotLight6, *mSpotLight7, *mSpotLight8, 
			*mSpotLight9, *mSpotLight10, *mMirror1, *mMirror2, *mMirror3, *mMirror4, 
			*mMirror5, *mMirror6, *mMirror7);
		mComponents = mLevel->UpdateComponent(mComponents);

	    player = new Player(*this, *mCamera, mSpotLights);
		player->Initialize();
		player->SetUpPosition(13, 0, 45);
		mComponents.push_back(player);


		mChest = new TreasureChest(*this, *mCamera, *mMouse, 100);
		mComponents.push_back(mChest);
		mChest->SetPosition(12, 0, 5, 0, 3.14, 0, 3.0, 3.0, 3.0);


		key1 = new GoldKey(*this, *mCamera, *mMouse);
		mComponents.push_back(key1);
		key1->SetPosition(12.0, 0, 50.0, 0, 1.57, 0, 0.05, 0.05, 0.05);


		mChest1 = new TreasureChest(*this, *mCamera, *mMouse, 1000); //Chest2
		mComponents.push_back(mChest1);
		mChest1->SetPosition(-100, 0, 260, 0, 0, 0, 3.0, 3.0, 3.0);

		key2 = new GoldKey(*this, *mCamera, *mMouse);
		mComponents.push_back(key2);
		key2->SetPosition(-90.5, 0, 210.5, 0, 1.57, 0, 0.05, 0.05, 0.05);

		key3 = new GoldKey(*this, *mCamera, *mMouse);
		mComponents.push_back(key3);
		key3->SetPosition(-50.5, 0, 25, 0, 1.57, 0, 0.05, 0.05, 0.05);

		key4 = new GoldKey(*this, *mCamera, *mMouse);
		mComponents.push_back(key4);
		key4->SetPosition(-160, 0, 15, 0, 1.57, 0, 0.05, 0.05, 0.05);

		mChest2 = new TreasureChest(*this, *mCamera, *mMouse, 100);//Chest3
		mComponents.push_back(mChest2);
		mChest2->SetPosition(-125, 0, 65, 0, 0, 0, 3.0, 3.0, 3.0);

		mChest3 = new TreasureChest(*this, *mCamera, *mMouse, 200);//Chest4
		mComponents.push_back(mChest3);
		mChest3->SetPosition(-265, 0, 10, 0, 3.14, 0, 3.0, 3.0, 3.0);

		mChest4 = new TreasureChest(*this, *mCamera, *mMouse, 200);//Chest5
		mComponents.push_back(mChest4);
		mChest4->SetPosition(-265, 0, 100, 0, 0 , 0, 3.0, 3.0, 3.0);

		//right basic door that required Key
		door1 = new BasicDoor(*this, *mCamera, *mMouse);
		mComponents.push_back(door1);
		door1->SetPosition(-275, 0, 40, 0, 1.57, 0, 4, 3.2, 4);

		//left basic door that required Key
		door2 = new BasicDoor(*this, *mCamera, *mMouse);
		mComponents.push_back(door2);
		door2->SetPosition(-275, 0, 80, 0, 1.57, 0, 4, 3.2, 4);



		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());


		mFpsComponent = new FpsComponent(*this);
		mFpsComponent->Initialize();

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(player->getPosition().x, zoomY, player->getPosition().z - camPosZ);

		//Test message
		ScreenMessage::PushMessage("Rendering_game_Initialized");
		
	}


#pragma region UpdateAllDoors
	void RenderingGame::UpdateAllDoors(const GameTime &gameTime)
	{
		if (door1->Visible())
		{
			if (mKeyNo > 0)
			{
				door1->Interaction(gameTime);
			}
		}
		else
		{
			if (!door1->collected())
			{
				mKeyNo--;

				door1->chestCollected();
			}
		}

		if (door2->Visible())
		{
			if (mKeyNo > 0)
			{
				door2->Interaction(gameTime);
			}
		}
		else
		{
			if (!door2->collected())
			{
				mKeyNo--;

				door2->chestCollected();
			}
		}

	}
#pragma endregion


#pragma region UpdateAllChests
	void RenderingGame::UpdateAllChests(const GameTime &gameTime)
	{
			if (mChest->Visible())
			{		
					mChest->Interaction(gameTime);
			}
			else
			{
				if (!mChest->collected())
				{
					mGold += mChest->GoldValue();

					mChest->chestCollected();
				}
			}

			if (mChest1->Visible())
			{
				if (mKeyNo > 0)
				{
					mChest1->Interaction(gameTime);
				}
			}
			else
			{
				if (!mChest1->collected())
				{
					mGold += mChest1->GoldValue();

					mKeyNo--;

					mChest1->chestCollected();
				}
			}
		
			if (mChest2->Visible())
			{
				mChest2->Interaction(gameTime);
			}
			else
			{
				if (!mChest2->collected())
				{
					mGold += mChest2->GoldValue();

					mChest2->chestCollected();
				}
			}

			if (mChest3->Visible())
			{
				mChest3->Interaction(gameTime);
			}
			else
			{
				if (!mChest3->collected())
				{
					mGold += mChest3->GoldValue();

					mChest3->chestCollected();
				}
			}

			if (mChest4->Visible())
			{
				mChest4->Interaction(gameTime);
			}
			else
			{
				if (!mChest4->collected())
				{
					mGold += mChest4->GoldValue();

					mChest4->chestCollected();
				}
			}
	}
#pragma endregion


#pragma region UpdateAllKeys
	void RenderingGame::UpdateAllKeys(const GameTime &gameTime)
	{
		if (key1->Visible())
		{
			key1->Interaction(gameTime);
		}
		else
		{
			if (!key1->collected())
			{
				mKeyNo++;

				key1->chestCollected();
			}
		}

		if (key2->Visible())
		{
			key2->Interaction(gameTime);
		}
		else
		{
			if (!key2->collected())
			{
				mKeyNo++;

				key2->chestCollected();
			}
		}

		if (key3->Visible())
		{
			key3->Interaction(gameTime);
		}
		else
		{
			if (!key3->collected())
			{
				mKeyNo++;

				key3->chestCollected();
			}
		}

		if (key4->Visible())
		{
			key4->Interaction(gameTime);
		}
		else
		{
			if (!key4->collected())
			{
				mKeyNo++;

				key4->chestCollected();
			}
		}


	}
#pragma endregion


	void RenderingGame::Update(const GameTime &gameTime)
	{

		mFpsComponent->Update(gameTime);

		UpdateAllDoors(gameTime);

		UpdateAllChests(gameTime);

		UpdateAllKeys(gameTime);

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
			if (mKeyboard->IsKeyDown(DIK_W) && !isColliding)
			{
				player->x -= player->GetLocalForward().x * gameTime.ElapsedGameTime() * 10;
				player->z -= player->GetLocalForward().y * gameTime.ElapsedGameTime() * 10;
			}

			if (mKeyboard->IsKeyDown(DIK_S))
			{
				player->x += player->GetLocalForward().x * gameTime.ElapsedGameTime() * 10;
				player->z += player->GetLocalForward().y * gameTime.ElapsedGameTime() * 10;
			}			
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

		CollisionDetection(player->x, player->z);

		Game::Update(gameTime);
	}


	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackGroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper->SaveAll();

		mSpriteBatch->Begin();

		//mFpsComponent->Draw(mGameTime);
	
		// ------ Debugging -----------

		//SendMessageW(mGameLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)mGameLogoImage);


		//std::wostringstream mouseLabel;
		//mouseLabel << L"Mouse Position: " << mMouse->X() << ", " << mCurrentMouseY << " Mouse Wheel: " << mMouse->Wheel();
		//mSpriteFont->DrawString(mSpriteBatch, mouseLabel.str().c_str(), mMouseTextPosition);

		////Player Location
		//std::wostringstream playerLocation;
		//XMFLOAT2 messageLoc = XMFLOAT2(5, 30);
		////playerLocation << "x " << player->getPosition().x << "    " << "z " << player->getPosition().z;
		//playerLocation << distance << " distance 2 " << distance2;
		//mSpriteFont->DrawString(mSpriteBatch, playerLocation.str().c_str(), messageLoc, Colors::White);
		//
		//std::wostringstream CameraPositon;
		//XMFLOAT2 cameraPosLabelLoc = XMFLOAT2(5, 90);
		//CameraPositon << L"Camera Position: Y: " << mCamera->Position().y << ", " << " Z: " << mCamera->Position().z;
		//mSpriteFont->DrawString(mSpriteBatch, CameraPositon.str().c_str(), cameraPosLabelLoc);

		////player forward
		//std::wostringstream playerForward;
		//XMFLOAT2 forwardLoc = XMFLOAT2(5, 110);
		//playerForward << "Player forward x: " << player->GetLocalForward().x << ", z: " << player->GetLocalForward().y;
		//mSpriteFont->DrawString(mSpriteBatch, playerForward.str().c_str(), forwardLoc);
		//

		//std::wostringstream CameraRot;
		//XMFLOAT2 RotLoc = XMFLOAT2(5, 150);
		//CameraRot << "Rotation " << mRotation;
		//mSpriteFont->DrawString(mSpriteBatch, CameraRot.str().c_str(), RotLoc);

		////Bounding box test
		//if (isColliding)
		//{
		//	std::wostringstream coll;
		//	XMFLOAT2 collLoc = XMFLOAT2(5, 180);
		//	coll << "Collision";
		//	mSpriteFont->DrawString(mSpriteBatch, coll.str().c_str(), collLoc);
		//}
		std::wostringstream goldLabel;
		goldLabel << L"Gold: " << mGold << "\n";
		mSpriteFont->DrawString(mSpriteBatch, goldLabel.str().c_str(), XMFLOAT2(Game::DefaultScreenWidth - 200, 50.0f), Colors::Gray);

		std::wostringstream keyLabel;
		keyLabel << L"Keys: " << mKeyNo << "\n";
		mSpriteFont->DrawString(mSpriteBatch, keyLabel.str().c_str(), XMFLOAT2(Game::DefaultScreenWidth/2, 50.0f), Colors::Gray);

		mSpriteBatch->End();

		////On screen message system-----------------------

		////showMessages();

		////------------------------------------------------

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
		DeleteObject(mChest);
		DeleteObject(key1);
		DeleteObject(key2);
		DeleteObject(key3);
		DeleteObject(key4);
		DeleteObject(mChest1);
		DeleteObject(mChest2);
		DeleteObject(mChest3);
		DeleteObject(mChest4);
		DeleteObject(door1);
		DeleteObject(door2);
		DeleteObject(mProxyModel1);
		DeleteObject(mProxyModel2);
		DeleteObject(mProxyModel3);
		DeleteObject(mSpotLight1);
		DeleteObject(mSpotLight2);
		DeleteObject(mSpotLight3);
		DeleteObject(mSpotLight4);
		DeleteObject(mSpotLight5);
		DeleteObject(mSpotLight6);
		DeleteObject(mSpotLight7);
		DeleteObject(mSpotLight8);
		DeleteObject(mSpotLight9);
		DeleteObject(mSpotLight10);
		DeleteObject(mMirror1);
		DeleteObject(mMirror2);
		DeleteObject(mMirror3);
		DeleteObject(mMirror4);
		DeleteObject(mMirror5);
		DeleteObject(mMirror6);
		DeleteObject(mMirror7);
		DeleteObject(mLevel);
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

	float RenderingGame::Distance(const XMVECTOR& point1, const XMVECTOR& point2)
	{
		float distance = sqrt((XMVectorGetX(point1) - XMVectorGetX(point2)) * (XMVectorGetX(point1) - XMVectorGetX(point2)) +
			(XMVectorGetY(point1) - XMVectorGetY(point2)) * (XMVectorGetY(point1) - XMVectorGetY(point2)) +
			(XMVectorGetZ(point1) - XMVectorGetZ(point2)) * (XMVectorGetZ(point1) - XMVectorGetZ(point2)));
		return distance;
	}

	void RenderingGame::CollisionDetection(float playerX, float playerZ)
	{
		//loop all objects in the map
		/*if (mLevel->wallsList.at(objectView)->Visible())
		{
		gameObjects.push_back(mLevel->wallsList.at(objectView));
		}*/



		XMFLOAT4X4 P;
		XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());

		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(playerX, 1.0f, playerZ, 0.0f);

		// Tranform ray to local space of Mesh via the inverse of both of view and world transform
		XMMATRIX V = mCamera->ViewMatrix();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


		XMMATRIX W = XMLoadFloat4x4(mLevel->wallsList.at(objectView)->WorldMatrix());
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);


		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);

		float tmin = 0.0;
		//Check if visible
		/*if (mLevel->wallsList.at(objectView)->mTextureMaterial->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{
		LookingAtObject = objectView;
		}*/
		for (int i = 0; i < mLevel->wallsList.size(); i++)
		{
			distance = Distance(player->positionVector, mLevel->wallsList.at(i)->positionVector);
			if (distance < 7)
			{
				LookingAtObject = i;
			}

			distance2 = Distance(player->positionVector, mLevel->wallsList.at(LookingAtObject)->positionVector);
			if (distance2 < 6.5)//6.5
			{
				isColliding = true;
			}
			else
			{
				isColliding = false;
			}
		}
		//check distance
		//distance = Distance(player->positionVector, mLevel->wallsList.at(LookingAtObject)->positionVector);

		/*if (distance < 6.5)
		isColliding = true;
		else
		isColliding = false;*/

		//Reset the loop
		/*objectView++;
		if (objectView > mLevel->wallsList.size() - 1)
		objectView = 0;*/

	}

}