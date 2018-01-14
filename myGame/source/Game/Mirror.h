#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Mouse;
	class Effect;
	class SpotLight;
}

namespace Rendering
{
	class DiffuseLightingMaterial;
	class Mirror : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Mirror, DrawableGameComponent)

	public:
		Mirror(Game& game, Camera& camera, Mouse& mouse, SpotLight& spotLight1, SpotLight& spotLight2, int id);
		~Mirror();

		const XMFLOAT3& Position() const;
		const XMFLOAT3& Direction() const;
		const XMFLOAT3& Up() const;
		const XMFLOAT3& Right() const;

		XMVECTOR PositionVector() const;
		XMVECTOR DirectionVector() const;
		XMVECTOR UpVector() const;
		XMVECTOR RightVector() const;

		void SetPosition(FLOAT x, FLOAT y, FLOAT z);
		void SetPosition(FXMVECTOR position);
		void SetPosition(const XMFLOAT3& position);

		void ApplyRotation(CXMMATRIX transform);
		void ApplyRotation(const XMFLOAT4X4& transform);

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

		int GetID();

	private:
		Mirror();
		Mirror(const Mirror& rhs);
		Mirror& operator=(const Mirror& rhs);

		static const XMFLOAT2 LightRotationRate;
		void Rotate(const GameTime& gameTime);

		Effect* mEffect;
		DiffuseLightingMaterial* mMaterial;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		SpotLight* mSpotLight1;
		SpotLight* mSpotLight2;
		Mouse* mMouse;

		std::wstring mTextureName;

		XMFLOAT4X4 mWorldMatrix;
		XMFLOAT4X4 mScaleMatrix;

		XMFLOAT3 mPosition;
		XMFLOAT3 mDirection;
		XMFLOAT3 mUp;
		XMFLOAT3 mRight;

		int mID;
		float mDefaultRadius;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}