#include "Catapult.h"


// The physics engine
namespace PhysicsEngine
{
	// COnstructor
	Catapult::Catapult(GameScene* scene)
	{
		base = new Box(PxTransform(PxVec3(0.0f, 1.0f, 25.0f)), PxVec3(0.25f, 0.25f, 0.25f));
		base->Color(PxVec3(0.0f, 0.0f, 1.0f));
		base->SetKinematic(true);
		base->Name("KickBase");
		scene->Add(base);

		kick = new Box(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)), PxVec3(0.1f, 0.25f, 0.1f));
		kick->Color(PxVec3(0.0f, 0.0f, 1.0f));
		kick->Name("Kick");
		scene->Add(kick);

		kickJoint = new RevoluteJoint(base, PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxPi * 2, PxVec3(0.0f, 0.0f, 1.0f))), kick, PxTransform(PxVec3(0.0f, 0.0f, 0.5f)));
		kickJoint->DriveVelocity(10.0f);
	}

	// Destructor
	Catapult::~Catapult()
	{
		//delete base;
		//base = NULL;

		//delete kick;
		//kick = NULL;

		//delete kickJoint;
		//kickJoint = NULL;
	}
}
