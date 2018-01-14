#include "Mirror.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include <WICTextureLoader.h>
#include "DiffuseLightingMaterial.h"
#include "SpotLight.h"
#include "VectorHelper.h"
#include "Mouse.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Mirror)

		const XMFLOAT2 Mirror::LightRotationRate = XMFLOAT2(XM_PIDIV4, XM_PIDIV4);

		Mirror::Mirror(Game& game, Camera& camera, Mouse& mouse, SpotLight& spotLight1, SpotLight& spotLight2, int id)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mWorldMatrix(MatrixHelper::Identity), mScaleMatrix(MatrixHelper::Identity),
		mPosition(Vector3Helper::Zero), mDirection(Vector3Helper::Right), mUp(Vector3Helper::Forward), mRight(Vector3Helper::Up),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;
		mSpotLight1 = &spotLight1;
		mSpotLight2 = &spotLight2;
		mMouse = &mouse;
		mID = id;
		mDefaultRadius = mSpotLight2->Radius();
	}

	Mirror::~Mirror()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
		DeleteObject(mSpotLight1);
		DeleteObject(mSpotLight2);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	const XMFLOAT3& Mirror::Position() const
	{
		return mPosition;
	}

	const XMFLOAT3& Mirror::Direction() const
	{
		return mDirection;
	}

	const XMFLOAT3& Mirror::Up() const
	{
		return mUp;
	}

	const XMFLOAT3& Mirror::Right() const
	{
		return mRight;
	}

	XMVECTOR Mirror::PositionVector() const
	{
		return XMLoadFloat3(&mPosition);
	}

	XMVECTOR Mirror::DirectionVector() const
	{
		return XMLoadFloat3(&mDirection);
	}

	XMVECTOR Mirror::UpVector() const
	{
		return XMLoadFloat3(&mUp);
	}

	XMVECTOR Mirror::RightVector() const
	{
		return XMLoadFloat3(&mRight);
	}

	void Mirror::SetPosition(FLOAT x, FLOAT y, FLOAT z)
	{
		XMVECTOR position = XMVectorSet(x, y, z, 1.0f);
		SetPosition(position);
	}

	void Mirror::SetPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&mPosition, position);
	}

	void Mirror::SetPosition(const XMFLOAT3& position)
	{
		mPosition = position;
	}

	void Mirror::ApplyRotation(CXMMATRIX transform)
	{
		XMVECTOR direction = XMLoadFloat3(&mDirection);
		XMVECTOR up = XMLoadFloat3(&mUp);

		direction = XMVector3TransformNormal(direction, transform);
		direction = XMVector3Normalize(direction);

		up = XMVector3TransformNormal(up, transform);
		up = XMVector3Normalize(up);

		XMVECTOR right = XMVector3Cross(direction, up);
		up = XMVector3Cross(right, direction);

		XMStoreFloat3(&mDirection, direction);
		XMStoreFloat3(&mUp, up);
		XMStoreFloat3(&mRight, right);
	}

	void Mirror::ApplyRotation(const XMFLOAT4X4& transform)
	{
		XMMATRIX transformMatrix = XMLoadFloat4x4(&transform);
		ApplyRotation(transformMatrix);
	}

	void Mirror::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\LightLock.fbx", true));

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->CompileFromFile(L"Content\\Effects\\DiffuseLighting.fx");
		mMaterial = new DiffuseLightingMaterial();
		mMaterial->Initialize(mEffect);

		// Create the vertex and index buffers
		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		mColorTextureVariable = mEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\LightLock.png";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);
	}

	void Mirror::Update(const GameTime& gameTime)
	{
		XMMATRIX worldMatrix = XMMatrixIdentity();
		MatrixHelper::SetForward(worldMatrix, mDirection);
		MatrixHelper::SetUp(worldMatrix, mUp);
		MatrixHelper::SetRight(worldMatrix, mRight);
		MatrixHelper::SetTranslation(worldMatrix, mPosition);

		XMStoreFloat4x4(&mWorldMatrix, XMLoadFloat4x4(&mScaleMatrix) * worldMatrix);

		Rotate(gameTime);

		switch (mID)
		{
			case 1:
				if (this->Direction().x < -0.65f) {
					mSpotLight2->SetRadius(0);
				} else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 2:
				if (this->Direction().x < -0.76f) {
					mSpotLight2->SetRadius(0);
				} else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 3:
				if (this->Direction().x < -0.7f || mSpotLight1->Direction().x < -0.8f || mSpotLight1->Direction().x > -0.5f || mSpotLight1->Radius() == 0) {
					mSpotLight2->SetRadius(0);
				}
				else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 4:
				if (this->Direction().x < -0.8f || mSpotLight1->Direction().x < 0.7f || mSpotLight1->Direction().x > 0.9f || mSpotLight1->Radius() == 0) {
					mSpotLight2->SetRadius(0);
				} else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 5:
				if (this->Direction().z > 0.75f) {
					mSpotLight2->SetRadius(0);
				}
				else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 6:
				if (this->Direction().z < -0.97f || mSpotLight1->Direction().x < -0.4f || mSpotLight1->Direction().x > -0.2f || mSpotLight1->Direction().z < 0.0f || mSpotLight1->Radius() == 0) {
					mSpotLight2->SetRadius(0);
				}
				else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
			case 7:
				if (this->Direction().z < -0.8f || mSpotLight1->Direction().x > -0.4f || mSpotLight1->Direction().x < -0.75f || mSpotLight1->Direction().z > 0.0f || mSpotLight1->Radius() == 0) {
					mSpotLight2->SetRadius(0);
				}
				else {
					mSpotLight2->SetRadius(mDefaultRadius);
				}
				break;
		}
	}

	void Mirror::Rotate(const GameTime& gameTime)
	{
		XMFLOAT4X4 P;
		XMStoreFloat4x4(&P, mCamera->ProjectionMatrix());


		//Compute picking ray in view space.
		float vx = (+2.0f*Game::screenX / Game::DefaultScreenWidth - 1.0f) / P(0, 0);
		float vy = (-2.0f*Game::screenY / Game::DefaultScreenHeight + 1.0f) / P(1, 1);

		// Ray definition in view space.
		XMVECTOR rayOrigin = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR rayDir = XMVectorSet(vx, vy, -1.0f, 0.0f);

		// Tranform ray to local space of Mesh via the inverse of both of view and world transform

		XMMATRIX V = mCamera->ViewMatrix();
		XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(V), V);


		XMMATRIX W = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);

		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);

		float tmin = 0.0;
		if (mMaterial->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{
			float elapsedTime = (float)gameTime.ElapsedGameTime();

			// Rotate directional light
			XMFLOAT2 rotationAmount = Vector2Helper::Zero;
			if (mMouse->IsButtonDown(MouseButtonLeft))
			{
				rotationAmount.x += LightRotationRate.x * elapsedTime;
			}
			if (mMouse->IsButtonDown(MouseButtonRight))
			{
				rotationAmount.x -= LightRotationRate.x * elapsedTime;
			}

			XMMATRIX lightRotationMatrix = XMMatrixIdentity();
			if (rotationAmount.x != 0)
			{
				lightRotationMatrix = XMMatrixRotationY(rotationAmount.x);
			}

			if (rotationAmount.x != 0.0f)
			{
				mSpotLight2->ApplyRotation(lightRotationMatrix);
				this->ApplyRotation(lightRotationMatrix);				
			}
		}
	}

	void Mirror::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		UINT stride = mMaterial->VertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mMaterial->WorldViewProjection() << wvp;
		mMaterial->World() << worldMatrix;
		mMaterial->ColorTexture() << mTextureShaderResourceView;
		mMaterial->pos() << mSpotLight1->PositionVector();
		mMaterial->range() << mSpotLight1->Radius();
		mMaterial->dir() << mSpotLight1->DirectionVector();
		mMaterial->cone() << mSpotLight1->OuterAngle();
		mMaterial->att() << XMFLOAT3(0.1f, 0.01f, 0.000f);
		mMaterial->ambient() << XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		mMaterial->diffuse() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}

	int Mirror::GetID()
	{
		return mID;
	}
}