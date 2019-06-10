#include "Game.h"

// The physics engine
namespace PhysicsEngine
{
	// Method called when the contact with the trigger object is detected.
	void SimulationEventCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
	{
		// You can read the trigger information here
		for (PxU32 i = 0; i < count; i++)
		{
			// Filter out contact with the planes
			if (pairs[i].otherShape->getGeometryType() != PxGeometryType::ePLANE)
			{
				// Check if eNOTIFY_TOUCH_FOUND trigger
				if (pairs[i].status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					//cerr << "onTrigger::eNOTIFY_TOUCH_FOUND : " << pairs[i].triggerShape->getActor()->getName() << " : " << pairs[i].otherShape->getActor()->getName() << endl;
					//score = true;

					string trigger = pairs[i].triggerShape->getActor()->getName();
					string other = pairs[i].otherShape->getActor()->getName();

					// Check for castle collision
					if (trigger == "TriggerBox_inv0" && other == "TargetBox0")
					{
						castleTrigger1 = true;
					}

					// Check for castle collision
					if (trigger == "TriggerBox_inv1" && other == "TargetBox1")
					{
						castleTrigger2 = true;
					}
				
					// Check for castle collision
					if (trigger == "TriggerBox_inv2" && other == "TargetBox2")
					{
						castleTrigger3 = true;
					}

					// Check for castle collision
					if (trigger == "TriggerBox_inv3" && other == "TargetBox3")
					{
						castleTrigger4 = true;
					}

					// Goal scored
					if (trigger == "GoalTrigger_inv" && (other == "Red_Ball" || other == "Blue_Ball"))
					{
						score = true;
					}
				}
			}
		}
	}

	// Method called when the contact by the filter shader is detected.
	void SimulationEventCallback::onContact(const PxContactPairHeader &pairHeader, const PxContactPair *pairs, PxU32 nbPairs)
	{
		// Check all pairs
		for (PxU32 i = 0; i < nbPairs; i++)
		{
			// Collisions set up for different castle targets
			switch (pairs[i].shapes[0]->getSimulationFilterData().word0)
			{
			case FilterGroup::RED_BALL:
				switch (pairs[i].shapes[1]->getSimulationFilterData().word0)
				{
				case FilterGroup::CASTLE_TARGET_1:
					targethit1Red = true;
					break;
				case FilterGroup::CASTLE_TARGET_3:
					targethit3Red = true;
					break;
				}
				break;
			case FilterGroup::BLUE_BALL:
				switch (pairs[i].shapes[1]->getSimulationFilterData().word0)
				{
				case FilterGroup::CASTLE_TARGET_2:
					targethit2Blue = true;
					break;
				case FilterGroup::CASTLE_TARGET_4:
					targethit4Blue = true;
					break;
				}
				break;
			}
		}
	}

	// A custom scene class
	void GameScene::SetVisualisation()
	{
		px_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eJOINT_LIMITS, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.0f);

