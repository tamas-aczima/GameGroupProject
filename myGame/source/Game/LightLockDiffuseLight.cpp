#include "LightLockDiffuseLight.h"
#include "DiffuseLightingMaterial.h"
#include "Game.h"
#include "GameException.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Model.h"
#include "Mesh.h"
#include "Utility.h"
#include "DirectionalLight.h"
#include "Keyboard.h"
#include <WICTextureLoader.h>
#include "ProxyModel.h"
#include "RenderStateHelper.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <sstream>
#include "SpotLight.h"
#include "PointLight.h"
#include "Mirror.h"
#include "Door.h"

namespace Rendering
{
	RTTI_DEFINITIONS(LightLockDiffuseLight)

	LightLockDiffuseLight::LightLockDiffuseLight(Game& game, Camera& camera, SpotLight& spotLight)
		: DrawableGameComponent(game, camera), mEffect(nullptr), mMaterial(nullptr), mMaterial2(nullptr), mTextureShaderResourceView(nullptr), mTextureShaderResourceView2(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0), mIndexCount2(0),
		mKeyboard(nullptr), mAmbientColor(1, 1, 1, 0),
		mWorldMatrix(MatrixHelper::Identity),
		mRenderStateHelper(nullptr), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 120.0f), mSpriteUI(nullptr), mSpotLight(nullptr)
	{
		mSpotLight = &spotLight;
	}

	LightLockDiffuseLight::LightLockDiffuseLight(Game& game, Camera& camera, SpotLight& spotLight, Mirror& mirror, Door& door1, Door& door2)
		: DrawableGameComponent(game, camera), mEffect(nullptr), mMaterial(nullptr), mMaterial2(nullptr), mTextureShaderResourceView(nullptr), mTextureShaderResourceView2(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0), mIndexCount2(0),
		mKeyboard(nullptr), mAmbientColor(1, 1, 1, 0),
		mWorldMatrix(MatrixHelper::Identity), mOpenDoor(false),
		mRenderStateHelper(nullptr), mSpriteBatch(nullptr), mSpriteFont(nullptr), mTextPosition(0.0f, 120.0f), mSpriteUI(nullptr), mSpotLight(nullptr)
	{
		mSpotLight = &spotLight;
		mMirror = &mirror;
		mDoor1 = &door1;
		mDoor2 = &door2;
	}

	LightLockDiffuseLight::~LightLockDiffuseLight()
	{
		DeleteObject(mSpriteFont);
		DeleteObject(mSpriteBatch);
		DeleteObject(mSpriteUI);
		DeleteObject(mRenderStateHelper);
		DeleteObject(mSpotLight);
		ReleaseObject(mTextureShaderResourceView);
		ReleaseObject(mTextureShaderResourceView2);
		DeleteObject(mMaterial);
		DeleteObject(mMaterial2);
		DeleteObject(mEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}



	void LightLockDiffuseLight::SetPosition(const float rotateX, const float rotateY, const float rotateZ, const float scaleFactor, const float translateX, const float translateY, const float translateZ)
	{

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX RotationZ = XMMatrixRotationZ(rotateZ);
		XMMATRIX RotationX = XMMatrixRotationX(rotateX);
		XMMATRIX RotationY = XMMatrixRotationY(rotateY);
		XMMATRIX Scale = XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor);
		XMMATRIX Translation = XMMatrixTranslation(translateX, translateY, translateZ);
		worldMatrix = RotationZ*RotationX *RotationY* Scale * Translation;

		XMStoreFloat4x4(&mWorldMatrix, worldMatrix);
	}


	void LightLockDiffuseLight::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\LightLock.fbx", true));

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->CompileFromFile(L"Content\\Effects\\DiffuseLighting.fx"); //SpotLight.cso");



		mMaterial = new DiffuseLightingMaterial();
		mMaterial->Initialize(mEffect);

		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		std::wstring textureName = L"Content\\Textures\\LightLock.png";
		HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView);
		if (FAILED(hr))
		{
			throw GameException("CreateWICTextureFromFile() failed.", hr);
		}

		mKeyboard = (Keyboard*)mGame->Services().GetService(Keyboard::TypeIdClass());
		assert(mKeyboard != nullptr);

		mRenderStateHelper = new RenderStateHelper(*mGame);

		mSpriteBatch = new SpriteBatch(mGame->Direct3DDeviceContext());
		mSpriteFont = new SpriteFont(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");

	}

	void LightLockDiffuseLight::Update(const GameTime& gameTime)
	{
		if (mMirror != NULL && mMirror->Direction().x < 0.1f && mMirror->Direction().x > -0.1f)
		{
			mOpenDoor = true;
		}
		else {
			mOpenDoor = false;
		}

		if (mOpenDoor)
		{
			if (mDoor1 != NULL) mDoor1->SetVisible(false);
			if (mDoor2 != NULL) mDoor2->SetVisible(false);
		}
		else {
			if (mDoor1 != NULL) mDoor1->SetVisible(true);
			if (mDoor2 != NULL) mDoor2->SetVisible(true);
		}
	}
		

	void LightLockDiffuseLight::Draw(const GameTime& gameTime)
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
		XMVECTOR ambientColor = XMLoadColor(&mAmbientColor);

		mMaterial->WorldViewProjection() << wvp;
		mMaterial->World() << worldMatrix;
		mMaterial->ColorTexture() << mTextureShaderResourceView;
		mMaterial->pos() << mSpotLight->PositionVector();
		mMaterial->range() << mSpotLight->Radius();
		mMaterial->dir() << mSpotLight->DirectionVector();
		mMaterial->cone() << mSpotLight->OuterAngle();
		mMaterial->att() << XMFLOAT3(0.1f, 0.01f, 0.000f);
		mMaterial->ambient() << XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		mMaterial->diffuse() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);

		mRenderStateHelper->SaveAll();
		mSpriteBatch->Begin();

		std::wostringstream helpLabel;
		helpLabel << L"Ambient Intensity (+PgUp/-PgDn): " << mAmbientColor.a << "\n";
		helpLabel << L"Directional Light Intensity (+Home/-End): " << mSpotLight->Color().a << "\n";
		helpLabel << L"Rotate Directional Light (Arrow Keys)\n";
		if (mMirror != NULL)
		{
			helpLabel << L"Directionx: " << mMirror->Direction().x << "y: " << mMirror->Direction().y << "z: " << mMirror->Direction().z;
			//helpLabel << L"Open: " << mOpenDoor;
		}


		mSpriteFont->DrawString(mSpriteBatch, helpLabel.str().c_str(), mTextPosition);

		mSpriteBatch->End();
		mRenderStateHelper->RestoreAll();
	}
}