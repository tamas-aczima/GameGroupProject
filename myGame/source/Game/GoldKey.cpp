#include "GoldKey.h"
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
	RTTI_DEFINITIONS(GoldKey)

		GoldKey::GoldKey(Game& game, Camera& camera, Mouse& mouse)
		: DrawableGameComponent(game, camera),
		mMaterial(nullptr), mTextureEffect(nullptr),
		mVertexBuffer(nullptr), mIndexBuffer(nullptr), mIndexCount(0),
		mTextureShaderResourceView(nullptr), mColorTextureVariable(nullptr)
	{
		mWorldMatrix = MatrixHelper::Identity;
		mMouse = &mouse;
	}

	GoldKey::~GoldKey()
	{
		ReleaseObject(mColorTextureVariable);
		ReleaseObject(mTextureShaderResourceView);
		DeleteObject(mMaterial);
		DeleteObject(mTextureEffect);
		ReleaseObject(mVertexBuffer);
		ReleaseObject(mIndexBuffer);
	}

	void GoldKey::Initialize()
	{
		SetCurrentDirectory(Utility::ExecutableDirectory().c_str());

		//Load the model
		std::unique_ptr<Model> model(new Model(*mGame, "Content\\Models\\GoldKey.obj", true));

		// Initialize the material
		mTextureEffect = new Effect(*mGame);
		mTextureEffect->CompileFromFile(L"Content\\Effects\\DiffuseLighting.fx");
		mMaterial = new DiffuseLightingMaterial();
		mMaterial->Initialize(mTextureEffect);

		Mesh* mesh = model->Meshes().at(0);
		CreateVertexBuffer(mGame->Direct3DDevice(), *mesh, &mVertexBuffer);
		mesh->CreateIndexBuffer(&mIndexBuffer);
		mIndexCount = mesh->Indices().size();

		std::wstring textureName = L"Content\\Textures\\GoldKey.png";
		HRESULT hr = DirectX::CreateWICTextureFromFile(mGame->Direct3DDevice(), mGame->Direct3DDeviceContext(), textureName.c_str(), nullptr, &mTextureShaderResourceView);
		if (FAILED(hr))
		{
			throw GameException("CreateWICTextureFromFile() failed.", hr);
		}

		mColorTextureVariable = mTextureEffect->GetEffect()->GetVariableByName("ColorTexture")->AsShaderResource();			
	}

	void GoldKey::Update(const GameTime& gameTime)
	{
		if (this->Visible())
		{
			Interaction(gameTime);
		}		
	}

	void GoldKey::Interaction(const GameTime& gameTime)//, TreasureChest* chest)
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

	void GoldKey::Draw(const GameTime& gameTime)
	{
		ID3D11DeviceContext* direct3DDeviceContext = mGame->Direct3DDeviceContext();
		direct3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Pass* pass = mMaterial->CurrentTechnique()->Passes().at(0);
		ID3D11InputLayout* inputLayout = mMaterial->InputLayouts().at(pass);
		direct3DDeviceContext->IASetInputLayout(inputLayout);

		UINT stride = sizeof(TextureMappingVertex);
		UINT offset = 0;
		direct3DDeviceContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
		direct3DDeviceContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		XMMATRIX worldMatrix = XMLoadFloat4x4(&mWorldMatrix);
		XMMATRIX wvp = worldMatrix * mCamera->ViewMatrix() * mCamera->ProjectionMatrix();
		mMaterial->WorldViewProjection() << wvp;

		mMaterial->World() << worldMatrix;
		mMaterial->ColorTexture() << mTextureShaderResourceView;
		mMaterial->att() << XMFLOAT3(0.1f, 0.01f, 0.000f);
		mMaterial->ambient() << XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		mMaterial->diffuse() << XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		mColorTextureVariable->SetResource(mTextureShaderResourceView);

		pass->Apply(0, direct3DDeviceContext);
		direct3DDeviceContext->DrawIndexed(mIndexCount, 0, 0);
	}

	void GoldKey::CreateVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.Vertices();

		std::vector<TextureMappingVertex> vertices;
		vertices.reserve(sourceVertices.size());

		std::vector<XMFLOAT3>* textureCoordinates = mesh.TextureCoordinates().at(0);
		assert(textureCoordinates->size() == sourceVertices.size());
		//generate the bounding box
		float min = -1e38f;
		float max = 1e38f;

		XMFLOAT3 vMinf3(max, max, max);
		XMFLOAT3 vMaxf3(min, min, min);


		XMVECTOR vMin = XMLoadFloat3(&vMinf3);
		XMVECTOR vMax = XMLoadFloat3(&vMaxf3);
		//end

		for (UINT i = 0; i < sourceVertices.size(); i++)
		{
			XMFLOAT3 position = sourceVertices.at(i);
			XMFLOAT3 uv = textureCoordinates->at(i);
			vertices.push_back(TextureMappingVertex(XMFLOAT4(position.x, position.y, position.z, 1.0f), XMFLOAT2(uv.x, uv.y)));

			//create the bounding box from the list of vertices
			XMVECTOR P = XMLoadFloat3(&position);
			vMin = XMVectorMin(vMin, P);
			vMax = XMVectorMax(vMax, P);
			//the end
		}
		//final step to generate the bounding box
		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Center), 0.5f*(vMin + vMax));
		XMStoreFloat3(const_cast<XMFLOAT3*>(&mBoundingBox.Extents), 0.5f*(vMax - vMin));

		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = sizeof(TextureMappingVertex) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = &vertices[0];
		if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer)))
		{
			throw GameException("ID3D11Device::CreateBuffer() failed.");
		}
	}
}