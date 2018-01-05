#pragma once

#include "DrawableGameComponent.h"
#include "TextureMaterial.h"

using namespace Library;
using namespace std;

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

		virtual void Update(const GameTime& gameTime) override;

		std::vector<GameComponent*> UpdateComponent(std::vector<GameComponent*> mComponents);
		
		void UpdatePlayerLocation(XMVECTOR& playerLocation, const GameTime & gameTime);
		
		bool collision = false;
		
		//Delete!
		std::vector<GameComponent*> WallsVect();
		
		std::vector<Wall*> wallsList;

		Wall* wall2;
		Wall* wall3;
		//--------------

	private:

		vector<GameComponent*> tempCompVector;
		
		int i = 0;
		

	};
}