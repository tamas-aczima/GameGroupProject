#pragma once

#include "DrawableGameComponent.h"
#include "TextureMaterial.h"

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
		std::vector<TextureMaterial*> WallsVect();
		

	private:
		std::vector<GameComponent*> tempCompVector;
		std::vector<TextureMaterial*> wallsVect;

	};
}