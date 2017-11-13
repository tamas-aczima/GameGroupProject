#include "TreasureChest.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include <WICTextureLoader.h>

namespace Rendering
{
	RTTI_DEFINITIONS(TreasureChest)

		TreasureChest::TreasureChest(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mTextureMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;
	}

	TreasureChest::~TreasureChest()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mTextureMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void TreasureChest::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\treasure_chest.obj", true));

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
		mTextureName = L"Content\\Textures\\treasure_chest.jpg";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);
	}

	void TreasureChest::Update(const GameTime& gameTime)
	{
	}

	void TreasureChest::Draw(const GameTime& gameTime)
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
}