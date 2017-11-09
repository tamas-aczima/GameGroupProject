#pragma once

#include "GameComponent.h"

namespace Library
{
	class Camera;

	class DrawableGameComponent : public GameComponent
	{
		RTTI_DECLARATIONS(DrawableGameComponent, GameComponent)

	public:
		DrawableGameComponent();
		DrawableGameComponent(Game& game);
		DrawableGameComponent(Game& game, Camera& camera);
		virtual ~DrawableGameComponent();

		bool Visible() const;
		void SetVisible(bool visible);

		Camera* GetCamera();
		void SetCamera(Camera* camera);

		virtual void Draw(const GameTime& gameTime);
		void SetPosition(const float translateX, const float translateY, const float translateZ,
			const float rotateX, const float rotateY, const float rotateZ, const float scaleX, const float scaleY, const float scaleZ);

	protected:
		bool mVisible;
		Camera* mCamera;
		XMFLOAT4X4 mWorldMatrix;

	private:
		DrawableGameComponent(const DrawableGameComponent& rhs);
		DrawableGameComponent& operator=(const DrawableGameComponent& rhs);
	};
}