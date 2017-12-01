#include "FirstPersonCamera.h"
#include "Game.h"
#include "GameTime.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "VectorHelper.h"

namespace Library
{
	RTTI_DEFINITIONS(FirstPersonCamera)

	const float FirstPersonCamera::DefaultRotationRate = XMConvertToRadians(1.0f);
	const float FirstPersonCamera::DefaultMovementRate = 10.0f;
	const float FirstPersonCamera::DefaultMouseSensitivity = 100.0f;

	FirstPersonCamera::FirstPersonCamera(Game& game)
		: Camera(game), mKeyboard(nullptr), mMouse(nullptr),
		mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate)
	{
	}

	//FirstPersonCamera::FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance)
	//	: Camera(game, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance), mKeyboard(nullptr), mMouse(nullptr),
	//	mMouseSensitivity(DefaultMouseSensitivity), mRotationRate(DefaultRotationRate), mMovementRate(DefaultMovementRate)

	//{
	//}

	FirstPersonCamera::~FirstPersonCamera()
	{
		mKeyboard = nullptr;
		mMouse = nullptr;
	}

	const Keyboard& FirstPersonCamera::GetKeyboard() const
	{
		return *mKeyboard;
	}

	void FirstPersonCamera::SetKeyboard(Keyboard& keyboard)
	{
		mKeyboard = &keyboard;
	}

	const Mouse& FirstPersonCamera::GetMouse() const
	{
		return *mMouse;
	}

	void FirstPersonCamera::SetMouse(Mouse& mouse)
	{
		mMouse = &mouse;
	}

	float& FirstPersonCamera::MouseSensitivity()
	{
		return mMouseSensitivity;
	}


	float& FirstPersonCamera::RotationRate()
	{
		return mRotationRate;
	}

	float& FirstPersonCamera::MovementRate()
	{
		return mMovementRate;
	}

	void FirstPersonCamera::Initialize()
	{
		mKeyboard = (Keyboard*)mGame->Services().GetService(Keyboard::TypeIdClass());
		mMouse = (Mouse*)mGame->Services().GetService(Mouse::TypeIdClass());

		Camera::Initialize();

		//Camera Rotation ----------------------------
		XMFLOAT2 rotation = XMFLOAT2(3.14,-0.6);

		//XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR rotationVector = XMLoadFloat2(&rotation);

		XMVECTOR right = XMLoadFloat3(&mRight);
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
		//--------------------------------------

	}

	void FirstPersonCamera::Update(const GameTime& gameTime)
	{
		XMFLOAT3 movementAmount = Vector3Helper::Zero;
		if (mKeyboard != nullptr)
		{

			if (isEditing)
			{
				if (mKeyboard->IsKeyDown(DIK_W))
				{
					movementAmount.y = 3.0f;
				}

				if (mKeyboard->IsKeyDown(DIK_S))
				{
					movementAmount.y = -3.0f;
				}

				if (mKeyboard->IsKeyDown(DIK_A))
				{
					movementAmount.x = -3.0f;
				}

				if (mKeyboard->IsKeyDown(DIK_D))
				{
					movementAmount.x = 3.0f;
				}

			}
		

		}

		XMFLOAT2 rotationAmount = Vector2Helper::Zero;
		//if ((mMouse != nullptr) && (mMouse->IsButtonHeldDown(MouseButtonLeft)))
		//{
			LPDIMOUSESTATE mouseState = mMouse->CurrentState();
			rotationAmount.x = -mouseState->lX * mMouseSensitivity;
			//rotationAmount.y = -mouseState->lY * mMouseSensitivity;
		//}



		/*if (mKeyboard->IsKeyDown(DIK_Q))
		{
			rotationAmount.y = -10.0f;
		}*/

		float elapsedTime = (float)gameTime.ElapsedGameTime();
		//XMFLOAT2 rotation = XMFLOAT2(0, 10);
		////XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		//XMVECTOR rotationVector = XMLoadFloat2(&rotation);

		//XMVECTOR right = XMLoadFloat3(&mRight);		
		//XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		//XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		//ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));

		/*XMVECTOR right = XMLoadFloat3(&mRight);

		XMFLOAT2 rotation = XMFLOAT2(0, 10);
		XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));*/
		XMVECTOR right = XMLoadFloat3(&mRight);
		//Apply rotation in Editing MODE
		if (isEditing)
		{
			XMFLOAT2 rotation = XMFLOAT2(0, 10);
			XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
			right = XMLoadFloat3(&mRight);
		
			XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
			XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

			ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
		}
		
		//_----------------------------------------------
		if (isEditing)
		{
			XMVECTOR position = XMLoadFloat3(&mPosition);
			XMVECTOR movement = XMLoadFloat3(&movementAmount) * mMovementRate * elapsedTime;

			XMVECTOR strafe = right * XMVectorGetX(movement);
			position += strafe;

			//XMVECTOR forward = XMLoadFloat3(&mDirection) * XMVectorGetY(movement);

			XMVECTOR forward = XMLoadFloat3(&mDirection) * XMVectorGetY(movement);
			//else forward = XMLoadFloat3(&mDirection) * movement;
			position += forward;


			XMVECTOR upDown = XMLoadFloat3(&mUp) * XMVectorGetZ(movement);
			position += upDown;

			XMStoreFloat3(&mPosition, position);

		}
		Camera::Update(gameTime);
	}


	void FirstPersonCamera::set_IsEditing_ON()
	{
		isEditing = true;
	}
	void FirstPersonCamera::set_IsEditing_OFF()
	{
		isEditing = false;
	}

	bool FirstPersonCamera::getIsEditing()
	{
		return isEditing;
	}

	void FirstPersonCamera::set_FPS_ON()
	{
		//Camera Rotation ----------------------------
		XMFLOAT2 rotation = XMFLOAT2(0, 0.6);

		//XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR rotationVector = XMLoadFloat2(&rotation);

		XMVECTOR right = XMLoadFloat3(&mRight);
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
		//--------------------------------------
		isFPS = true;
	}

	void FirstPersonCamera::set_FPS_OFF()
	{
		//Camera Rotation ----------------------------
		XMFLOAT2 rotation = XMFLOAT2(0, -0.6);

		//XMVECTOR rotationVector = XMLoadFloat2(&rotationAmount) * mRotationRate * elapsedTime;
		XMVECTOR rotationVector = XMLoadFloat2(&rotation);

		XMVECTOR right = XMLoadFloat3(&mRight);
		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
		//--------------------------------------
		isFPS = false;
	}

	bool FirstPersonCamera::GetIsFPS()
	{
		return isFPS;
	}

	void FirstPersonCamera::Rotate(float angle)
	{
		XMVECTOR right = XMLoadFloat3(&mRight);

		XMFLOAT2 rotation = XMFLOAT2(angle, 0);
		XMVECTOR rotationVector = XMLoadFloat2(&rotation);
		right = XMLoadFloat3(&mRight);

		XMMATRIX pitchMatrix = XMMatrixRotationAxis(right, XMVectorGetY(rotationVector));
		XMMATRIX yawMatrix = XMMatrixRotationY(XMVectorGetX(rotationVector));

		ApplyRotation(XMMatrixMultiply(pitchMatrix, yawMatrix));
	}

}