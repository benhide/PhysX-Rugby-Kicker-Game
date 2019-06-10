#pragma once

#include "Actors.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h> 
#include <time.h> 

// The physics engine
namespace PhysicsEngine
{
	// Using the namespace
	using namespace std;

	// Game variables
	static bool playing		= false;
	static int score		= 0;
	static int power		= 50;
	static int balls		= 20;

	// List of colours
	static const PxVec3 color_palette[] =
	{ 
		PxVec3(255.0f / 255.0f, 0.0f   / 255.0f, 0.0f   / 255.0f), // Red		(0)
		PxVec3(0.0f   / 255.0f, 255.0f / 255.0f, 0.0f   / 255.0f), // Green		(1)
		PxVec3(0.0f   / 255.0f, 0.0f   / 255.0f, 255.0f / 255.0f), // Blue		(2)
		PxVec3(255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f), // White		(3)
		PxVec3(0.0f   / 255.0f, 0.0f   / 255.0f, 0.0f   / 255.0f), // Black		(4)
		PxVec3(128.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f), // Grey		(5)
		PxVec3(102.0f / 255.0f, 51.0f  / 255.0f, 0.0f   / 255.0f), // Wood		(6)
	};

	// Filter groups
	struct FilterGroup
	{
		enum Enum
		{
			RED_BALL		= (1 << 0),
			BLUE_BALL		= (1 << 1),
			CASTLE_TARGET_1	= (1 << 2),
			CASTLE_TARGET_2 = (1 << 3),
			CASTLE_TARGET_3 = (1 << 4),
			CASTLE_TARGET_4 = (1 << 5),
		};
	};

	// A customised collision class, implemneting various callbacks
	class SimulationEventCallback : public PxSimulationEventCallback
	{
	public:
		// Variables that will be checked in the main simulation loop
		bool castleTrigger1;
		bool castleTrigger2;
		bool castleTrigger3;
		bool castleTrigger4;
		bool score;

		// Targets hit flags
		bool targethit1Red  = false;
		bool targethit3Red  = false;
		bool targethit2Blue = false;
		bool targethit4Blue = false;

		// Simulation event callback
		SimulationEventCallback() : castleTrigger1(false), castleTrigger2(false), castleTrigger3(false), castleTrigger4(false), score(false) {}

		// Method called when the contact with the trigger object is detected.
		virtual void onTrigger(PxTriggerPair* pairs, PxU32 count);

		// Method called when the contact by the filter shader is detected.
		virtual void onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs);

