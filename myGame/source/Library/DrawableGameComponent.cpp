#include "DrawableGameComponent.h"
#include "GameTime.h"

namespace Library
{
	RTTI_DEFINITIONS(DrawableGameComponent)

	DrawableGameComponent::DrawableGameComponent()
		: GameComponent(), mVisible(true), mCamera(nullptr)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game)
		: GameComponent(game), mVisible(true), mCamera(nullptr)
	{
	}

	DrawableGameComponent::DrawableGameComponent(Game& game, Camera& camera)
		: GameComponent(game), mVisible(true), mCamera(&camera)
	{
	}

	DrawableGameComponent::~DrawableGameComponent()
	{
	}

	bool DrawableGameComponent::Visible() const
	{
		return mVisible;
	}

	void DrawableGameComponent::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	Camera* DrawableGameComponent::GetCamera()
	{
		return mCamera;
	}

	void DrawableGameComponent::SetCamera(Camera* camera)
	{
		mCamera = camera;
	}

	void DrawableGameComponent::Draw(const GameTime& gameTime)
	{
	}

	void DrawableGameComponent::SetPosition(const float translateX, const float translateY, const float translateZ,
		const float rotateX, const float rotateY, const float rotateZ, const float scale)
	{
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX translationMatrix = XMMatrixTranslation(translateX, translateY, translateZ);
		XMMATRIX rotationXMatrix = XMMatrixRotationX(rotateX);
		XMMATRIX rotationYMatrix = XMMatrixRotationX(rotateY);
		XMMATRIX rotationZMatrix = XMMatrixRotationX(rotateZ);
		XMMATRIX scaleMatrix = XMMatrixScaling(scale, scale, scale);
		worldMatrix = rotationZMatrix * rotationYMatrix * rotationXMatrix * scaleMatrix * translationMatrix;

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}
}