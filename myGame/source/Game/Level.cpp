#include "Level.h"
#include "Wall.h"
#include "Lava.h"
#include "Floor.h"
#include "Player.h"
#include "GoldKey.h"
#include "BronzeKey.h"
#include "Door.h"
#include "BasicDoor.h"
#include "LightLock.h"
#include "TreasureChest.h"
#include "LightLockDiffuseLight.h"
#include "Mirror.h"
#include "Mouse.h"

namespace Rendering
{
	RTTI_DEFINITIONS(Level)
	
	Level::Level(Game& game, Camera& camera, SpotLight& spotLight1, SpotLight& spotLight2, SpotLight& spotLight3,
		SpotLight& spotLight4, SpotLight& spotLight5, SpotLight& spotLight6, Mirror& mirror1, Mirror& mirror2, Mirror& mirror3, Mirror& mirror4,
		TreasureChest& chest1)
		: DrawableGameComponent(game, camera)
	{

		//Please use this way of creation objects
		//--------------------------------------

		Wall* wall = new Wall(game, camera, spotLight2); //backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, -5, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2);//backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(17, 0, -5, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2);//backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(34, 0, -5, 0, 0, 0, 1, 1.5, 0.1);

		//frontwall

		wall = new Wall(game, camera, spotLight2);//backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-12, 0, 109, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2);//backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(0, 0, 114, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2);//backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(17, 0, 114, 0, 0, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2);//backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(34, 0, 114, 0, 0, 0, 1, 1.5, 0.1);


		//leftwalls

		wall = new Wall(game, camera, spotLight2); //leftsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 3, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 12, 0, 1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(37, 0, 21, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 37, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 5
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 44, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight1); //leftsidewall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 57, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight1); //leftsidewall 5
		tempCompVector.push_back(wall);
		wall->SetPosition(34.5, 0, 74, 0, 1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 88, 0, 90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(40, 0, 99, 0, 1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //leftsidewall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(38, 0, 109, 0, 1.2, 0, 1, 1.5, 0.1);

		//rightwalls

		wall = new Wall(game, camera, spotLight2); //rightsidewall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-5, 0, 3, 0, -90, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  2
		tempCompVector.push_back(wall);
		wall->SetPosition(-5, 0, 12, 0, -1.57, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  3
		tempCompVector.push_back(wall);
		wall->SetPosition(-8, 0, 28.5, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  4
		tempCompVector.push_back(wall);
		wall->SetPosition(-9, 0, 37, 0, -1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  5
		tempCompVector.push_back(wall);
		wall->SetPosition(-9.6, 0, 52, 0, -1.6, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  6
		tempCompVector.push_back(wall);
		wall->SetPosition(-13, 0, 69, 0, 1.2, 0, 1, 1.5, 0.1);

		wall = new Wall(game, camera, spotLight2); //rightsidewall  THICK 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-20, 0, 30, 0, -1.6, 0, 5, 1.5, 0.5);

		//section 2

		//rightroom

		wall = new Wall(game, camera, spotLight2); //Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-35, 0, 69, 0, -1.6, 0, 1, 1.5, 2.5);

		wall = new Wall(game, camera, spotLight5); //right Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-135, 0, 70, 0, 0, 0, 6.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //rightroom backwall 1
		tempCompVector.push_back(wall);
		wall->SetPosition(-60, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //rightroom backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-145, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight4); //rightroom backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-225, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //rightroom backwall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-145, 0, 45, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-90, 0, 30, 0, -1.6, 0, 5, 1.5, 0.5);

		//leftroom

		wall = new Wall(game, camera, spotLight2); //right Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-35, -42, 118, 0, -1.6, 0, 2.5, 4, 2.5);

		wall = new Wall(game, camera, spotLight5); //right Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-140, -45, 114, 0, 0, 0, 5, 4, 2);

		wall = new Wall(game, camera, spotLight2); //right Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-40, -45, 180, 0, -1.6, 0, 5, 4, 0.5);

		wall = new Wall(game, camera, spotLight2); //Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-42, -45, 265.5, 0, -1.6, 0, 5, 4, 0.5);

		wall = new Wall(game, camera, spotLight2); //Left Entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-82, -45, 280, 0, 0, 0, 5, 4, 0.5);

		wall = new Wall(game, camera, spotLight2); //Left Roomm backwall
		tempCompVector.push_back(wall);
		wall->SetPosition(-166.3, -45, 280, 0, 0, 0, 5, 4, 0.5);

		wall = new Wall(game, camera, spotLight5); //Left Roomm backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-165, -45, 150, 0, -1.6, 0, 5, 4, 0.5);

		wall = new Wall(game, camera, spotLight2); //Left Roomm backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-167, -45, 235.5, 0, -1.6, 0, 5, 4, 0.5);

		//section 2.2

		wall = new Wall(game, camera, spotLight4); //Left Roomm backwall 4
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 40, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight5); //Left Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-185, 0, 25, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight4); //Left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 20, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //right room entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-256, 0, 40, 0, 0, 0, 1.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //left Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-294, 0, 40, 0, 0, 0, 1.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight4); //left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 95, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight6); //left Roomm
		tempCompVector.push_back(wall);
		wall->SetPosition(-240, 0, 145, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //left room
		tempCompVector.push_back(wall);
		wall->SetPosition(-275, 0, 130, 0, 0, 0, 4.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight4); //left room
		tempCompVector.push_back(wall);
		wall->SetPosition(-256, 0, 80, 0, 0, 0, 1.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight4); //left room entrance wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-294, 0, 80, 0, 0, 0, 1.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight6); //left room
		tempCompVector.push_back(wall);
		wall->SetPosition(-277, 0, 190, 0, 0, 0, 4.5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //left Roomm 
		tempCompVector.push_back(wall);
		wall->SetPosition(-310, 0, 120, 0, -1.6, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //Left Roomm backwall 3
		tempCompVector.push_back(wall);
		wall->SetPosition(-210, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		// section 3 

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-295, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-380, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-463.2, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-395, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-480, 0, 0, 0, 0, 0, 5, 1.5, 0.5);

		//inner buildings 

		#pragma region InnerBuildings

		/*wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-500, 0, 85, 0, -1.6, 0, 3, 1.5, 0.5);*/

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-502, 0, 135.5, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); // Devider Wall
		tempCompVector.push_back(wall);
		wall->SetPosition(-470, 0, 15.5, 0, -1.6, 0, 3, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2); //Left  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-530, 0, 177, 0, 0, 0, 4.5, 1.5, 2.0);

		wall = new Wall(game, camera, spotLight2); //Left  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-550, -60, 32.8, 0, -1.6, 0,  5, 5, 0.5);

		wall = new Wall(game, camera, spotLight2); //right  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-552, -60, 118.2, 0, -1.6, 0, 5, 5, 0.5);

		// big wall
		wall = new Wall(game, camera, spotLight2); //right  backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-390, 0, 145, 0, -1.6, 0, 3, 1.5, 5.5);

		#pragma endregion
		//section 4 

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-548.6, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-634, 0, 220, 0, 0, 0, 5, 1.5, 0.5);

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-565, -60, 0, 0, 0, 0, 5, 5, 0.5);

		wall = new Wall(game, camera, spotLight2); //frontwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-650, -60, 0, 0, 0, 0, 5, 5, 0.5);

		wall = new Wall(game, camera, spotLight2); //frontwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-670, -60, 42.8, 0, -1.6, 0, 5, 5, 0.5);

		wall = new Wall(game, camera, spotLight2); //backwall 
		tempCompVector.push_back(wall);
		wall->SetPosition(-672, -60, 128.2, 0, -1.6, 0, 5, 5, 0.5);

		wall = new Wall(game, camera, spotLight2); //backwall 2
		tempCompVector.push_back(wall);
		wall->SetPosition(-674, -60, 213.8, 0, -1.6, 0, 5, 5, 0.5);

		//floor

		#pragma region Floors


		Floor* floor = new Floor(game, camera, spotLight2);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(5, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight2);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(5, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		//section 2 floor

		#pragma region Section2

		//85.4 increments
		floor = new Floor(game, camera, spotLight1);//floor3
		tempCompVector.push_back(floor);
		floor->SetPosition(-80.5, 0, 25, 0, 0, 0, 5, 0.01, 5);
		
		floor = new Floor(game, camera, spotLight1);//floor5
		tempCompVector.push_back(floor);
		floor->SetPosition(-80.5, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor5
		tempCompVector.push_back(floor);
		floor->SetPosition(-105, -17, 110.5, 0, 0, 0, 7, 1, 5);

		#pragma region LavaLevel

		wall = new Wall(game, camera, spotLight2);
		tempCompVector.push_back(wall);
		wall->SetPosition(-50, -90, 10, 0, 0, 0, 14, 5, 0.5);

		//pillers
		floor = new Floor(game, camera, spotLight1);//pillar
		tempCompVector.push_back(floor);
		floor->SetPosition(-80.5, -85.5, 160.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 2
		tempCompVector.push_back(floor);
		floor->SetPosition(-90.5, -85.5, 170.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 3
		tempCompVector.push_back(floor);
		floor->SetPosition(-100.5, -85.5, 180.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 4
		tempCompVector.push_back(floor);
		floor->SetPosition(-110.5, -85.5, 190.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 5
		tempCompVector.push_back(floor);
		floor->SetPosition(-100.5, -85.5, 200.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 6
		tempCompVector.push_back(floor);
		floor->SetPosition(-90.5, -85.5, 210.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 7
		tempCompVector.push_back(floor);
		floor->SetPosition(-90.5, -85.5, 220.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 8
		tempCompVector.push_back(floor);
		floor->SetPosition(-90.5, -85.5, 230.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 8
		tempCompVector.push_back(floor);
		floor->SetPosition(-100.5, -85.5, 235.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//DummyPillar
		tempCompVector.push_back(floor);
		floor->SetPosition(-80.5, -85.5, 190.5, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//DummyPillar
		tempCompVector.push_back(floor);
		floor->SetPosition(-126.5, -85.5, 215, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//Platform
		tempCompVector.push_back(floor);
		floor->SetPosition(-100, -85.5, 260, 0, 0, 0, 4, 5, 2);

		Lava* lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-80.5, -40, 116, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-80.5, -40, 30.5, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(5, -40, 116, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(5, -40, 30.5, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-80.5, -40, 201.5, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-80.5, -40, 287, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-166, -40, 116, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-166, -40, 201.5, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-166, -40, 287, 0, 0, 0, 5, 0.01, 5);

		#pragma endregion

		floor = new Floor(game, camera, spotLight1);//floor4
		tempCompVector.push_back(floor);
		floor->SetPosition(-165.9, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-165.9, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-210.9, 0, 196, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor6
		tempCompVector.push_back(floor);
		floor->SetPosition(-251.3, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-251.3, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-296.3, 0, 196, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-336.7, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor7
		tempCompVector.push_back(floor);
		floor->SetPosition(-381.7, 0, 196, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor6
		tempCompVector.push_back(floor);
		floor->SetPosition(-336.7, 0, 25, 0, 0, 0, 5, 0.01, 5);

#pragma endregion

		//section 3 

		#pragma region Section3
		floor = new Floor(game, camera, spotLight1);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-422.1, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-422.1, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-467.1, 0, 196, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor2
		tempCompVector.push_back(floor);
		floor->SetPosition(-507.5, 0, 25, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor8
		tempCompVector.push_back(floor);
		floor->SetPosition(-507.5, 0, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-552.5, -85.5, 196, 0, 0, 0, 5, 5, 5);

#pragma endregion

		//section 4
		#pragma region Section4

	#pragma region Lava
		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-592.9, -60, 25, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-592.9, -60, 110.5, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-678.3, -60, 25, 0, 0, 0, 5, 0.01, 5);

		lava = new Lava(game, camera);//  LAVA
		tempCompVector.push_back(lava);
		lava->SetPosition(-678.3, -60, 110.5, 0, 0, 0, 5, 0.01, 5);

		floor = new Floor(game, camera, spotLight1);//pillar Finish
		tempCompVector.push_back(floor);
		floor->SetPosition(-610, -85.5, 12, 0, 0, 0, 3, 5, 1);

		floor = new Floor(game, camera, spotLight1);//pillar final to finish
		tempCompVector.push_back(floor);
		floor->SetPosition(-610, -85.5, 35, 0, 0, 0, 0.6, 5, 1.6);

		floor = new Floor(game, camera, spotLight1);//pillar
		tempCompVector.push_back(floor);
		floor->SetPosition(-650, -85.5, 55, 0, 0, 0, 0.7, 5, 0.7);
		
		floor = new Floor(game, camera, spotLight1);//pillar 2
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 65, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 3
		tempCompVector.push_back(floor);
		floor->SetPosition(-630, -85.5, 75, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 4
		tempCompVector.push_back(floor);
		floor->SetPosition(-620, -85.5, 65, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 5
		tempCompVector.push_back(floor);
		floor->SetPosition(-610, -85.5, 55, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 6
		tempCompVector.push_back(floor);
		floor->SetPosition(-630, -85.5, 85, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 7
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 95, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 8
		tempCompVector.push_back(floor);
		floor->SetPosition(-650, -85.5, 105, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar fake
		tempCompVector.push_back(floor);
		floor->SetPosition(-600, -85.5, 65, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar fake
		tempCompVector.push_back(floor);
		floor->SetPosition(-610, -85.5, 85, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar fake
		tempCompVector.push_back(floor);
		floor->SetPosition(-570, -85.5, 95, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar fake
		tempCompVector.push_back(floor);
		floor->SetPosition(-600, -85.5, 105, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 115, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-630, -85.5, 125, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-620, -85.5, 135, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-630, -85.5, 125, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 135, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 135, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 145, 0, 0, 0, 0.4, 5, 0.4);

		floor = new Floor(game, camera, spotLight1);//pillar 9
		tempCompVector.push_back(floor);
		floor->SetPosition(-640, -85.5, 145, 0, 0, 0, 0.4, 5, 0.4);


	#pragma endregion
		floor = new Floor(game, camera, spotLight1);//floor1
		tempCompVector.push_back(floor);
		floor->SetPosition(-637.9, -85.5, 196, 0, 0, 0, 5, 5, 5);

#pragma endregion
		#pragma endregion

		//basic door

		#pragma region Doors


		//right
		BasicDoor* basicdoor = new BasicDoor(game, camera);
		tempCompVector.push_back(basicdoor);
		basicdoor->SetPosition(-275, 0, 40, 0, 1.57, 0, 4, 3.2, 4);
		//left
		basicdoor = new BasicDoor(game, camera);
		tempCompVector.push_back(basicdoor);
		basicdoor->SetPosition(-275, 0, 80, 0, 1.57, 0, 4, 3.2, 4);

		// first door
		Door* door1 = new Door(game, camera);
		tempCompVector.push_back(door1);
		door1->SetPosition(-14, 0, 94, 0, 1.57, 0, 0.1, 0.1, 0.1);

		Door* door2 = new Door(game, camera);
		tempCompVector.push_back(door2);
		door2->SetPosition(-14, 20, 76, 0, 1.57, 3.14, 0.1, 0.1, 0.1);

		// second door 

		Door* door3 = new Door(game, camera);
		tempCompVector.push_back(door3);
		door3->SetPosition(-240, 2, 215, 0, 1.57, 0, 0.1, 0.1, 0.1);

		Door* door4 = new Door(game, camera);
		tempCompVector.push_back(door4);
		door4->SetPosition(-240, 22, 197, 0, 1.57, 3.14, 0.1, 0.1, 0.1);

		// third door
		Door* door5 = new Door(game, camera);
		tempCompVector.push_back(door5);
		door5->SetPosition(-499, 2, 215, 0, 1.57, 0, 0.1, 0.1, 0.1);

		Door* door6 = new Door(game, camera);
		tempCompVector.push_back(door6);
		door6->SetPosition(-499, 22, 197, 0, 1.57, 3.14, 0.1, 0.1, 0.1);

		//final door
		Door* door7 = new Door(game, camera);
		tempCompVector.push_back(door7);
		door7->SetPosition(-618, 2, 5, 0, 0, 0, 0.1, 0.1, 0.1);

		Door* door8 = new Door(game, camera);
		tempCompVector.push_back(door8);
		door8->SetPosition(-600, 22, 5, 0, 0, 3.14, 0.1, 0.1, 0.1);


#pragma endregion

		// lightlocks

		#pragma region Light

		//light source1
		LightLockDiffuseLight* lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 2.34, 1.57, 0.5, 0, 10, 36);

		//lock1
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2, mirror1, *door1, *door2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 0, 1.57, 0.5, -12.5, 12, 73);

		//light source2
		lightLock = new LightLockDiffuseLight(game, camera, spotLight4);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 2.34, 1.57, 0.5, -225, 12, 10);

		//lock2
		lightLock = new LightLockDiffuseLight(game, camera, spotLight6, mirror4, *door3, *door4);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 0, 1.57, 0.5, -235.5, 12, 180);

		//Source 3
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1, 1.57, 0.5, -495, 12, 150);

		//lock
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1, 1.57, 0.5, -544, 12, 150);

		//Mirror
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1.57, 1.57, 0.5, -430, 12, 100);

		//mirror 3.2
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1.57, 1.57, 0.5, -440, 12, 15);

		//mirror 3.3
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1.57, 1.57, 0.5, -505, 12, 15);

		// lock 4
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, -1, 1.57, 0.5, -561, 12, 150);

		// Mirror 4
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1.57, 1.57, 0.5, -650, 12, 55);

		// Source 4
		lightLock = new LightLockDiffuseLight(game, camera, spotLight2);
		tempCompVector.push_back(lightLock);
		lightLock->SetPosition(0, 1.57, 1.57, 0.5, -650, 12, 214);
#pragma endregion


		// Treasure Chests
		#pragma region Chest

		//TreasureChest* chest = new TreasureChest(game, camera);
		//tempCompVector.push_back(chest);
		//chest->SetPosition(12, 0, 5, 0, 3.14, 0, 3.0, 3.0, 3.0);



#pragma endregion


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