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
	class PlayerBoundingBox : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(PlayerBoundingBox, DrawableGameComponent)

	public:
		PlayerBoundingBox(Game& game, Camera& camera);
		
		~PlayerBoundingBox();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		TextureMaterial* mTextureMaterial;

		BoundingBox mBoundingBox;

		float x;
		float y;
		float z;
		
		void SetTransform(float X, float Y, float Z, float rotX, float rotY, float rotZ, float scaleX, float scaleY, float scaleZ);


	private:
		PlayerBoundingBox();
		PlayerBoundingBox& operator=(const PlayerBoundingBox& rhs);

		Effect* mTextureEffect;

		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;

	};
}

