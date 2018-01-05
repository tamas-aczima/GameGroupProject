#include "Rock.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include <WICTextureLoader.h>
#include "DirectXCollision.h"

namespace Rendering
{

	RTTI_DEFINITIONS(Rock)

		Rock::Rock(Game& game, Camera& camera) : DrawableGameComponent(game, camera),
		mTextureMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
        {
        	mWorldMatrix = MatrixHelper::Identity;
        }

	Rock::~Rock()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mTextureMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void Rock::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\mountain_canyon_01.obj", true));

		// Initialize the material
		mTextureEffect = new Effect(*mGame);
		//mTextureEffect->LoadCompiledEffect(L"Content\\Effects\\TextureEffect.cso");
		mTextureEffect->CompileFromFile(L"Content\\Effects\\TextureEffect.fx");
		mTextureMaterial = new TextureMaterial();
		mTextureMaterial->Initialize(mTextureEffect);

		// Create the vertex and index buffers
		Mesh* mesh = model->Meshes().at(0);
		mTextureMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		mBoundingBox = mTextureMaterial->GetBoundinBox();

		mColorTextureVariable = mTextureEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\mountain_01_diffuse.png";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);

	}

	void Rock::Update(const GameTime& gameTime)
	{

	}

	void Rock::Draw(const GameTime& gameTime)
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

	BoundingBox Rock::GetBoundingBox()
	{
		return mBoundingBox;
	}


}