		// Virtual functions
		virtual void onConstraintBreak(PxConstraintInfo *constraints, PxU32 count) {}
		virtual void onWake(PxActor **actors, PxU32 count) {}
		virtual void onSleep(PxActor **actors, PxU32 count) {}
	};

	// A simple filter shader based on PxDefaultSimulationFilterShader - without group filtering
	static PxFilterFlags CustomFilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
	{
		// let triggers through
		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT;
		pairFlags |= PxPairFlag::eCCD_LINEAR;
		pairFlags |= PxPairFlag::eSOLVE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_DISCRETE_CONTACT;
		pairFlags |= PxPairFlag::eDETECT_CCD_CONTACT;


		// Trigger the contact callback for pairs (A,B) where 
		// the filtermask of A contains the ID of B and vice versa.
		if ((filterData0.word0 & filterData1.word1) && (filterData1.word0 & filterData0.word1))
		{
			// Trigger onContact callback for this pair of objects
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= PxPairFlag::eNOTIFY_TOUCH_LOST;
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
		}

		return PxFilterFlags();
	}

	// Custom scene class
	class GameScene : public Scene
	{
	public:
		// Plane
		Plane* plane;

		// Pitch
		PitchLines* pitchLines;
		Box* pitchTopLeft;
		Box* pitchTopRight;
		Box* pitchBottomLeft;
		Box* pitchBottomRight;
		Box* pitchTopGoal;
		Box* pitchBottomGoal;

		// Posts
		Post* posts;

		// Wall
		Wall* wall;

		// Ramps
		Ramp* ramp1;
		Ramp* ramp2;

		// Materials
		PxMaterial* longGrassMaterial;
		PxMaterial* shortGrassMaterial;
		PxMaterial* woodMaterial;
		PxMaterial* leatherMaterial;
		PxMaterial* rubberMaterial;
		PxMaterial* concreteMaterial;

		// Kicking machine
		RevoluteJoint* kickJoint;
		RevoluteJoint* wheelJointFL;
		RevoluteJoint* wheelJointFR;
		RevoluteJoint* wheelJointBL;
		RevoluteJoint* wheelJointBR;
		Kicker* kicker;
		KickerBase* kickerBase;
		Wheel* wheelFL;
		Wheel* wheelFR;
		Wheel* wheelBL;
		Wheel* wheelBR;
		bool forward;
		bool backward;
		bool left;
		bool right;
		float speed = 7.0f;

		// Draw bridge
		RevoluteJoint* drawBridgeJoint;
		Box* drawBridge;
		Drawbridge* bridge;

		// Target box
		Target* target;

		// Flags
		Cloth* flag;
		Box* flagPole;

		// Trampolines
		Trampoline* trampoline1;
		Trampoline* trampoline2;
		Player* bouncer1;
		Player* bouncer2;

		// Goal collision triggerr
		Box* goalCollisionShape;

		// Simulation Event Callback
		SimulationEventCallback* collisionCallback;

		// Castles
		vector<Box*> castle1;
		vector<Box*> castle2;
		vector<Box*> castle3;
		vector<Box*> castle4;
		vector<Box*> castleTargets;
		vector<Box*> castleTriggers;
		vector<RevoluteJoint*> targetJoints;
		int castleIndex = 1;
		int castlesDestroyed = 0;
		bool castleDestroyed = false;

		// Balls
		vector<Ball*> ball;

		// Players
		vector<Player*> playersRed;
		int numberOfPlayers = 9;

		// Spinners
		Box* spinnerBox;
		Box* spinnerBase;
		RevoluteJoint* spinner;

		// Fireworks
		vector<Box*> fireworks1;
		vector<Box*> fireworks2;

		// Cannons
		Cannon* cannon1;
		Cannon* cannon2;
		Sphere* bullet1;
		Sphere* bullet2;

		// Constructor
		GameScene() : Scene(CustomFilterShader) {};

		// A custom scene class
		void SetVisualisation();

		// Custom scene initialisation
		virtual void CustomInit();

		// Custom update function
		virtual void CustomUpdate(PxReal dt);

		// Get the score
		int Score();

		// Get the shot power
		float Power();

		// Get the ball count
		int Balls();

		// Get the objects count
		int Objects();

		// Get the castle destroyed count
		int DestroyedCastles();

		// Key release handling
		void KeyReleaseHandler(int key);

		// Key pressed handling
		void KeyPressHandler(int key);

		// Key hold handling
		void KeyHoldHandler(int key);

		// Build a castle
		void BuildCastle(float xOffset, float zOffset, float targetOffset, PxVec3 colour, vector<Box*>& castle);

		// Build a castle
		void BuildTeams(float zOffset);

		// Build the pitch
		void BuildPitch();

		// Build the pitch
		void BuildGoalCastle();

		// Build the kickers
		void BuildKickers(float xOffset, float zOffset);

		// Set the balls
		void SetBalls(int balls);

		// New the ball
		void NewBall();

		// Set ball pose
		void SetBallPose();

		// Destroy a castle
		void DestroyCastle(vector<Box*>& castle);

		// Set spinners
		void SetSpinners(float xOffset, float zOffset, float height, float drive);

		// Set the trampolines
		void SetTrampolines(float xOffset, float zOffset);

		// Set the fireworks
		void SetFireWorks(int count, float xOffset, float zOffset);

		// Fire the fireworks
		void FireWorks();

		// Set the cannons
		void SetCannons(float xOffset, float zOffset);

		// Set the cannons
		void FireCannons();

		// Max kick power
		int maxPower		 = 75;
		bool kicked			 = true;
		bool allowMovement   = false;

		// Flags
		bool followPlayer = true;

		// Timers
		float bridgeTimer			= 0.0f;
		float initialBridgeTimer	= 0.0f;
		float kickTimer				= 0.0f;
		float kickResetTimer		= 0.0f;
		float trampTimer			= 0.0f;
		float fireworkTimer			= 0.0f;
		float bouncerTimer			= 0.0f;
		float fireTimer				= 0.0f;
		bool newBall				= true;
		bool bridgeSet				= false;
		bool fired					= true;

		// Object count
		int objects = 0;
	};
}