#pragma once

#include "DrawableGameComponent.h"
#include "DirectXCollision.h"

using namespace Library;
using namespace DirectX;

namespace Library
{
	class Effect;
	class TextureMaterial;
}

namespace Rendering
{
	class Wall : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Wall, DrawableGameComponent)

	public:
		Wall(Game& game, Camera& camera);
		~Wall();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		TextureMaterial* mTextureMaterial;

		BoundingBox GetBoundingBox();

		XMFLOAT4X4* WorldMatrix() { return &mWorldMatrix; }
		
		void SetTransform(float X, float Y, float Z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);
		XMVECTOR positionVector;
		void SetPlayerLocation(XMVECTOR& player);

		float x;
		float y;
		float z;

		bool isColliding = false;

	private:
		Wall();
		Wall(const Wall& rhs);
		Wall& operator=(const Wall& rhs);

		Effect* mTextureEffect;

		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

		float Distance(const XMVECTOR& point1, const XMVECTOR& point2);
		float distance;
		XMVECTOR PlayerLocation;

		//BoundingBox mBoundingBox;
	};
}