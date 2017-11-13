#include "Level.h"
#include "Wall.h"
#include "Player.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Level)
	
	Level::Level(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera)
	{

		//Please use this way of creation objects
		//--------------------------------------

		Wall* wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, -5, 0, 0, 0, 1, 0.5, 0.1);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(25, 0, 20, 0, 1.57, 0, 1, 0.5, 0.1);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-25, 0, 20, 0, 1.57, 0, 1, 0.5, 0.1);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, 45, 0, 0, 0, 1, 0.5, 0.1);


		//-------------------------
		//instead of this way 


		/*Wall* mWall1 = new Wall(game, *mCamera);
		tempCompVector.push_back(mWall1);
		mWall1->SetPosition(0, 0, -5, 0, 0, 0, 1, 0.5, 0.1);

		Wall* mWall2 = new Wall(game, *mCamera);
		tempCompVector.push_back(mWall2);
		mWall2->SetPosition(25, 0, 20, 0, 1.57, 0, 1, 0.5, 0.1);

		Wall* mWall3 = new Wall(game, *mCamera);
		tempCompVector.push_back(mWall3);
		mWall3->SetPosition(-25, 0, 20, 0, 1.57, 0, 1, 0.5, 0.1);

		Wall* mWall4 = new Wall(game, *mCamera);
		tempCompVector.push_back(mWall4);
		mWall4->SetPosition(0, 0, 45, 0, 0, 0, 1, 0.5, 0.1);*/
	}

	Level::~Level()
	{
	}

	std::vector<GameComponent*> Level::UpdateComponent(std::vector<GameComponent*> mComponents)
	{
		for (int i = 0; i < tempCompVector.size(); i++)
		{
			mComponents.push_back(tempCompVector[i]);
		}

		return mComponents;
	}
}