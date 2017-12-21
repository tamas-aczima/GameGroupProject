#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class Effect;
	class TextureMaterial;
}

namespace Rendering
{
	class BasicDoor : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(BasicDoor, DrawableGameComponent)

	public:
		BasicDoor(Game& game, Camera& camera);
		~BasicDoor();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;

	private:
		BasicDoor();
		BasicDoor(const BasicDoor& rhs);
		BasicDoor& operator=(const BasicDoor& rhs);

		Effect* mTextureEffect;
		TextureMaterial* mTextureMaterial;
		XMCOLOR mAmbientColor;

		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mIndexCount;

		std::wstring mTextureName;

		ID3D11ShaderResourceView* mTextureShaderResourceView;
		ID3DX11EffectShaderResourceVariable* mColorTextureVariable;
	};
}

