#include "Wall.h"
#include "Game.h"
#include "GameException.h"
#include "MatrixHelper.h"
#include "Camera.h"
#include "Utility.h"
#include "Model.h"
#include "Mesh.h"
#include <WICTextureLoader.h>
#include "ColorHelper.h"
#include "DiffuseLightingMaterial.h"
#include <WICTextureLoader.h>
#include "SpotLight.h"
#include "DepthMapMaterial.h"
#include "ShadowMappingMaterial.h"
#include "VectorHelper.h"
#include "DepthMap.h"
#include "Projector.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Wall)

	const UINT Wall::DepthMapWidth = 1024U;
	const UINT Wall::DepthMapHeight = 1024U;

	Wall::Wall(Game& game, Camera& camera, SpotLight& spotLight)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr),
		mAmbientColor(reinterpret_cast<const float*>(&ColorHelper::White)), mDepthMap(nullptr),
		mProjector(nullptr), mRenderStateHelper(game), mDepthBiasState(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;
		mSpotLight = &spotLight;
	}

	Wall::~Wall()
	{
		ReleaseObject(mDepthBiasState)
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mMaterial);
		DeleteObject(mEffect);
		DeleteObject(mDepthMap);
		DeleteObject(mProjector);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void Wall::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		// Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\Cube.obj", true));

		// Initialize the material
		mEffect = new Effect(*mGame);
		mEffect->CompileFromFile(L"Content\\Effects\\DiffuseLightingWithShadow.fx");
		mMaterial = new ShadowMappingMaterial();
		mMaterial->Initialize(mEffect);

		mDepthMapEffect = new Effect(*mGame);
		mDepthMapEffect->CompileFromFile(L"Content\\Effects\\DepthMap.fx");

		mDepthMapMaterial = new DepthMapMaterial();
		mDepthMapMaterial->Initialize(mDepthMapEffect);

		// Plane vertex buffers
		VertexPositionTextureNormal positionUVNormalVertices[] =
		{
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),

			VertexPositionTextureNormal(XMFLOAT4(-0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), Vector3Helper::Backward),
			VertexPositionTextureNormal(XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), Vector3Helper::Backward),
		};

		mPlaneVertexCount = ARRAYSIZE(positionUVNormalVertices);
		std::vector<VertexPositionNormal> positionNormalVertices;
		positionNormalVertices.reserve(mPlaneVertexCount);
		std::vector<VertexPosition> positionVertices;
		positionVertices.reserve(mPlaneVertexCount);
		for (UINT i = 0; i < mPlaneVertexCount; i++)
		{
			positionNormalVertices.push_back(VertexPositionNormal(positionUVNormalVertices[i].Position, positionUVNormalVertices[i].Normal));
			positionVertices.push_back(VertexPosition(positionUVNormalVertices[i].Position));
		}

		mDepthMapMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), &positionVertices[0], mPlaneVertexCount, &mPlanePositionVertexBuffer);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), positionUVNormalVertices, mPlaneVertexCount, &mPlanePositionUVNormalVertexBuffer);

		mProjector = new Projector(*mGame);
		mProjector->Initialize();
		mProjector->SetPosition(mSpotLight->PositionVector());
		mProjector->ApplyRotation(mSpotLight->GetRotationMatrix());

		InitializeProjectedTextureScalingMatrix();

		// Create the vertex and index buffers
		Mesh* mesh = model->Meshes().at(0);
		mMaterial->CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();


		mColorTextureVariable = mEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();
		//Load the texture
		mTextureName = L"Content\\Textures\\rock.jpg";

		DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), mTextureName.c_str(), nullptr, &mTextureShaderResourceView);

		mDepthMap = new DepthMap(*mGame, DepthMapWidth, DepthMapHeight);
	}

	void Wall::Update(const GameTime& gameTime)
	{
		
	}

	void Wall::Draw(const GameTime& gameTime)
	{
		//depth map
		mRenderStateHelper.SaveRasterizerState();
		mDepthMap->Begin();

		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//direct3DDeviceContext->ClearDepthStencilView(mDepthMap->DepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		Pass* pass = mDepthMapMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mDepthMapMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		direct3DDeviceContext->RSSetState(mDepthBiasState);

		UINT stride = mDepthMapMaterial->VertexSize();
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		mDepthMapMaterial->WorldLightViewProjection() << worldMatrix * mProjector->ViewMatrix() * mProjector->ProjectionMatrix();

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0); //shadow map drawing

		mDepthMap->End();
		mRenderStateHelper.RestoreRasterizerState();

		pass = mMaterial->CurrentTechnique()->Passes().at(0);
		inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		stride = mMaterial->VertexSize();

		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();

		mAmbientColor.a = 100;
		XMVECTOR ambientColor = XMLoadColor(&mAmbientColor);

		mMaterial->WorldViewProjection() << wvp;
		mMaterial->World() << worldMatrix;
		mMaterial->pos() << mSpotLight->PositionVector();
		mMaterial->range() << mSpotLight->Radius();
		mMaterial->dir() << mSpotLight->DirectionVector();
		mMaterial->cone() << mSpotLight->OuterAngle();
		mMaterial->att() << XMFLOAT3(0.4f, 0.02f, 0.000f);
		mMaterial->ambient() << XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		mMaterial->diffuse() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);


		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		pass->Apply(0, direct3DDeviceContext);

		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}

	void Wall::InitializeProjectedTextureScalingMatrix()
	{
		mProjectedTextureScalingMatrix._11 = 0.5f;
		mProjectedTextureScalingMatrix._22 = -0.5f;
		mProjectedTextureScalingMatrix._33 = 1.0f;
		mProjectedTextureScalingMatrix._41 = 0.5f;
		mProjectedTextureScalingMatrix._42 = 0.5f;
		mProjectedTextureScalingMatrix._44 = 1.0f;
	}
}