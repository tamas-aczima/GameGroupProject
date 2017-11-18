#include "Player.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include <WICTextureLoader.h>
#include "VectorHelper.h"
#include "Mesh.h"

#include "Keyboard.h"

#include "ScreenMessage.h"

namespace Rendering
{
	
	RTTI_DEFINITIONS(Player)

	Player::Player(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mTextureMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;		

	}

	Player::~Player()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mTextureMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void Rendering::Player::Initialize()
	{

		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\BEAR_BLK.obj", true));

		// Initialize the material
		mTextureEffect = new Effect(*mGame);
		mTextureEffect->CompileFromFile(L"Content\\Effects\\TextureEffect.fx");
		mTextureMaterial = new TextureMaterial();
		mTextureMaterial->Initialize(mTextureEffect);

		// Create the vertex and index buffers
		Mesh* mesh = model->Meshes().at(0);
		mTextureMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		mColorTextureVariable = mTextureEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\BEAR_BK.tif";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);
		//ScreenMessage::Message.push_back("Player Initialized");
		//ScreenMessage::message = "Player Initialized";
		ScreenMessage::PushMessage("Player Initialized");
	}


	void Rendering::Player::Update(const GameTime & gameTime)
	{
		
		//mAngle += XM_PI * static_cast<float>(gameTime.ElapsedGameTime());
		//x += XM_PI * static_cast<float>(gameTime.ElapsedGameTime());

		//XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationY(mAngle));
		XMStoreFloat4x4(&mWorldMatrix, XMMatrixTranslation(x, 0, z));
		
			

	}

	void Rendering::Player::Draw(const GameTime & gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mTextureMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mTextureMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		UINT stride = mTextureMaterial->VertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mTextureMaterial->WorldViewProjection() << wvp;

		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);

	}

	//void Player::SetUpPosition(float X, float Y, float Z)
	//{

	//	x = X;
	//	y = Y;
	//	z = Z;
	//	
	//}

	XMFLOAT3 Player::getPosition()
	{
		return XMFLOAT3(x,y,z);
	}





}