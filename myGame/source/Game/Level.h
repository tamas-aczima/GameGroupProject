#pragma once

#include "DrawableGameComponent.h"

using namespace Library;

namespace Rendering
{
	class Wall;
	class Floor;

	class Level : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Level, DrawableGameComponent)

	public:
		Level(Game& game, Camera& camera);
		~Level();

		std::vector<GameComponent*> UpdateComponent(std::vector<GameComponent*> mComponents);

	private:
		std::vector<GameComponent*> tempCompVector;
		

	};
}