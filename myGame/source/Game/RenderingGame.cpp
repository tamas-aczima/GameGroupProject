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
//#include "LightRay.h"
#include "PlayerBoundingBox.h"
#include "Wall.h"


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
		
		/*wall = new Wall(*this, *mCamera);
		mComponents.push_back(wall);
		wall->SetTransform(13, 0, 65, 0, 0, 0, 1, 1.5, 0.1);
		gameObjects.push_back(wall);

		wall2 = new Wall(*this, *mCamera);
		mComponents.push_back(wall2);
		wall2->SetTransform(13, 0, 15, 0, 0, 0, 1, 1.5, 0.1);
		gameObjects.push_back(wall2);*/

		mTreasureChest = new TreasureChest(*this, *mCamera);
		mComponents.push_back(mTreasureChest);

		mTreasureChest->SetPosition(0, 0, -50.0f, -1.57f, 0, 0, 1,1,1);

		/*mRock = new Rock(*this, *mCamera);
		mComponents.push_back(mRock);
		mRock->SetPosition(300, 0, -50, 0, 0, 0, 20,1,1);

		Rock* mRock2 = new Rock(*this, *mCamera);
		mComponents.push_back(mRock2);
		mRock2->SetPosition(300, 0, -300, 0, 0, 0, 20,1,1);*/

	    player = new Player(*this, *mCamera);
		player->Initialize();
		player->SetUpPosition(13, 0, 45);
		mComponents.push_back(player);

		playerBoundingBox = new PlayerBoundingBox(*this, *mCamera);
		playerBoundingBox->Initialize();
		playerBoundingBox->SetPosition(13, 2, 45, 0, 0, 0, 0.18, 0.3, 0.1);
		mComponents.push_back(playerBoundingBox);

		/*wall = new Wall(*this, *mCamera);
		wall->Initialize();
		wall->SetPosition(13, 2, 45, 0, 0, 0, 0.18, 0.3, 0.1);
		mComponents.push_back(wall);*/


		/*LightRay* lightRay = new LightRay(*this, *mCamera);
		mComponents.push_back(lightRay);
		lightRay->SetPosition(10, 5, 10, 0, -1.57f, 0, 100, 0.5f, 0.5f);*/
	
		//player->SetPosition(0,0,10.0f,0,0,0,1,1,1);

		/*mAnimation = new PlayerAnimation(*this, *mCamera);
		mComponents.push_back(mAnimation);
		mAnimation->SetPosition(0, 0, -10, 0, 0, 0, 1, 1, 1);*/

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		mSpriteBatch = new SpriteBatch(mDirect3DDeviceContext);
		mSpriteFont = new SpriteFont(mDirect3DDevice, L"Content\\Fonts\\Arial_14_Regular.spritefont");

		Game::Initialize();

		mCamera->SetPosition(player->getPosition().x, 20.0f, player->getPosition().z - 20.0f);

		//Test message
		//ScreenMessage::PushMessage("Rendering_game_Initialized");
		
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		mFpsComponent->Update(gameTime);

		if (mKeyboard->WasKeyPressedThisFrame(DIK_ESCAPE))
		{
			Exit();
		}

		//Update bounding box position
		playerBoundingBox->SetTransform(player->x, 2, player->z, 0, 0, 0, 0.18, 0.3, 0.1);

		if (!mCamera->getIsEditing())
		{
			//Camera movement------
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

			zoomX = 20 * player->GetLocalForward().x;
			zoomZ = -20 * player->GetLocalForward().y;
			//----------------------------

			//Update the camera position
			mCamera->SetPosition(player->getPosition().x + zoomX , zoomY, player->getPosition().z - zoomZ);
			
			// Camera Zoom In / Out --------------------------------------------------------------------	
			if (!mCamera->GetIsFPS())
			{
				zoomingSpeed = (float)mMouse->Wheel() / 1000;
				mMouse->WheelToZero();

				//Zoom IN
				if ((float)mMouse->Wheel() / 1000 > 0 && mCamera->Position().y > 13 && mCamera->Position().z < player->getPosition().z - 9)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoomZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
				}

				//Zoom Out
				if ((float)mMouse->Wheel() / 1000 < 0 && mCamera->Position().y < 20 && mCamera->Position().z > player->getPosition().z - 20)
				{
					zoomY -= 1 * zoomingSpeed * gameTime.ElapsedGameTime();
					zoomZ -= 1.5f * zoomingSpeed * gameTime.ElapsedGameTime();
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
					zoomY = 20.0f;
					zoomZ = 20;
					//--------------------

					mCamera->set_FPS_OFF();

					break;

				case false:
					//FIRST PERSON CAMERA-----
					zoomY = 5;
					zoomZ = -2;
					//--------------------------

					mCamera->set_FPS_ON();

					break;
				}
			}
			
			//Player movement------
			if (mKeyboard->IsKeyDown(DIK_W) && !isColliding)
			{
				player->x -= player->GetLocalForward().x * gameTime.ElapsedGameTime() * 5;
				player->z -= player->GetLocalForward().y * gameTime.ElapsedGameTime() * 5;
			}

			if (mKeyboard->IsKeyDown(DIK_S))
			{
				player->x += player->GetLocalForward().x * gameTime.ElapsedGameTime() * 5;
				player->z += player->GetLocalForward().y * gameTime.ElapsedGameTime() * 5;
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

		
		//Pick(Game::screenX, Game::screenY, mLevel->wall2);
		//Pick(player->x, player->y,player->z, mLevel->wall2);

		/*if (mLevel->wall2 != NULL)
		{	
			if (mLevel->wall2->mTextureMaterial->mBoundingBox.Intersects(playerBoundingBox->mBoundingBox))
			{
				isColliding = true;
			}
			else
			{
				isColliding = false;
			}
			
		}*/

		//distance = Distance(player->positionVector, mLevel->wall2->positionVector);
		/*if (distance < 3)
			isColliding = true;
		else
			isColliding = false;*/

		CollisionDetection(player->x, player->z);
		//mLevel->UpdatePlayerLocation(player->positionVector, gameTime);
		
		/*for (int i = 0; i < gameObjects.size(); i++)
		{
			distance = Distance(player->positionVector, gameObjects.at(i)->positionVector);
			if (distance < 6.5)
			{
				LookingAtObject = i;
			}
			
			distance2 = Distance(player->positionVector, gameObjects.at(LookingAtObject)->positionVector);
			if (distance2 < 6.5)
			{
				isColliding = true;
			}
			else
			{
				isColliding = false;
			}
			
				
		}*/


		Game::Update(gameTime);
	}

	float RenderingGame::Distance(const XMVECTOR& point1, const XMVECTOR& point2)
	{
		float distance = sqrt( (XMVectorGetX(point1) - XMVectorGetX(point2)) * (XMVectorGetX(point1) - XMVectorGetX(point2)) +
			(XMVectorGetY(point1) - XMVectorGetY(point2)) * (XMVectorGetY(point1) - XMVectorGetY(point2)) +
			(XMVectorGetZ(point1) - XMVectorGetZ(point2)) * (XMVectorGetZ(point1) - XMVectorGetZ(point2)) );
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
			if (distance2 < 6.5)
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
		//playerLocation << "x " << player->getPosition().x << "    " << "z " << player->getPosition().z;
		playerLocation << distance << " distance 2 " << distance2; 
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

		//Bounding box test
		if (isColliding)
		{
			std::wostringstream coll;
			XMFLOAT2 collLoc = XMFLOAT2(5, 180);
			coll << "Collision";
			mSpriteFont->DrawString(mSpriteBatch, coll.str().c_str(), collLoc);
		}


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

	void RenderingGame::Pick(float sx, float sy, float sz, Wall * model)
	{

		//XMMATRIX P = mCam.Proj(); 
		XMFLOAT4X4 P;
		XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());

		//Compute picking ray in view space.
		/*float vx = (+2.0f*sx / Game::DefaultScreenWidth - 1.0f) / P(0, 0);
		float vy = (-2.0f*sz / Game::DefaultScreenHeight + 1.0f) / P(1, 1);
		*/

		// Ray definition in view space.
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		//XMVECTOR rayDir = XMVectorSet(vx, vy, -1.0f, 0.0f);	
		//XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(sx, 1.0f, sz, 0.0f);

		// Tranform ray to local space of Mesh via the inverse of both of view and world transform
		XMMATRIX V = mCamera->ViewMatrix();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


		XMMATRIX W = XMLoadFloat4x4(model->WorldMatrix());
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);
		

		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);

		float tmin = 0.0;
		if (model->mTextureMaterial->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{
			/*std::wostringstream pickupString;
			pickupString << L"Do you want to pick up: " << '\n' << '\t' << '+' << L" points";

			int result = MessageBox(0, pickupString.str().c_str(), L"Object Found", MB_ICONASTERISK | MB_YESNO);

			if (result == IDYES)
			{


			}*/
			if(distance < 6.5)
				isColliding = true;
		}
		else
			isColliding = false;

	}

}