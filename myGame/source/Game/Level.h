#pragma once

#include "DrawableGameComponent.h"

using namespace Library;
using namespace std;

namespace Library
{
	class SpotLight;
}

namespace Rendering
{
	class Wall;
	class Floor;
	class Mirror;
	class TreasureChest;

	class Level : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Level, DrawableGameComponent)

	public:
		Level(Game& game, Camera& camera, SpotLight& spotLight1, SpotLight& spotLight2, SpotLight& spotLight3, 
			SpotLight& spotLight4, SpotLight& spotLight5, SpotLight& spotLight6, SpotLight& spotLight7, SpotLight& spotLight8,
			SpotLight& spotLight9, SpotLight& spotLight10, Mirror& mirror1, Mirror& mirror2, Mirror& mirror3, Mirror& mirror4, 
			Mirror& mirror5, Mirror& mirror6, Mirror& mirror7, TreasureChest& chest1);
		~Level();

		std::vector<GameComponent*> UpdateComponent(std::vector<GameComponent*> mComponents);

		std::vector<Wall*> wallsList;

	private:
		std::vector<GameComponent*> tempCompVector;
	};
}