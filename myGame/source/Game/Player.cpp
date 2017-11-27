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
		mWalkRightAnimation = new Model(*mGame, "Content\\Models\\WalkRight.dae", true);
		mWalkLeftAnimation = new Model(*mGame, "Content\\Models\\WalkLeft.dae", true);
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

		mIdlePlayer = new AnimationPlayer(*mGame, *mSkinnedModel, false);
		mWalkForwardPlayer = new AnimationPlayer(*mGame, *mWalkForwardAnimation, false);
		mWalkRightPlayer = new AnimationPlayer(*mGame, *mWalkRightAnimation, false);
		mWalkLeftPlayer = new AnimationPlayer(*mGame, *mWalkLeftAnimation, false);
		mWalkBackPlayer = new AnimationPlayer(*mGame, *mWalkBackAnimation, false);
		mJumpPlayer = new AnimationPlayer(*mGame, *mJumpAnimation, false);

		mAnimationPlayer = mIdlePlayer;
		mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));

		//ScreenMessage::message = "Player Initialized";
		//ScreenMessage::PushMessage("Player Initialized");
	}

	void Rendering::Player::Update(const GameTime & gameTime)
	{
		
		//mAngle += XM_PI * static_cast<float>(gameTime.ElapsedGameTime());
		//x += XM_PI * static_cast<float>(gameTime.ElapsedGameTime());

		//XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mAngle));
		XMMATRIX rotationMatrix = XMMatrixRotationY(3.14f);
		XMMATRIX scaleMatrix = XMMatrixScaling(0.05f, 0.05f, 0.05f);
		XMMATRIX translationMatrix = XMMatrixTranslation(x, 0, z);
		XMStoreFloat4x4(&mWorldMatrix, scaleMatrix * rotationMatrix * translationMatrix);

		if (mKeyboard->IsKeyDown(DIK_W) && !mIsWalking && !mIsJumping)
		{
			mAnimationPlayer = mWalkForwardPlayer;
			mAnimationPlayer->StartClip(*(mWalkForwardAnimation->Animations().at(0)));
			mIsWalking = true;
			mIdlePlaying = false;
		}
		if (mKeyboard->IsKeyDown(DIK_D) && !mIsWalking && !mIsJumping)
		{
			mAnimationPlayer = mWalkRightPlayer;
			mAnimationPlayer->StartClip(*(mWalkRightAnimation->Animations().at(0)));
			mIsWalking = true;
			mIdlePlaying = false;
		}
		if (mKeyboard->IsKeyDown(DIK_A) && !mIsWalking && !mIsJumping)
		{
			mAnimationPlayer = mWalkLeftPlayer;
			mAnimationPlayer->StartClip(*(mWalkLeftAnimation->Animations().at(0)));
			mIsWalking = true;
			mIdlePlaying = false;
		}
		if (mKeyboard->IsKeyDown(DIK_S) && !mIsWalking && !mIsJumping)
		{
			mAnimationPlayer = mWalkBackPlayer;
			mAnimationPlayer->StartClip(*(mWalkBackAnimation->Animations().at(0)));
			mIsWalking = true;
			mIdlePlaying = false;
		}
		if (mKeyboard->IsKeyUp(DIK_W) && mKeyboard->IsKeyUp(DIK_D) && mKeyboard->IsKeyUp(DIK_A) && mKeyboard->IsKeyUp(DIK_S))
		{
			mIsWalking = false;
		}
		if (mKeyboard->WasKeyPressedThisFrame(DIK_SPACE) && !mIsJumping)
		{
			mAnimationPlayer = mJumpPlayer;
			mAnimationPlayer->StartClip(*(mJumpAnimation->Animations().at(0)));
			mIsJumping = true;
			mIsWalking = false;
			mIdlePlaying = false;
		}
		if (mIsJumping && mAnimationPlayer->CurrentKeyframe() >= mJumpAnimation->Animations().at(0)->KeyframeCount() - 2)
		{
			mIsJumping = false;
		}
		if (!mIsWalking && !mIsJumping && !mIdlePlaying)
		{
			mIdlePlaying = true;
			mAnimationPlayer = mIdlePlayer;
			mAnimationPlayer->StartClip(*(mSkinnedModel->Animations().at(0)));
		}


		mAnimationPlayer->Update(gameTime);

	}

	void Rendering::Player::Draw(const GameTime & gameTime)
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





}