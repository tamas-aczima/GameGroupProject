#pragma once

#include "Camera.h"

namespace Library
{
	class Keyboard;
	class Mouse;

	class FirstPersonCamera : public Camera
	{
		RTTI_DECLARATIONS(FirstPersonCamera, Camera)

	public:
		FirstPersonCamera(Game& game);
		FirstPersonCamera(Game& game, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);

		virtual ~FirstPersonCamera();

		const Keyboard& GetKeyboard() const;
		void SetKeyboard(Keyboard& keyboard);

		const Mouse& GetMouse() const;
		void SetMouse(Mouse& mouse);

		float& MouseSensitivity();
		float& RotationRate();
		float& MovementRate();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;

		static const float DefaultMouseSensitivity;
		static const float DefaultRotationRate;
		static const float DefaultMovementRate;

		void set_IsEditing_ON();
		void set_IsEditing_OFF();
		bool getIsEditing();
		
		void set_FPS_ON();
		void set_FPS_OFF();
		bool GetIsFPS();
		void LookBack();
		void LookForward();

		void Rotate(float Angle, GameTime gt);
		void LookRight();
		void LookLeft();

	protected:
		float mMouseSensitivity;
		float mRotationRate;
		float mMovementRate;

		Keyboard* mKeyboard;
		Mouse* mMouse;

	private:
		FirstPersonCamera(const FirstPersonCamera& rhs);
		FirstPersonCamera& operator-(const FirstPersonCamera& rhs);

		bool isEditing = false;

		bool isFPS = false;
		bool isLookBack = false;

		bool isLookRight = true;
		bool isLookLeft = true;


	};
}