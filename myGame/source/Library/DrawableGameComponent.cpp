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
		const float rotateX, const float rotateY, const float rotateZ, const float scaleX, const float scaleY, const float scaleZ)
	{
		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX translationMatrix = XMMatrixTranslation(translateX, translateY, translateZ);
		////New position var added
		//x = translateX;
		//y = translateY;
		//z = translateZ;

		XMMATRIX rotationXMatrix = XMMatrixRotationX(rotateX);
		XMMATRIX rotationYMatrix = XMMatrixRotationY(rotateY);
		XMMATRIX rotationZMatrix = XMMatrixRotationZ(rotateZ);
		XMMATRIX scaleMatrix = XMMatrixScaling(scaleX, scaleY, scaleZ);
		worldMatrix = scaleMatrix * rotationZMatrix * rotationYMatrix * rotationXMatrix * translationMatrix;

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}

	/*XMMATRIX DrawableGameComponent::getPostion()
	{

		return XMMATRIX(x,y,z);
	}*/


}