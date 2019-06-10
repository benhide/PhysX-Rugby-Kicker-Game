#pragma once

#include "BasicActors.h"
#include "Game.h"

// The physics engine
namespace PhysicsEngine
{
	// Catapult class
	class Catapult
	{
	public:
		Catapult(GameScene* scene);
		~Catapult();

	private:
		// Kick joint
		RevoluteJoint* kickJoint;
		Box* base;
		Box* kick;
	};
}