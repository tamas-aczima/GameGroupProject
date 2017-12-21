#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Library
{
	class SpotLight;
}

namespace Rendering
{
	class Wall;
	class Floor;
	class Mirror;

	class Level : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Level, DrawableGameComponent)

	public:
		Level(Game& game, Camera& camera, SpotLight& spotLight1, SpotLight& spotLight2, Mirror& mirror1);
		~Level();

		std::vector<GameComponent*> UpdateComponent(std::vector<GameComponent*> mComponents);

	private:
		std::vector<GameComponent*> tempCompVector;
		

	};
}