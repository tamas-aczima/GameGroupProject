#include "PlayerBoundingBox.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include "TextureMaterial.h"
#include <WICTextureLoader.h>
#include "ColorHelper.h"
#include "D3DCompiler.h"
#include "DirectXCollision.h"

namespace Rendering
{
	RTTI_DEFINITIONS(PlayerBoundingBox)

	PlayerBoundingBox::PlayerBoundingBox(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera),
		mTextureMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr),
		mAmbientColor(reinterpret_cast<const float*>(&ColorHelper::White))
	{

	}

	PlayerBoundingBox::~PlayerBoundingBox()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mTextureMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void Rendering::PlayerBoundingBox::Initialize()
	{
		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\Cube.obj", true));

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
		//BouningBox
		mBoundingBox = mTextureMaterial->GetBoundinBox();

		mColorTextureVariable = mTextureEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\rock.jpg";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);

	}
	 
	void Rendering::PlayerBoundingBox::Update(const GameTime & gameTime)
	{
	}

	void Rendering::PlayerBoundingBox::Draw(const GameTime & gameTime)
	{
	}

	void PlayerBoundingBox::SetTransform(float X, float Y, float Z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ)
	{
		
		this->SetPosition(X, Y, Z, rotX, rotY, rotZ, scaleX, scaleY, scaleZ);

		x = X;
		y = Y;
		z = Z;

		//XMMATRIX translationMatrix = XMMatrixTranslation(x, y, z);

		//this->mTextureMaterial->mBoundingBox.Transform(this->mTextureMaterial->mBoundingBox, translationMatrix);
	}
}


