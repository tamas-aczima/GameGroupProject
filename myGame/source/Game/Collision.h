#pragma once

#include "DrawableGameComponent.h"
#include "Wall.h"

using namespace Library;
using namespace std;

namespace Rendering
{

	class Collision : public DrawableGameComponent
	{
		RTTI_DECLARATIONS(Collision,DrawableGameComponent)

	public :

		Collision(Game& game, Camera& camera);
		~Collision();

		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void Draw(const GameTime& gameTime) override;


	private:

		vector<Wall*> walls;

		void CollisionDetection(float x, float z);
		void UpdateLists();

	};

}