		// Cloth visualisation
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_HORIZONTAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_VERTICAL, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_BENDING, 1.0f);
		px_scene->setVisualizationParameter(PxVisualizationParameter::eCLOTH_SHEARING, 1.0f);
	}

	// Custom scene initialisation
	void GameScene::CustomInit()
	{
		// Physics materials
		longGrassMaterial	= CreateMaterial(0.75f, 0.75f, 0.25f);	// Long grass
		shortGrassMaterial	= CreateMaterial(0.55f, 0.55f, 0.45f);	// Short grass
		woodMaterial		= CreateMaterial(0.62f, 0.48f, 0.27f);	// Wood (oak)		
		leatherMaterial		= CreateMaterial(0.12f, 0.52f, 0.46f);	// Leather
		rubberMaterial		= CreateMaterial(0.61f, 0.75f, 0.82f);	// Rubber
		concreteMaterial	= CreateMaterial(0.80f, 1.00f, 0.14f);	// Conerete

		// Random seed
		srand(time(NULL));

		// Set visulisation
		SetVisualisation();
		collisionCallback = new SimulationEventCallback();
		px_scene->setSimulationEventCallback(collisionCallback);
		px_scene->setFlag(PxSceneFlag::eENABLE_CCD, true);

		// Build the pitch
		BuildPitch();

		// Castles
		BuildCastle(-25.0f, -40.0f, 3.0f, color_palette[0], castle1);
		BuildCastle(-35.0f, -20.0f, 3.0f, color_palette[2], castle2);
		BuildCastle(20.0f,  -40.0f, 3.0f, color_palette[0], castle3);
		BuildCastle(30.0f,  -20.0f, 3.0f, color_palette[2], castle4);

		// Build the goal castle
		BuildGoalCastle();

		// Buid the teams
		BuildTeams(5.0f);

		// Build the kickers
		BuildKickers(0.0f, 20.0f);

		// Set the trampolines
		SetTrampolines(55.0f, -20.0f);

		// Set the balls
		SetBalls(balls);

		// Set spinnsers
		SetSpinners(-5.0f, -20.0f, 4.0f, -0.25f);
		SetSpinners(5.0f, -45.0f, 12.0f, 0.5f);

		// Set the fireworks
		SetFireWorks(20, 20.0f, -110.0f);	

		// Set the cannons
		SetCannons(52.0f, -76.0f);
	}

	// Custom update function
	void GameScene::CustomUpdate(PxReal dt)
	{
		// Update object count
		objects = ObjectsCount();

		// Reduce the bridge timer
		bridgeTimer -= dt;
		initialBridgeTimer -= dt;

		// Reduce the bridge timer
		trampTimer -= dt;

		// Reduce the firework timer
		fireworkTimer -= dt;

		// Reduce the bouncer timer
		bouncerTimer -= dt;

		// Reduce the fire timer
		fireTimer -= dt;

		// Castle destroyed
		if (castleDestroyed)
		{
			// Set drive velocity
			drawBridgeJoint->DriveVelocity(1.0f);

			// If timer is less than 0
			if (bridgeTimer < 0.0f)
			{
				// Stop drive velocity
				drawBridgeJoint->DriveVelocity(0.0f);
				castleDestroyed = false;
			}
		}

		// Move bridge
		if (!bridgeSet)
		{
			drawBridgeJoint->DriveVelocity(drawBridgeJoint->DriveVelocity() * -1.0f);
			bridgeSet = true;
		}

		// Reset kick
		if (kicked)
		{
			kickTimer -= dt;
			if (kickTimer < 2.0f)
			{
				power = 30;
				kickJoint->DriveVelocity(-1.0f);

				if (kickTimer < 0.0f && balls != 0)
				{
					kicked = false;
					kickTimer = 5.0f;
					NewBall();
				}
			}
		}

		// Wait to reset the kicker
		if (!kicked) { kickResetTimer -= dt; }

		// Driving controls
		if (forward && !kicked)
		{
			wheelJointFL->DriveVelocity(-1.0f * speed);
			wheelJointFR->DriveVelocity(-1.0f * speed);
			wheelJointBL->DriveVelocity(-1.0f * speed);
			wheelJointBR->DriveVelocity(-1.0f * speed);
			SetBallPose();
		}
		if (backward && !kicked)
		{
			wheelJointFL->DriveVelocity(1.0f * speed);
			wheelJointFR->DriveVelocity(1.0f * speed);
			wheelJointBL->DriveVelocity(1.0f * speed);
			wheelJointBR->DriveVelocity(1.0f * speed);
			SetBallPose();
		}
		if (left && !kicked)
		{
			wheelJointFL->DriveVelocity(2.0f * speed);
			wheelJointFR->DriveVelocity(-2.0f * speed);
			SetBallPose();
		}
		if (right && !kicked)
		{
			wheelJointFL->DriveVelocity(-2.0f * speed);
			wheelJointFR->DriveVelocity(2.0f * speed);
			SetBallPose();
		}
		if (!forward && !backward && !left && !right)
		{
			wheelJointFL->DriveVelocity(0.0f);
			wheelJointFR->DriveVelocity(0.0f);
			wheelJointBL->DriveVelocity(0.0f);
			wheelJointBR->DriveVelocity(0.0f);
		}

		// If there is a goal
		if (collisionCallback->score)
		{
			score += 10;
			FireWorks();
			collisionCallback->score = false;
		}

		// Destroy castles
		if (collisionCallback->castleTrigger1)
		{
			DestroyCastle(castle1);
			castleTargets[0]->Color(color_palette[3]);
			castleTargets[0]->SetKinematic(true);
			collisionCallback->castleTrigger1 = false;
		}
		if (collisionCallback->castleTrigger2)
		{
			DestroyCastle(castle2);
			castleTargets[1]->Color(color_palette[3]);
			castleTargets[1]->SetKinematic(true);
			collisionCallback->castleTrigger2 = false;
		}
		if (collisionCallback->castleTrigger3)
		{
			DestroyCastle(castle3);
			castleTargets[2]->Color(color_palette[3]);
			castleTargets[2]->SetKinematic(true);
			collisionCallback->castleTrigger3 = false;
		}
		if (collisionCallback->castleTrigger4)
		{
			DestroyCastle(castle4);
			castleTargets[3]->Color(color_palette[3]);
			castleTargets[3]->SetKinematic(true);
			collisionCallback->castleTrigger4 = false;
		}

		// Collision filtering
		if (collisionCallback->targethit1Red)
		{
			castleTargets[0]->SetKinematic(false);
			((PxRigidBody*)castleTargets[0]->Get())->addTorque(PxVec3(-50.0f, 0.0f, 0.0f), PxForceMode::eIMPULSE);
			collisionCallback->targethit1Red = false;
		}
		if (collisionCallback->targethit3Red)
		{
			castleTargets[2]->SetKinematic(false);
			((PxRigidBody*)castleTargets[2]->Get())->addTorque(PxVec3(-50.0f, 0.0f, 0.0f), PxForceMode::eIMPULSE);
			collisionCallback->targethit3Red = false;
		}
		if (collisionCallback->targethit2Blue)
		{
			castleTargets[1]->SetKinematic(false);
			((PxRigidBody*)castleTargets[1]->Get())->addTorque(PxVec3(-50.0f, 0.0f, 0.0f), PxForceMode::eIMPULSE);
			collisionCallback->targethit2Blue = false;
		}
		if (collisionCallback->targethit4Blue)
		{
			castleTargets[3]->SetKinematic(false);
			((PxRigidBody*)castleTargets[3]->Get())->addTorque(PxVec3(-50.0f, 0.0f, 0.0f), PxForceMode::eIMPULSE);
			collisionCallback->targethit4Blue = false;
		}

		// Reset the firworks
		if (fireworkTimer < 0.0f)
		{
			for (int i = 0; i < fireworks1.size(); i++)
			{
				((PxRigidBody*)fireworks1[i]->Get())->setGlobalPose(PxTransform(PxVec3(-20.0f, 1.0f * i, -110.0f)));
				((PxRigidBody*)fireworks2[i]->Get())->setGlobalPose(PxTransform(PxVec3(20.0f, 1.0f * i, -110.0f)));
			}
		}

		// Reset the firworks
		if (bouncerTimer < 0.0f)
		{
			((PxRigidBody*)bouncer1->Get())->setGlobalPose(PxTransform(PxVec3(-55.0f, 50.0f, -20.0f)));
			((PxRigidBody*)bouncer2->Get())->setGlobalPose(PxTransform(PxVec3(55.0f, 50.0f, -20.0f)));

			bouncerTimer = 15.0f;
		}

		// Reset the fire cannons
		if (fireTimer < 4.5f)
		{
			if (!fired)
			{
				FireCannons();
				fired = true;
			}

			if (fireTimer < 0.0f)
			{
				((PxRigidBody*)bullet1->Get())->setGlobalPose(PxTransform(PxVec3(-52.0f, 19.0f, -76.0f)));
				((PxRigidBody*)bullet2->Get())->setGlobalPose(PxTransform(PxVec3(52.0f, 21.0f, -76.0f)));
				bullet1->SetKinematic(true);
				bullet2->SetKinematic(true);
			}
		}
	}

	// Get the score
	int GameScene::Score()
	{
		return score;
	}

	// Get the shot power
	float GameScene::Power()
	{
		return power / 10.0f;
	}

	// Get the ball count
	int GameScene::Balls()
	{
		return balls;
	}

	// Get the objects count
	int GameScene::Objects()
	{
		return objects;
	}

	// Get the destroyed castles count
	int GameScene::DestroyedCastles()
	{
		return castlesDestroyed;
	}

	// An example use of key release handling
	void GameScene::KeyReleaseHandler(int key)
	{
		// Driving controls
		if (key == 'T') forward = false;
		if (key == 'G') backward = false;
		if (key == 'F') left = false;
		if (key == 'H') right = false;
	}

	// An example use of key presse handling
	void GameScene::KeyPressHandler(int key)
	{
		// Increase shot power
		if (key == 'C')
		{
			power++;
			if (power > maxPower) power = maxPower;
		}

		// Shoot ball / reset arm
		if (key == 'V')
		{
			// Shoot the ball
			if (kickJoint->DriveVelocity() == -1.0f && kickResetTimer <= 0.0f)
			{
				kickJoint->DriveVelocity(power / 10.0f);
				kicked = true;
				kickResetTimer = 4.0f;
				SetBallPose();
				FireCannons();
			}
		}

		// Increase shot power
		if (key == 'P')
		{
			followPlayer = !followPlayer;
		}

		// Spawn balls
		if (key == 'N')
		{
			SetBalls(10);
		}
	}

	// An example use of key presse handling
	void GameScene::KeyHoldHandler(int key)
	{
		// Driving controls
		if (key == 'T') forward = true;
		if (key == 'G') backward = true;
		if (key == 'F') left = true;
		if (key == 'H') right = true;
	}

	// Build castle
	void GameScene::BuildCastle(float xOffset, float zOffset, float targetOffset, PxVec3 colour, vector<Box*>& castle)
	{
		// Build castle brick by brick
		for (int x = 0; x < 3; x++)
		{
			for (int y = 1; y < 4; y++)
			{
				for (int z = 0; z < 3; z++)
				{
					if (y == 3 && (abs(x) % 2 != 1 && (z == 0 || z == 2)) || (abs(z) % 2 != 1 && (x == 0 || x == 2)))
					{
						Box* box = new Box(PxTransform(PxVec3(xOffset + (3.0f * x), 0.0f + (3.0f * y), zOffset + (3.0f * z))), PxVec3(1.5f, 1.5f, 1.5f));
						box->Color(color_palette[5]);
						box->Material(concreteMaterial);
						Add(box);
						castle.push_back(box);
					}
					else if (y < 3 && (x == 0 || x == 2))
					{
						Box* box = new Box(PxTransform(PxVec3(xOffset + (3.0f * x), 0.0f + (3.0f * y), zOffset + (3.0f * z))), PxVec3(1.5f, 1.5f, 1.5f));
						box->Color(color_palette[5]);
						box->Material(concreteMaterial);
						Add(box);
						castle.push_back(box);
					}
					else if (y < 3 && (z == 0 || z == 2))
					{
						Box* box = new Box(PxTransform(PxVec3(xOffset + (3.0f * x), 0.0f + (3.0f * y), zOffset + (3.0f * z))), PxVec3(1.5f, 1.5f, 1.5f));
						box->Color(color_palette[5]);
						box->Material(concreteMaterial);
						Add(box);
						castle.push_back(box);
					}
				}
			}
		}

		// Target
		target = new Target(PxTransform(PxVec3(xOffset + targetOffset, 5.0f, zOffset + 3.0f)), PxVec3(5.0f, 5.0f, 5.0f));
		target->Color(PxVec3(1.0f, 1.0f, 1.0f));
		target->SetKinematic(true);
		target->Name("Target");
		Add(target);

		// Castle target box
		castleTargets.push_back(new Box(PxTransform(PxVec3(xOffset + targetOffset, 15.25f, zOffset + 3.0f), PxQuat(-PxPi * 2.0f, PxVec3(1.0f, 0.0f, 0.0f))), PxVec3(5.0f, 5.0f, 0.25f)));
		castleTargets.back()->Color(colour);
		castleTargets.back()->SetKinematic(true);
		((PxRigidBody*)castleTargets.back()->Get())->setMass(0.25f);
		castleTargets.back()->Name("TargetBox" + to_string(castleTargets.size() - 1));
		Add(castleTargets.back());

		// Filtering
		FilterGroup::Enum filterGroup;
		FilterGroup::Enum filterGroupBall;
		switch (castleIndex)
		{
		case 1: filterGroup = FilterGroup::CASTLE_TARGET_1; filterGroupBall = FilterGroup::RED_BALL; break;
		case 2: filterGroup = FilterGroup::CASTLE_TARGET_2; filterGroupBall = FilterGroup::BLUE_BALL; break;
		case 3: filterGroup = FilterGroup::CASTLE_TARGET_3; filterGroupBall = FilterGroup::RED_BALL; break;
		case 4: filterGroup = FilterGroup::CASTLE_TARGET_4; filterGroupBall = FilterGroup::BLUE_BALL; break;
		}
		castleTargets.back()->SetupFiltering(filterGroup, filterGroupBall);
		castleIndex++;

		// Trigger box
		castleTriggers.push_back(new Box(PxTransform(PxVec3(xOffset + targetOffset, 10.5f, zOffset - 1.5f))));
		castleTriggers.back()->Name("TriggerBox_inv" + to_string(castleTargets.size() - 1));
		castleTriggers.back()->SetKinematic(true);
		castleTriggers.back()->SetTrigger(true);
		Add(castleTriggers.back());

		// Castle target joint
		targetJoints.push_back(new RevoluteJoint(target, PxTransform(PxVec3(0.0f, 5.25f, 0.0f), PxQuat(PxPi * 2.0f, PxVec3(1.0f, 0.0f, 0.0f))), castleTargets.back(), PxTransform(PxVec3(0.0f, -5.0f, 0.0f))));
	}

	// Build team
	void GameScene::BuildTeams(float zOffset)
	{
		bool upJavalin = true;
		for (int i = 0; i < numberOfPlayers; i++)
		{
			playersRed.push_back(new Player(upJavalin, PxTransform(PxVec3(-40.0f + (10.0f * i), 1.5f, -zOffset - (5.0f * (i % 2)))), PxVec3(0.5f, 0.5f, 0.5f)));
			playersRed.back()->Color(PxVec3(1.0f, 0.0f, 0.0f));
			((PxRigidBody*)playersRed.back()->Get())->setMass(100.0f);
			playersRed.back()->Name("PlayerRed" + i);
			playersRed.back()->SetKinematic(true);
			Add(playersRed.back());
			upJavalin = !upJavalin;
		}
	}

	// Build the pitch
	void GameScene::BuildPitch()
	{
		// Plane 
		plane = new Plane();
		plane->Color(PxVec3(0.0f, 0.0f, 1.0f));
		Add(plane);

		// Pitch lines
		pitchLines = new PitchLines();
		pitchLines->Color(color_palette[3]);
		pitchLines->SetKinematic(true);
		pitchLines->Name("PitchLines");
		Add(pitchLines);

		// Pitch grass
		pitchBottomLeft = new Box(PxTransform(PxVec3(-34.875f, 0.0f, 30.0f)), PxVec3(34.875f, 1.0f, 29.75f));
		pitchBottomLeft->Color(color_palette[1] / 2.0f);
		pitchBottomLeft->SetKinematic(true);
		pitchBottomLeft->Name("PitchBottomLeft");
		pitchBottomLeft->Material(longGrassMaterial);
		Add(pitchBottomLeft);

		pitchBottomRight = new Box(PxTransform(PxVec3(34.875f, 0.0f, 30.0f)), PxVec3(34.875f, 1.0f, 29.75f));
		pitchBottomRight->Color(color_palette[1]);
		pitchBottomRight->SetKinematic(true);
		pitchBottomRight->Name("PitchBottomRight");
		pitchBottomRight->Material(shortGrassMaterial);
		Add(pitchBottomRight);

		pitchTopLeft = new Box(PxTransform(PxVec3(34.875f, 0.0f, -30.0f)), PxVec3(34.875f, 1.0f, 29.75f));
		pitchTopLeft->Color(color_palette[1] / 2.0f);
		pitchTopLeft->SetKinematic(true);
		pitchTopLeft->Name("PitchTopLeft");
		pitchTopLeft->Material(longGrassMaterial);
		Add(pitchTopLeft);

		pitchTopRight = new Box(PxTransform(PxVec3(-34.875f, 0.0f, -30.0f)), PxVec3(34.875f, 1.0f, 29.75f));
		pitchTopRight->Color(color_palette[1]);
		pitchTopRight->SetKinematic(true);
		pitchTopRight->Name("PitchTopRight");
		pitchTopRight->Material(shortGrassMaterial);
		Add(pitchTopRight);

		pitchTopGoal = new Box(PxTransform(PxVec3(0.0f, 0.0f, -90.0f)), PxVec3(69.75f, 1.0f, 9.75f));
		pitchTopGoal->Color(color_palette[1]);
		pitchTopGoal->SetKinematic(true);
		pitchTopGoal->Name("PitchTopGoal");
		Add(pitchTopGoal);

		pitchBottomGoal = new Box(PxTransform(PxVec3(0.0f, 0.0f, 90.0f)), PxVec3(69.75f, 1.0f, 9.75f));
		pitchBottomGoal->Color(color_palette[1]);
		pitchBottomGoal->SetKinematic(true);
		pitchBottomGoal->Name("PitchBottomGoal");
		Add(pitchBottomGoal);

		// Posts
		posts = new Post();
		posts->Color(PxVec3(1.0f, 1.0f, 1.0f));
		posts->SetKinematic(true);
		posts->Name("Posts");
		Add(posts);

		// Goal collision
		goalCollisionShape = new Box(PxTransform(PxVec3(0.0f, 40.25f, -85.5f)), PxVec3(5.0f, 30.0f, 0.1f));
		goalCollisionShape->SetKinematic(true);
		goalCollisionShape->SetTrigger(true);
		goalCollisionShape->Name("GoalTrigger_inv");
		//goalCollisionShape->GetShape()->setFlag(PxShapeFlag::eVISUALIZATION, false);
		Add(goalCollisionShape);
	}

	// Build the goadl castle
	void GameScene::BuildGoalCastle()
	{
		// Wall
		wall = new Wall(PxTransform(PxVec3(0.0f, 10.0f, -80.0f)), PxVec3(70.0f, 10.0f, 2.5f));
		wall->Color(color_palette[5]);
		wall->SetKinematic(true);
		wall->Material(concreteMaterial);
		wall->Name("Wall");
		Add(wall);

		// Bridge
		bridge = new Drawbridge(PxTransform(PxVec3(0.0f, 1.5f, -55.0f), PxQuat(-PxPi / 2.0f, PxVec3(1.0f, 0.0f, 0.0f))), PxVec3(12.25f, 12.5f, 0.5f));
		bridge->Color(color_palette[0]);
		bridge->SetKinematic(false);
		bridge->Material(woodMaterial);
		bridge->Name("Bridge");
		Add(bridge);

		drawBridgeJoint = new RevoluteJoint(nullptr, PxTransform(PxVec3(0.0f, 1.5f, -80.0f), PxQuat(-PxPi / 2.0f, PxVec3(1.0f, 0.0f, 0.0f))), bridge, PxTransform(PxVec3(0.0f, 25.0f, 0.0f)));
		drawBridgeJoint->DriveVelocity(1.5f);
		drawBridgeJoint->SetLimits(-PxPi / 2.0f, -0.025f);

		// Flags
		for (int i = 0; i < 2; i++)
		{
			flag = new Cloth(PxTransform(PxVec3(-70.0f + (140.0f * i), 30.0f, -80.0f), PxQuat(3.0f * PxPi / 6.0f, PxVec3(0.0f, 0.0f, 1.0f))), PxVec2(15.0f, 15.0f), 20, 20);
			flag->Color(color_palette[0]);
			flag->Name("Flag");
			Add(flag);

			// Setting custom cloth parameters
			((PxCloth*)flag->Get())->setStretchConfig(PxClothFabricPhaseType::eBENDING, PxClothStretchConfig(1.0f));
			((PxCloth*)flag->Get())->setSolverFrequency(20.0f);
			((PxCloth*)flag->Get())->setSelfCollisionDistance(0.1f);
			((PxCloth*)flag->Get())->setSelfCollisionStiffness(1.0f);
			((PxCloth*)flag->Get())->setClothFlag(PxClothFlag::eSWEPT_CONTACT, true);
			((PxCloth*)flag->Get())->setExternalAcceleration(PxVec3(5.0f + (i * -10.0f), 1.25f, 0.0f));

			flagPole = new Box(PxTransform(PxVec3(-70.0f + (140.0f * i), 35.0f, -80.0f)), PxVec3(0.25f, 10.0f, 0.25f));
			flagPole->SetKinematic(true);
			flagPole->Color(color_palette[3]);
			flag->Name("FlagPole");
			Add(flagPole);
		}
	}

	// Build the kickers
	void GameScene::BuildKickers(float xOffset, float zOffset)
	{
		// Base
		kickerBase = new KickerBase(PxTransform(PxVec3(0.0f, 3.5f, zOffset)), PxVec3(5.0f, 3.0f, 3.0f));
		kickerBase->Color(color_palette[6]);
		((PxRigidBody*)kickerBase->Get())->setMass(150.0f);
		kickerBase->Material(woodMaterial);
		kickerBase->Name("KickerBase");
		Add(kickerBase);

		// Kick
		kicker = new Kicker(PxTransform(PxVec3(-xOffset, 11.375f, zOffset), PxQuat(-PxPi / 2.0f, (PxVec3(1.0f, 0.0f, 0.0f)))), PxVec3(5.0f, 3.0f, 3.3f));
		kicker->Color(color_palette[6]);
		kicker->Material(woodMaterial);
		((PxRigidBody*)kicker->Get())->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		kicker->Name("Kicker");
		Add(kicker);

		// Kick joint
		kickJoint = new RevoluteJoint(kickerBase, PxTransform(PxVec3(0.0f, 6.0f, 0.0f), PxQuat(PxPi * 2, PxVec3(0.0f, 0.0f, 1.0f))), kicker, PxTransform(PxVec3(0.0f, 0.0f, -3.3f)));
		kickJoint->DriveVelocity(-1.0f);
		kickJoint->SetLimits(-PxPi / 4.0f, (PxPi * 3.0f) / 4.0f);
		//kickJoint->SetLimits(PxPi / 4.0f, -PxPi / 2.0f); // alternate joint limits

		// Front left wheel
		wheelFL = new Wheel(PxTransform(PxVec3(-5.5f, 3.5f, -5.0f + zOffset)));
		wheelFL->mesh->Color(color_palette[6]);
		wheelFL->mesh->Material(woodMaterial);
		Add(wheelFL->mesh);
		wheelJointFL = new RevoluteJoint(kickerBase, PxTransform(PxVec3(-5.25f, -0.5f, -5.0f)), wheelFL->mesh, PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
		wheelJointFL->DriveVelocity(0.0f);

		// Front right wheel
		wheelFR = new Wheel(PxTransform(PxVec3(5.5f, 3.5f, -5.0f + zOffset)));
		wheelFR->mesh->Color(color_palette[6]);
		wheelFR->mesh->Material(woodMaterial);
		Add(wheelFR->mesh);
		wheelJointFR = new RevoluteJoint(kickerBase, PxTransform(PxVec3(5.25f, -0.5f, -5.0f)), wheelFR->mesh, PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
		wheelJointFR->DriveVelocity(0.0f);

		// Back left wheel
		wheelBL = new Wheel(PxTransform(PxVec3(-5.5f, 3.5f, 5.0f + zOffset)));
		wheelBL->mesh->Color(color_palette[6]);
		wheelBL->mesh->Material(woodMaterial);
		Add(wheelBL->mesh);
		wheelJointBL = new RevoluteJoint(kickerBase, PxTransform(PxVec3(-5.25f, -0.5f, 5.0f)), wheelBL->mesh, PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
		wheelJointBL->DriveVelocity(0.0f);

		// Back right wheel
		wheelBR = new Wheel(PxTransform(PxVec3(5.5f, 3.5f, 5.0f + zOffset)));
		wheelBR->mesh->Color(color_palette[6]);
		wheelBR->mesh->Material(woodMaterial);
		Add(wheelBR->mesh);
		wheelJointBR = new RevoluteJoint(kickerBase, PxTransform(PxVec3(5.25f, -0.5f, 5.0f)), wheelBR->mesh, PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));
		wheelJointBR->DriveVelocity(0.0f);
	}

	// Set the balls
	void GameScene::SetBalls(int balls)
	{
		// Set the balls
		for (int i = 0; i < balls; i++)
		{
			ball.push_back(new Ball(PxTransform(PxVec3(20.0f + (2.0f * i), 3.5f, 5.0f), PxQuat(PxPi / 2.0f, (PxVec3(1.0f, 0.0f, 0.0f))))));

			((PxRigidBody*)ball.back()->mesh->Get())->setMass(0.45f);
			Add(ball.back()->mesh);

			if (i % 2 == 0)
			{
				ball.back()->mesh->Material(leatherMaterial);
				ball.back()->mesh->Color(color_palette[2]);
				ball.back()->mesh->Name("Blue_Ball");
				ball.back()->mesh->SetupFiltering(FilterGroup::BLUE_BALL, FilterGroup::CASTLE_TARGET_1 | FilterGroup::CASTLE_TARGET_2 | FilterGroup::CASTLE_TARGET_3 | FilterGroup::CASTLE_TARGET_4);
			}
			else
			{
				ball.back()->mesh->Material(rubberMaterial);
				ball.back()->mesh->Color(color_palette[0]);
				ball.back()->mesh->Name("Red_Ball");
				ball.back()->mesh->SetupFiltering(FilterGroup::RED_BALL, FilterGroup::CASTLE_TARGET_1 | FilterGroup::CASTLE_TARGET_2 | FilterGroup::CASTLE_TARGET_3 | FilterGroup::CASTLE_TARGET_4);
			}

			ball.back()->mesh->Get()->isRigidBody()->setRigidBodyFlag(PxRigidBodyFlag::eENABLE_CCD, true);
		}
	}

	// Reset the ball
	void GameScene::NewBall()
	{
		if (balls > 0)
		{
			balls--;
			SetBallPose();
			newBall = false;
		}
	}

	// Set ball pose
	void GameScene::SetBallPose()
	{
		PxVec3 pos = ((PxRigidBody*)kickerBase->Get())->getGlobalPose().p;
		PxTransform transform = PxTransform(PxVec3(pos.x, pos.y + 0.0f, pos.z), PxQuat(PxPi / 2.0f, (PxVec3(1.0f, 0.0f, 0.0f))));
		((PxRigidBody*)ball[balls]->mesh->Get())->setLinearVelocity(PxVec3(0.0f, 0.0f, 0.0f));
		((PxRigidBody*)ball[balls]->mesh->Get())->setAngularVelocity(PxVec3(0.0f, 0.0f, 0.0f));
		((PxRigidBody*)ball[balls]->mesh->Get())->setGlobalPose(transform);
	}

	// Destroy a castle
	void GameScene::DestroyCastle(vector<Box*>& castle)
	{
		for (int i = 0; i < castle.size(); i++)
		{
			int min = -15;
			int max = 15;
			float randomForce = (float)rand() / (float)RAND_MAX;
			((PxRigidBody*)castle[i]->Get())->addForce(PxVec3(min + randomForce * (max - min), 10.0f, min + randomForce * (max - min)) * 25.0f, PxForceMode::eIMPULSE);
		}
		castlesDestroyed++;
		castleDestroyed = true;
		bridgeTimer = 0.5f;
	}

	// Set spinners
	void GameScene::SetSpinners(float xOffset, float zOffset, float height, float drive)
	{
		spinnerBox = new Box(PxTransform(PxVec3(xOffset, 10.0f, zOffset)), PxVec3(height, 5.0f, 0.25f));
		spinnerBox->Color(color_palette[6]);
		spinnerBox->Material(woodMaterial);
		Add(spinnerBox);

		spinnerBase = new Box(PxTransform(PxVec3(xOffset, 5.0f, zOffset)), PxVec3(0.5f, 5.0f, 0.25f));
		Add(spinnerBase);
		spinnerBase->SetKinematic(true);

		spinner = new RevoluteJoint(spinnerBase, PxTransform(PxVec3(0.0f, height, 0.0f), PxQuat(PxPi / 2.0f, PxVec3(0.0f, 0.0f, 1.0f))), spinnerBox, PxTransform(PxVec3(0.0f, 5.0f, 0.0f)));
		spinner->DriveVelocity(drive);
	}

	// Set the trampolines
	void GameScene::SetTrampolines(float xOffset, float zOffset)
	{
		// Trampolines
		trampoline1 = new Trampoline(1000.0f, 10.0f, PxVec3(5.0f, 5.0f, 5.0f), PxTransform(PxVec3(-xOffset, 1.0f, zOffset), PxQuat(PxPiDivTwo, PxVec3(0.0f, 1.0f, 0.0f))), PxTransform(PxVec3(-xOffset, 5.0f, zOffset), PxQuat(PxPiDivTwo, PxVec3(0.0f, 1.0f, 0.0f))));
		trampoline1->AddToScene(this);

		trampoline2 = new Trampoline(1000.0f, 10.0f, PxVec3(5.0f, 5.0f, 5.0f), PxTransform(PxVec3(xOffset, 1.0f, zOffset), PxQuat(PxPiDivTwo, PxVec3(0.0f, 1.0f, 0.0f))), PxTransform(PxVec3(xOffset, 5.0f, zOffset), PxQuat(PxPiDivTwo, PxVec3(0.0f, 1.0f, 0.0f))));
		trampoline2->AddToScene(this);

		bouncer1 = new Player(true, PxTransform(PxVec3(-xOffset, 50.0f, zOffset)), PxVec3(0.5f, 0.5f, 0.5f));
		((PxRigidBody*)bouncer1->Get())->setMass(150.0f);
		bouncer1->Material(CreateMaterial(0.75f, 0.75f, 0.0f));
		bouncer1->Color(color_palette[0]);
		Add(bouncer1);

		bouncer2 = new Player(true, PxTransform(PxVec3(xOffset, 50.0f, zOffset)), PxVec3(0.5f, 0.5f, 0.5f));
		((PxRigidBody*)bouncer2->Get())->setMass(150.0f);
		bouncer2->Material(CreateMaterial(0.75f, 0.75f, 0.0f));
		bouncer2->Color(color_palette[0]);
		Add(bouncer2);
	}

	// Set the fireworks
	void GameScene::SetFireWorks(int count, float xOffset, float zOffset)
	{
		for (int i = 1; i <= count; i++)
		{
			fireworks1.push_back(new Box(PxTransform(PxVec3(-xOffset, 1.0f * i, zOffset)), PxVec3(0.5f, 0.5f, 0.5f)));
			Add(fireworks1.back());
			fireworks1.back()->Color(color_palette[1]);
			fireworks2.push_back(new Box(PxTransform(PxVec3(xOffset, 1.0f * i, zOffset)), PxVec3(0.5f, 0.5f, 0.5f)));
			Add(fireworks2.back());
			fireworks2.back()->Color(color_palette[1]);

			switch (i % 3)
			{
			case 0:
				fireworks1.back()->Color(color_palette[0]);
				fireworks2.back()->Color(color_palette[0]);
				break;
			case 1:
				fireworks1.back()->Color(color_palette[1]);
				fireworks2.back()->Color(color_palette[1]);
				break;
			case 2:
				fireworks1.back()->Color(color_palette[2]);
				fireworks2.back()->Color(color_palette[2]);
				break;
			default:
				fireworks1.back()->Color(color_palette[2] / 2.0f);
				fireworks2.back()->Color(color_palette[2] / 2.0f);
				break;
			}
		}
	}

	// Fire works
	void GameScene::FireWorks()
	{
		for (int i = 0; i < fireworks1.size(); i++)
		{
			int min = -15;
			int max = 15;
			float randomForce = (float)rand() / (float)RAND_MAX;
			((PxRigidBody*)fireworks1[i]->Get())->addForce(PxVec3(min + randomForce * (max - min), 10.0f, min + randomForce * (max - min)), PxForceMode::eIMPULSE);
			((PxRigidBody*)fireworks2[i]->Get())->addForce(PxVec3(min + randomForce * (max - min), 10.0f, min + randomForce * (max - min)), PxForceMode::eIMPULSE);
		}

		fireworkTimer = 7.5f;
	}

	// Set the cannons
	void GameScene::SetCannons(float xOffset, float zOffset)
	{
		cannon1 = new Cannon(PxTransform(PxVec3(-xOffset, 19.0f, zOffset ), PxQuat(PxPi / 4.0f, (PxVec3(0.0f, 1.0f, 0.0f)))), PxVec3(1.5f, 1.5f, 1.5f));
		cannon1->SetKinematic(true);
		cannon1->Color(color_palette[5] / 2.0f);
		Add(cannon1);

		cannon2 = new Cannon(PxTransform(PxVec3(xOffset, 21.0f, zOffset), PxQuat(-PxPi / 4.0f, (PxVec3(0.0f, 1.0f, 0.0f)))), PxVec3(1.5f, 1.5f, 1.5f));
		cannon2->SetKinematic(true);
		cannon2->Color(color_palette[5] / 2.0f);
		Add(cannon2);

		bullet1 = new Sphere(PxTransform(PxVec3(-xOffset, 19.0f, zOffset )), 1.0f);
		bullet1->Color(color_palette[5]);
		((PxRigidBody*)bullet1->Get())->setMass(50.0f);
		bullet1->SetKinematic(true);
		Add(bullet1);

		bullet2 = new Sphere(PxTransform(PxVec3(xOffset, 21.0f, zOffset)), 1.0f);
		bullet2->Color(color_palette[5]);
		((PxRigidBody*)bullet2->Get())->setMass(50.0f);
		bullet2->SetKinematic(true);
		Add(bullet2);
	}

	// Set the cannons
	void GameScene::FireCannons()
	{
		fireTimer = 5.0f;
		bullet1->SetKinematic(false);
		bullet2->SetKinematic(false);
		((PxRigidBody*)bullet1->Get())->addForce(PxVec3(2000.0f, 0.0f, 2000.0f), PxForceMode::eIMPULSE);
		((PxRigidBody*)bullet2->Get())->addForce(PxVec3(-2000.0f, 0.0f, 2000.0f), PxForceMode::eIMPULSE);
	}
}