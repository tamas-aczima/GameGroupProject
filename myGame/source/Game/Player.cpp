#include "Player.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "SkinnedModelMaterial.h"
#include <WICTextureLoader.h>
#include "VectorHelper.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "ModelMaterial.h"
#include <sstream>
#include <iomanip>
#include "Shlwapi.h"
#include "ScreenMessage.h"
#include "ColorHelper.h"

namespace Rendering
{
	
	RTTI_DEFINITIONS(Player)

	Player::Player(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr),
		mVertexBuffers(), mIndexBuffers(), mIndexCounts(), mColorTextures(),
		mAmbientColor(reinterpret_cast<const float*>(&ColorHelper::White)),
		mSkinnedModel(nullptr), mAnimationPlayer(nullptr),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;		

	}

	Player::~Player()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);

		for (ID3D11Buffer* vertexBuffer : mVertexBuffers)
		{
			ReleaseObject(vertexBuffer);
		}

		for (ID3D11Buffer* indexBuffer : mIndexBuffers)
		{
			ReleaseObject(indexBuffer);
		}

		for (ID3D11ShaderResourceView* colorTexture : mColorTextures)
		{
			ReleaseObject(colorTexture);
		}

		DeleteObject(mSkinnedModel);
		DeleteObject(mAnimationPlayer);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
	}

	void Rendering::Player::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		mSkinnedModel = new Model(*mGame, "Content\\Models\\Idle.dae", true);
		mWalkForwardAnimation = new Model(*mGame, "Content\\Models\\WalkForward.dae", true);
		mWalkBackAnimation = new Model(*mGame, "Content\\Models\\WalkBack.dae", true);
		mJumpAnimation = new Model(*mGame, "Content\\Models\\Jump.dae", true);

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->CompileFromFile(L"Content\\Effects\\SkinnedModel.fx");
		mMaterial = new SkinnedModelMaterial();
		mMaterial->Initialize(mEffect);

		// Create the vertex and index buffers
		mVertexBuffers.resize(mSkinnedModel->Meshes().size());
		mIndexBuffers.resize(mSkinnedModel->Meshes().size());
		mIndexCounts.resize(mSkinnedModel->Meshes().size());
		mColorTextures.resize(mSkinnedModel->Meshes().size());
		for (UINT i = 0; i < mSkinnedModel->Meshes().size(); i++)
		{
			Mesh* mesh = mSkinnedModel->Meshes().at(i);

			ID3D11Buffer* vertexBuffer = nullptr;
			mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &vertexBuffer);
			mVertexBuffers[i] = vertexBuffer;

			ID3D11Buffer* indexBuffer = nullptr;
			mesh->CreateIndexBuffer(&indexBuffer);
			mIndexBuffers[i] = indexBuffer;

			mIndexCounts[i] = mesh->Indices().size();

			ID3D11ShaderResourceView* colorTexture = nullptr;
			ModelMaterial* material = mesh->GetMaterial();

			if (material != nullptr && material->Textures().find(TextureTypeDiffuse) != material->Textures().cend())
			{
				std::vector<std::wstring>* diffuseTextures = material->Textures().at(TextureTypeDiffuse);
				std::wstring filename = PathFindFileName(diffuseTextures->at(0).c_str());

				std::wostringstream textureName;
				textureName << L"Content\\Textures\\" << filename.substr(0, filename.length() - 4) << L".png";
				HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.str().c_str(), nullptr, &colorTexture);
				if (FAILED(hr))
				{
					throw GameException("CreateWICTextureFromFile() failed.", hr);
				}
			}
			mColorTextures[i] = colorTexture;
		}

		XMStoreFloat4x4(&mWorldMatrix, XMMatrixScaling(0.05f, 0.05f, 0.05f));

		mKeyboard = (Keyboard*)mGame->Services().GetService(Keyboard::TypeIdClass());
		assert(mKeyboard != nullptr);
		
		mMouse = (Mouse*)mGame->Services().GetService(Mouse::TypeIdClass());
		assert(mMouse != nullptr);

		mIdlePlayer = new AnimationPlayer(*mGame, *mSkinnedModel, false);
		mWalkForwardPlayer = new AnimationPlayer(*mGame, *mWalkForwardAnimation, false);
		mWalkBackPlayer = new AnimationPlayer(*mGame, *mWalkBackAnimation, false);
		mJumpPlayer = new AnimationPlayer(*mGame, *mJumpAnimation, false);

		mAnimationPlayer = mIdlePlayer;
		mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));

		//ScreenMessage::message = "Player Initialized";
		//ScreenMessage::PushMessage("Player Initialized");
		//mRotationMatrix = MatrixHelper::Identity;
	}

	void Rendering::Player::Update(const GameTime & gameTime)
	{
		mCurrentMouseX = mMouse->X();

		if (mCurrentMouseX > mLastMouseX)
		{
			mRotation -= 1;
		}
		else if (mCurrentMouseX < mLastMouseX)
		{
			mRotation += 1;
		}
		mLastMouseX = mCurrentMouseX;

		mAngle = mRotation;
		mAngleInRadians = mAngle * XM_PI / 180;

		mLocalForward = XMVector3Normalize(XMVectorSet(sin(mAngleInRadians), 0.0f, cos(mAngleInRadians), 1.0f) * -1);

		XMMATRIX rotationMatrix = XMMatrixRotationY(mAngleInRadians);
		XMMATRIX scaleMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);
		XMMATRIX translationMatrix = XMMatrixTranslation(x, 0, z);
		XMStoreFloat4x4(&mWorldMatrix, scaleMatrix * rotationMatrix * translationMatrix);
		XMStoreFloat4x4(&mRotationMatrix, rotationMatrix);


		//Animation
		if (mKeyboard->IsKeyDown(DIK_W) && !mIsWalkingForward && !mIsWalkingBack && !mIsJumping)
		{
			mAnimationPlayer = mWalkForwardPlayer;
			mAnimationPlayer->StartClip(*(mWalkForwardAnimation->Animations().at(0)));
			mIsWalkingForward = true;
			mIdlePlaying = false;
		}
		if (mKeyboard->IsKeyDown(DIK_S) && !mIsWalkingBack && !mIsWalkingForward && !mIsJumping)
		{
			mAnimationPlayer = mWalkBackPlayer;
			mAnimationPlayer->StartClip(*(mWalkBackAnimation->Animations().at(0)));
			mIsWalkingBack = true;
			mIdlePlaying = false;
		}
		if ((mIsWalkingForward && mKeyboard->IsKeyDown(DIK_S)) || (mIsWalkingBack && mKeyboard->IsKeyDown(DIK_W)))
		{
			mIsWalkingForward = false;
			mIsWalkingBack = false;
		}
		if (mKeyboard->IsKeyUp(DIK_W))
		{
			mIsWalkingForward = false;
		}
		if (mKeyboard->IsKeyUp(DIK_S))
		{
			mIsWalkingBack = false;
		}

		if (mKeyboard->WasKeyPressedThisFrame(DIK_SPACE) && !mIsJumping)
		{
			mAnimationPlayer = mJumpPlayer;
			mAnimationPlayer->StartClip(*(mJumpAnimation->Animations().at(0)));
			mIsJumping = true;
			mIsWalkingForward = false;
			mIsWalkingBack = false;
			mIdlePlaying = false;
		}
		if (mIsJumping && mAnimationPlayer->CurrentKeyframe() >= mJumpAnimation->Animations().at(0)->KeyframeCount() - 2)
		{
			mIsJumping = false;
		}
		if (!mIsWalkingForward && !mIsWalkingBack && !mIsJumping && !mIdlePlaying)
		{
			mIdlePlaying = true;
			mAnimationPlayer = mIdlePlayer;
			mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));
		}


		mAnimationPlayer->Update(gameTime);

	}

	void Rendering::Player::Draw(const GameTime & gameTime)
	{
		if (!isFPS)
		{
			ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
			direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);
			ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
			direct3DDeviceContext->IASetInputLayout(inputLayout);

			XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
			XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
			XMVECTOR ambientColor = XMLoadColor(&mAmbientColor);

			UINT stride = mMaterial->VertexSize();
			UINT offset = 0;

			for (UINT i = 0; i < mVertexBuffers.size(); i++)
			{
				ID3D11Buffer* vertexBuffer = mVertexBuffers[i];
				ID3D11Buffer* indexBuffer = mIndexBuffers[i];
				UINT indexCount = mIndexCounts[i];
				ID3D11ShaderResourceView* colorTexture = mColorTextures[i];

				direct3DDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
				direct3DDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

				mMaterial->WorldViewProjection() << wvp;
				mMaterial->World() << worldMatrix;
				mMaterial->AmbientColor() << ambientColor;
				mMaterial->ColorTexture() << colorTexture;
				mMaterial->CameraPosition() << mCamera->PositionVector();
				mMaterial->BoneTransforms() << mAnimationPlayer->BoneTransforms();

				pass->Apply(0, direct3DDeviceContext);

				direct3DDeviceContext->DrawIndexed(indexCount, 0, 0);
			}
		}
		
	}

	void Player::SetUpPosition(float X, float Y, float Z)
	{
		this->SetPosition(X, Y, Z, 0, 0, 0, 1, 1, 1);
		x = X;
		y = Y;
		z = Z;
		
	}

	XMFLOAT3 Player::getPosition()
	{
		return XMFLOAT3(x,y,z);
	}

	XMFLOAT2 Player::GetLocalForward()
	{
		return XMFLOAT2(XMVectorGetX(mLocalForward), XMVectorGetZ(mLocalForward));
	}
	void Player::setFPS_ON()
	{
		isFPS = true;
	}
	void Player::setFPS_OFF()
	{
		isFPS = false;
	}
}