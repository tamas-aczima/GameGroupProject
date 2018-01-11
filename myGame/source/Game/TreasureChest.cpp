#include "TreasureChest.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include "DiffuseLightingMaterial.h"
#include <WICTextureLoader.h>
#include "VectorHelper.h"
#include "Mouse.h"


namespace Rendering
{
	RTTI_DEFINITIONS(TreasureChest)

		TreasureChest::TreasureChest(Game& game, Camera& camera, Mouse& mouse, int goldValue)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr), mGoldValue(goldValue)
	{
		mWorldMatrix = MatrixHelper::Identity;
		mMouse = &mouse;
		mGoldValue = goldValue;
		mCollected = false;
	}

	TreasureChest::~TreasureChest()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void TreasureChest::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		//Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\treasure_chest.obj", true));

		// Initialize the material
		mTextureEffect = new Effect(*mGame);
		mTextureEffect->CompileFromFile(L"Content\\Effects\\DiffuseLighting.fx"); //SpotLight.cso");
		mMaterial = new DiffuseLightingMaterial();
		mMaterial->Initialize(mTextureEffect);

		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		std::wstring textureName = L"Content\\Textures\\treasure_chest.jpg";
		HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView);
		if (FAILED(hr))
		{
			throw GameException("CreateWICTextureFromFile() failed.", hr);
		}

		mColorTextureVariable = mTextureEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\treasure_chest.jpg";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);
	}

	void TreasureChest::Update(const GameTime& gameTime )
	{
		/*	if (this->Visible())
			{
				Interaction(gameTime);
			}	*/
	}

	void TreasureChest::chestCollected()
	{
		mCollected = true;
	}

	void TreasureChest::Interaction(const GameTime& gameTime)//, TreasureChest* chest)
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


		XMMATRIX W = XMLoadFloat4x4(this->WorldMatrix());//&mWorldMatrix);
		XMMATRIX invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

		XMMATRIX toLocal = XMMatrixMultiply(invView, invWorld);

		rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
		rayDir = XMVector3TransformNormal(rayDir, toLocal);

		// Make the ray direction unit length for the intersection tests.
		rayDir = XMVector3Normalize(rayDir);

		float tmin = 0.0;

		if (this->mBoundingBox.Intersects(rayOrigin, rayDir, tmin))
		{

			if (mMouse->IsButtonDown(MouseButtonLeft))
			{
					this->SetVisible(false);

					
			}
		}
	}

	void TreasureChest::Draw(const GameTime& gameTime)
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
		mMaterial->att() << XMFLOAT3(0.1f, 0.01f, 0.000f);
		mMaterial->ambient() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mMaterial->diffuse() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);

	}
}