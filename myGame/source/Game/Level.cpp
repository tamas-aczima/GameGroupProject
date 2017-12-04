#include "Level.h"
#include "Wall.h"
#include "Floor.h"
#include "Player.h"
#include "GoldKey.h"
#include "BronzeKey.h"
#include "Door.h"
#include "LightLock.h"
#include "TextureMaterial.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Level)

	Level::Level(Game& game, Camera& camera)
		: DrawableGameComponent(game, camera)
	{

		//Please use this way of creation objects
		//--------------------------------------

		Wall* wall = new Wall(game, camera); //backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, -5, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera);//backwall 2
		tempCompVector.push_back(wall);
		wall->Initialize();
		wall->SetPosition(17, 0, -5, 0, 0, 0, 1, 1.5, 0.1);
		wallsVect.push_back(wall->mTextureMaterial);

		wall = new Wall(game, camera);//backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(34, 0, -5, 0, 0, 0, 1, 1.5, 0.1);

		//frontwall

		wall = new Wall(game, camera);//backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-12, 0, 109, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera);//backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, 114, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera);//backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(17, 0, 114, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera);//backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(34, 0, 114, 0, 0, 0, 1, 1.5, 0.1);


		//leftwalls

		wall = new Wall(game, camera); //leftsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 3, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 12, 0, 1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(37, 0, 21, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 37, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 5
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 44, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 57, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 5
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 74, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 88, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(40, 0, 99, 0, 1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //leftsidewall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 109, 0, 1.2, 0, 1, 1.5, 0.1);

		//rightwalls

		wall = new Wall(game, camera); //rightsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-5, 0, 3, 0, -90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  2
		tempCompVector.push_back(wall);
		wall->SetPosition(-5, 0, 12, 0, -1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  3
		tempCompVector.push_back(wall);
		wall->SetPosition(-8, 0, 28.5, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  4
		tempCompVector.push_back(wall);
		wall->SetPosition(-9, 0, 37, 0, -1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  5
		tempCompVector.push_back(wall);
		wall->SetPosition(-9.6, 0, 52, 0, -1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  6
		tempCompVector.push_back(wall);
		wall->SetPosition(-13, 0, 69, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera); //rightsidewall  THICK 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-20, 0, 30, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightsidewall  THICK 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-40, 0, 140, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightsidewall  THICK 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-42, 0, 220, 0, -1.6, 0, 5, 1.5, 0.5);

		//section 2

		//rightroom

		wall = new Wall(game, camera); //Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-35, 0, 69, 0, -1.6, 0, 1, 1.5, 2.5);

		wall = new Wall(game, camera); //right Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-135, 0, 70, 0, 0, 0, 6.5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightroom backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-60, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightroom backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-145, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightroom backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-225, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //rightroom backwall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-185, 0, 10, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-145, 0, 45, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera); //right Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-90, 0, 30, 0, -1.6, 0, 5, 1.5, 0.5);

		//section 2.2

		wall = new Wall(game, camera); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 40, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //right Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 5, 0, -1.6, 0, 5, 1.5, 0.5);
		//leftroom

		wall = new Wall(game, camera); //Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-35, 0, 118, 0, -1.6, 0, 2.5, 1.5, 2.5);

		wall = new Wall(game, camera); //Left Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-135, 0, 104, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm backwall
		tempCompVector.push_back(wall);
		wall->SetPosition(-82, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-165, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-245, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm backwall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(-325, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-175, 0, 140, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-180, 0, 220, 0, -1.6, 0, 5, 1.5, 0.5);

		// section 2.2

		wall = new Wall(game, camera); //left Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 120, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 95, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera); //left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 145, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //left room
		tempCompVector.push_back(wall);
		wall->SetPosition(-275, 0, 130, 0, 0, 0, 4.5, 1.5, 0.5);

		wall = new Wall(game, camera); //left room
		tempCompVector.push_back(wall);
		wall->SetPosition(-275, 0, 190, 0, 0, 0, 4.5, 1.5, 0.5);

		// section 3 

		//inner buildings 
		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-500, 0, 140, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-500, 0, 70, 0, -1.6, 0, 4, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-530, 0, 180, 0, 0, 0, 4.5, 1.5, 2.0);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-560, 0, 140, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-560, 0, 60, 0, -1.6, 0,  5, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-560, 0, -20, 0, -1.6, 0, 5, 1.5, 0.5);

		// big wall
		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-390, 0, 160, 0, -1.6, 0, 3, 1.5, 5.5);

		//section 4 

		wall = new Wall(game, camera); //Left  backwall 
		tempCompVector.push_back(wall);

		wall->SetPosition(-565, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //Left  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-645, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //right  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-565, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //right  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-645, 0, 0, 0, 0, 0, 5, 1.5, 0.5);


		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-680, 0, 190, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-680, 0, 110, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera);
		tempCompVector.push_back(wall);
		wall->SetPosition(-680, 0, 30, 0, -1.6, 0, 5, 1.5, 0.5);


		//end walls
		wall = new Wall(game, camera); //frontwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-410, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //frontwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-490, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-395, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera); //backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-480, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		//floor

		Floor* floor = new Floor(game, camera);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(5, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(5, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		//section 2 floor

		floor = new Floor(game, camera);//floor3
		tempCompVector.push_back(floor);
		floor->SetPosition(-75, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor4
		tempCompVector.push_back(floor);
		floor->SetPosition(-155, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor5
		tempCompVector.push_back(floor);
		floor->SetPosition(-75, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor6
		tempCompVector.push_back(floor);
		floor->SetPosition(-75, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-155, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-155, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-240, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-240, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor6
		tempCompVector.push_back(floor);
		floor->SetPosition(-240, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-325, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-325, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor6
		tempCompVector.push_back(floor);
		floor->SetPosition(-325, 0, 25, 0, 0, 0, 5, 0.01, 5);

		//section 3 

		floor = new Floor(game, camera);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-410, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-410, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-410, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-490, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-490, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-490, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-570, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-570, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-570, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		//section 4

		floor = new Floor(game, camera);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-655, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-655, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-655, 0, 195.5, 0, 0, 0, 5, 0.01, 5);

		//GoldKey* goldKey = new GoldKey(game, camera);
		//goldKey->SetPosition(0, 0, 10, 0, 0, 0, 0.01, 0.01, 0.01);
		//tempCompVector.push_back(goldKey);

		//BronzeKey* bronzeKey = new BronzeKey(game, camera);
		//bronzeKey->SetPosition(0, 0, 20, 0, 0, 0, 0.01, 0.01, 0.01);
		//tempCompVector.push_back(bronzeKey);

		// first door
		Door* door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-14, 0, 94, 0, 1.57, 0, 0.1, 0.1, 0.1);

		door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-14, 20, 76, 0, 1.57, 3.14, 0.1, 0.1, 0.1);

		// second door 

		door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-240, 2, 215, 0, 1.57, 0, 0.1, 0.1, 0.1);

		door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-240, 22, 197, 0, 1.57, 3.14, 0.1, 0.1, 0.1);

		// third door
		door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-499, 2, 215, 0, 1.57, 0, 0.1, 0.1, 0.1);

		door = new Door(game, camera);
		tempCompVector.push_back(door);
		door->SetPosition(-499, 22, 197, 0, 1.57, 3.14, 0.1, 0.1, 0.1);


		LightLock* lightlock = new LightLock(game, camera);
		tempCompVector.push_back(lightlock);
		lightlock->SetPosition(-12, 12, 69, 0, 0, 0.2, 0.5, 0.5, 0.5);

		//-------------------------
		//instead of this way 

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
	std::vector<TextureMaterial*> Level::WallsVect()
	{
		return wallsVect;
	}
}