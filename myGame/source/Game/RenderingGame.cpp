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
#include "Camera.h"
#include "DiffuseLightingMaterial.h"
#include "Game.h"

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackGroundColor = ColorHelper::CornflowerBlue;
	const float RenderingGame::LightModulationRate = UCHAR_MAX;
	const XMFLOAT2 RenderingGame::LightRotationRate = XMFLOAT2(XM_PI, XM_PI);

	RenderingGame::RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
		: Game(instance, windowClass, windowTitle, showCommand), mFpsComponent(nullptr), mDirectInput(nullptr), mKeyboard(nullptr), mMouse(nullptr),
		 mRenderStateHelper(nullptr), mTreasureChest(nullptr), mSpotLight1(nullptr), mSpotLight2(nullptr), mProxyModel1(nullptr), mProxyModel2(nullptr), mChest(nullptr), mChest1(nullptr), mChest2(nullptr), mChest3(nullptr) ,mChest4(nullptr)
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

		//spotlight1 coming from lightsource1 
		mSpotLight1 = new SpotLight(*this);
		mSpotLight1->SetRadius(100.0f);
		mSpotLight1->SetPosition(0.0f, 10.0f, 36.0f);
		mSpotLight1->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight1->SetInnerAngle(0.005f);
		mSpotLight1->SetOuterAngle(60.0f);
		mSpotLights.push_back(mSpotLight1);

		//arrow for lightsource1
		mProxyModel1 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);//PointLightProxy.obj", 1.0f);
		mProxyModel1->Initialize();
		mProxyModel1->SetPosition(0.0f, 10.0f, 36.0f);
		mProxyModel1->ApplyRotation(XMMatrixRotationY(XM_PIDIV2));
		mComponents.push_back(mProxyModel1);

		mSpotLight1->ApplyRotation(XMMatrixRotationY(-2.36f));
		mProxyModel1->ApplyRotation(XMMatrixRotationY(-2.36f));

		//spotlight2 for mirror1
		mSpotLight2 = new SpotLight(*this);
		mSpotLight2->SetRadius(100.0f);
		mSpotLight2->SetPosition(33.0f, 10.0f, 73.0f);
		mSpotLight2->SetColor(0.0f, 0.0f, 1.0f, 1.0f);
		mSpotLight2->SetInnerAngle(0.005f);
		mSpotLight2->SetOuterAngle(60.0f);
		mSpotLight2->ApplyRotation(XMMatrixRotationY(0.78f));
		mSpotLights.push_back(mSpotLight2);

		//arrow for mirror1
		mProxyModel2 = new ProxyModel(*this, *mCamera, "Content\\Models\\DirectionalLightProxy.obj", 1.0f);//PointLightProxy.obj", 1.0f);
		mProxyModel2->Initialize();
		mProxyModel2->SetPosition(-495.0f, 12.0f, 120.0f);
		mProxyModel2->ApplyRotation(XMMatrixRotationY(0.78f));
		mComponents.push_back(mProxyModel2);

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
			*mMirror5, *mMirror6, *mMirror7, *mTreasureChest);
		mComponents = mLevel->UpdateComponent(mComponents);

	    player = new Player(*this, *mCamera, mSpotLights);
		player->Initialize();
		player->SetUpPosition(13, 0, 45);
		mComponents.push_back(player);


		mChest = new TreasureChest(*this, *mCamera);//Chest 1
		mComponents.push_back(mChest);
		mChest->SetPosition(12, 0, 5, 0, 3.14, 0, 3.0, 3.0, 3.0);


		mChest1 = new TreasureChest(*this, *mCamera); //Chest2
		mComponents.push_back(mChest1);
		mChest1->SetPosition(-100, 0, 260, 0, 0, 0, 3.0, 3.0, 3.0);

		mChest2 = new TreasureChest(*this, *mCamera);//Chest3
		mComponents.push_back(mChest2);
		mChest2->SetPosition(-100, 0, 30, 0, 1.57, 0, 3.0, 3.0, 3.0);

		mChest3 = new TreasureChest(*this, *mCamera);//Chest4
		mComponents.push_back(mChest3);
		mChest3->SetPosition(-265, 0, 10, 0, 3.14, 0, 3.0, 3.0, 3.0);

		mChest4 = new TreasureChest(*this, *mCamera);//Chest5
		mComponents.push_back(mChest4);
		mChest4->SetPosition(-265, 0, 100, 0, 0 , 0, 3.0, 3.0, 3.0);

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
	
		/*Interaction(gameTime, mChest);
		Interaction(gameTime, mChest1);
		Interaction(gameTime, mChest2);
		Interaction(gameTime, mChest3);
		Interaction(gameTime, mChest4);*/

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


	//void RenderingGame::Interaction(const GameTime& gameTime, TreasureChest* chest)
	//{
	//	XMFLOAT4X4 P;
	//	XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());


	//	//Compute picking ray in view space.
	//	float vx = (+2.0f*Game::screenX / Game::DefaultScreenWidth - 1.0f) / P(0, 0);
	//	float vy = (-2.0f*Game::screenY / Game::DefaultScreenHeight + 1.0f) / P(1, 1);

	//	// Ray definition in view space.
	//	XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	//	XMVECTOR rayDir = XMVectorSet(vx, vy, -1.0f, 0.0f);

	//	// Tranform ray to local space of Mesh via the inverse of both of view and world transform

	//	XMMATRIX V = mCamera->ViewMatrix();
	//	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


	//	XMMATRIX W = XMLoadFloat4x4(chest->WorldMatrix());//&mWorldMatrix);
	//	XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	//	XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

	//	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	//	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	//	// Make the ray direction unit length for the intersection tests.
	//	rayDir = XMVector3Normalize(rayDir);

	//	float tmin = 0.0;
	//	if (chest->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
	//	{
	//		float elapsedTime = (float)gameTime.ElapsedGameTime();

	//		if (mMouse->IsButtonDown(MouseButtonLeft))
	//		{
	//			int result = MessageBox(0, L"Treasure Found!", L"Surprise!", MB_ICONASTERISK || MB_YESNO);

	//			if (result == IDYES)
	//			{
	//				chest->SetVisible(false);

	//				DeleteObject(chest);
	//			}
	//		}

	//	}
	//}



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
		DeleteObject(mChest);
		DeleteObject(mChest1);
		DeleteObject(mChest2);
		DeleteObject(mChest3);
		DeleteObject(mChest4);
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