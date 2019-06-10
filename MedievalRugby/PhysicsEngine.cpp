#include "PhysicsEngine.h"
#include <iostream>

// Pyhsics engine
namespace PhysicsEngine
{
	// Using the physx and std namespaces
	using namespace physx;
	using namespace std;

	// Default error and allocator callbacks
	PxDefaultErrorCallback gDefaultErrorCallback;
	PxDefaultAllocator gDefaultAllocatorCallback;

	// PhysX objects
	PxFoundation* foundation = 0;
	debugger::comm::PvdConnection* vd_connection = 0;
	PxPhysics* physics = 0;
	PxCooking* cooking = 0;

	// PhysX functions
	void PxInit()
	{
		// Foundation
		if (!foundation) foundation = PxCreateFoundation(PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
		if(!foundation) throw new Exception("PhysicsEngine::PxInit, Could not create the PhysX SDK foundation.");

		// Physics
		if (!physics) physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale());
		if(!physics) throw new Exception("PhysicsEngine::PxInit, Could not initialise the PhysX SDK.");

		// Cookinig
		if (!cooking) cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale()));
		if(!cooking) throw new Exception("PhysicsEngine::PxInit, Could not initialise the cooking component.");

		// Visual debugger
		if (!vd_connection) vd_connection = PxVisualDebuggerExt::createConnection(physics->getPvdConnectionManager(), "localhost", 5425, 100, PxVisualDebuggerExt::getAllConnectionFlags());

		// Create a deafult material
		CreateMaterial();
	}

	// Release the resources
	void PxRelease()
	{
		if (vd_connection) vd_connection->release();
		if (cooking) cooking->release();
		if (physics) physics->release();
		if (foundation) foundation->release();
	}

	// Get the physics
	PxPhysics* GetPhysics() 
	{ 
		return physics; 
	}

	// Get the cooking
	PxCooking* GetCooking()
	{
		return cooking;
	}

	// Get the physics material
	PxMaterial* GetMaterial(PxU32 index)
	{
		std::vector<PxMaterial*> materials(physics->getNbMaterials());
		if (index < physics->getMaterials((PxMaterial**)&materials.front(), (PxU32)materials.size())) return materials[index];
		else return 0;
	}

	// Create a phyics material
	PxMaterial* CreateMaterial(PxReal sf, PxReal df, PxReal cr) 
	{
		return physics->createMaterial(sf, df, cr);
	}

	// Actor methods
	PxActor* Actor::Get()
	{
		return actor;
	}

	// Set the colour of the actor
	void Actor::Color(PxVec3 new_color, PxU32 shape_index)
	{
		// Change color of all shapes
		if (shape_index == -1)
		{
			for (unsigned int i = 0; i < colors.size(); i++)
				colors[i] = new_color;
		}

		// Or only the selected one
		else if (shape_index < colors.size())
		{
			colors[shape_index] = new_color;
		}
	}

	// Get the colour of an actor
	const PxVec3* Actor::Color(PxU32 shape_indx)
	{
		if (shape_indx < colors.size()) return &colors[shape_indx];
		else return 0;			
	}

	// Set the actor material
	void Actor::Material(PxMaterial* new_material, PxU32 shape_index)
	{
		// Vector of shapes
		std::vector<PxShape*> shape_list = GetShapes(shape_index);

		// Loop through the shapes
		for (PxU32 i = 0; i < shape_list.size(); i++)
		{
			// Vector of materials - from the shapes vector
			std::vector<PxMaterial*> materials(shape_list[i]->getNbMaterials());

			// Loop through the materials - set a new material
			for (unsigned int j = 0; j < materials.size(); j++)
				materials[j] = new_material;

			// Set the shapes material
			shape_list[i]->setMaterials(materials.data(), (PxU16)materials.size());
		}
	}

	// Get a shape from an actor
	PxShape* Actor::GetShape(PxU32 index)
	{
		// Vector of shapes
		std::vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());
		if (index < ((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size()))
			return shapes[index];

		// No shape returned
		else return 0;
	}

	// Get all shapes of an actor
	std::vector<PxShape*> Actor::GetShapes(PxU32 index)
	{
		// Vector of shapes
		std::vector<PxShape*> shapes(((PxRigidActor*)actor)->getNbShapes());

		// Get the shapes
		((PxRigidActor*)actor)->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

		// If the index is minus return the shapes
		if (index == -1)
			return shapes;

		// Else index ;less than shape size - return the shapes
		else if (index < shapes.size())
			return std::vector<PxShape*>(1, shapes[index]);

		// Else return the shapes
		else return std::vector<PxShape*>();
	}

	// Set the actor name
	void Actor::Name(const string& new_name)
	{
		name = new_name;
		actor->setName(name.c_str());
	}

	// Get the actor name
	string Actor::Name()
	{
		return name;
	}

	// Set as trigger
	void Actor::SetTrigger(bool value, PxU32 shape_index)
	{
		std::vector<PxShape*> shape_list = GetShapes(shape_index);
		for (PxU32 i = 0; i < shape_list.size(); i++)
		{
			shape_list[i]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !value);
			shape_list[i]->setFlag(PxShapeFlag::eTRIGGER_SHAPE, value);
		}
	}

	// Setup filtering
	void Actor::SetupFiltering(PxU32 filterGroup, PxU32 filterMask, PxU32 shape_index)
	{
		std::vector<PxShape*> shape_list = GetShapes(shape_index);
		for (PxU32 i = 0; i < shape_list.size(); i++)
			shape_list[i]->setSimulationFilterData(PxFilterData(filterGroup, filterMask, 0, 0));
	}

	// Create a dynamic actor
	DynamicActor::DynamicActor(const PxTransform& pose) : Actor()
	{
		actor = (PxActor*)GetPhysics()->createRigidDynamic(pose);
		Name("");
	}

	// Delete a dynamic actor
	DynamicActor::~DynamicActor()
	{
		for (unsigned int i = 0; i < colors.size(); i++)
			delete (UserData*)GetShape(i)->userData;
	}

	// Creates the shape of the dynamic actor
	void DynamicActor::CreateShape(const PxGeometry& geometry, PxReal density)
	{
		// Creates a shape
		PxShape* shape = ((PxRigidDynamic*)actor)->createShape(geometry, *GetMaterial());

		// Computation of the mass and inertia properties
		PxRigidBodyExt::updateMassAndInertia(*(PxRigidDynamic*)actor, density);

		// Add colour to the colors vector
		colors.push_back(default_color);

		// Pass the color pointers to the renderer
		shape->userData = new UserData();
		
		// Set the colours
		for (unsigned int i = 0; i < colors.size(); i++)
			((UserData*)GetShape(i)->userData)->color = &colors[i];
	}

	// Set a dynamic actor kinematic
	void DynamicActor::SetKinematic(bool value, PxU32 index)
	{
		((PxRigidDynamic*)actor)->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, value);
	}

	// Create a static actor
	StaticActor::StaticActor(const PxTransform& pose)
	{
		actor = (PxActor*)GetPhysics()->createRigidStatic(pose);
		Name("");
	}

	// Delete a static actor
	StaticActor::~StaticActor()
	{
		for (unsigned int i = 0; i < colors.size(); i++)
			delete (UserData*)GetShape(i)->userData;
	}

	// Creates the shape of the static actor
	void StaticActor::CreateShape(const PxGeometry& geometry, PxReal density)
	{
		// Creates a shape
		PxShape* shape = ((PxRigidStatic*)actor)->createShape(geometry,*GetMaterial());

		// Add colour to the colors vector
		colors.push_back(default_color);

		// Pass the color pointers to the renderer
		shape->userData = new UserData();
		
		// Set the colours
		for (unsigned int i = 0; i < colors.size(); i++)
			((UserData*)GetShape(i)->userData)->color = &colors[i];
	}

	// Scene methods
	void Scene::Init()
	{
		// Scene
		PxSceneDesc sceneDesc(GetPhysics()->getTolerancesScale());

		// The CPU task dispacter used for the scene
		if(!sceneDesc.cpuDispatcher)
		{
			PxDefaultCpuDispatcher* mCpuDispatcher = PxDefaultCpuDispatcherCreate(1);
			sceneDesc.cpuDispatcher = mCpuDispatcher;
		}

		// The custom filter shader to use for collision filtering
		sceneDesc.filterShader = filterShader;

		// Create the physics scene
		px_scene = GetPhysics()->createScene(sceneDesc);
		if (!px_scene) throw new Exception("PhysicsEngine::Scene::Init, Could not initialise the scene.");

		// Default gravity
		px_scene->setGravity(PxVec3(0.0f, -9.81f, 0.0f));

		// User definded initialisation
		CustomInit();

		// Not paused
		pause = false;

		// No actor
		selected_actor = 0;

		// Get the next actor
		SelectNextActor();
	}

	// Update the physics
	void Scene::Update(PxReal dt)
	{
		// No update when paused
		if (pause) return;

		// Custom update
		CustomUpdate(dt);

		// Simulate the scene
		px_scene->simulate(dt);
		px_scene->fetchResults(true);
	}

	// Add an actor to the scene
	void Scene::Add(Actor* actor)
	{
		px_scene->addActor(*actor->Get());
		objects++;
	}

	// Get objects count
	int Scene::ObjectsCount()
	{
		return objects;
	}

	// Get the scene
	PxScene* Scene::Get() 
	{ 
		return px_scene; 
	}

	// Reset the scene
	void Scene::Reset()
	{
		px_scene->release();
		Init();
	}

	// Release the scene
	void Scene::Release()
	{
		px_scene->release();
	}

	// Pause the scene
	void Scene::Pause(bool value)
	{
		pause = value;
	}

	// Is the scene paused
	bool Scene::Pause() 
	{ 
		return pause;
	}

	// Get an actor
	PxRigidDynamic* Scene::GetSelectedActor()
	{
		return selected_actor;
	}

	// Select the next actor
	void Scene::SelectNextActor()
	{
		// Vector of actors
		std::vector<PxRigidDynamic*> actors(px_scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC));

		// If there are actors
		if (actors.size() && (px_scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC, (PxActor**)&actors.front(), (PxU32)actors.size())))
		{
			// If actor selected
			if (selected_actor)
			{
				// Loop through actors
				for (unsigned int i = 0; i < actors.size(); i++)
				{
					// If the actors match
					if (selected_actor == actors[i])
					{
						// Set highlight off
						HighlightOff(selected_actor);

						// Select the next actor
						selected_actor = actors[(i + 1) % actors.size()];
						break;
					}
				}
			}

			// Get the first actor - highlight it
			else selected_actor = actors[0];
			HighlightOn(selected_actor);
		}

		// No actor
		else selected_actor = 0;
	}

	// Select the next actor
	void Scene::SelectPreviousActor()
	{
		// Vector of actors
		std::vector<PxRigidDynamic*> actors(px_scene->getNbActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC));

		// If there are actors
		if (actors.size() && (px_scene->getActors(PxActorTypeSelectionFlag::eRIGID_DYNAMIC, (PxActor**)&actors.front(), (PxU32)actors.size())))
		{
			// If actor selected
			if (selected_actor)
			{
				// Loop through actors
				for (size_t i = actors.size(); i > 0; i--)
				{
					// If the actors match
					if (selected_actor == actors[i])
					{
						// Set highlight off
						HighlightOff(selected_actor);

						// Select the next actor
						selected_actor = actors[(i + 1) % actors.size()];
						break;
					}
				}
			}

			// Get the first actor - highlight it
			else selected_actor = actors[0];
			HighlightOn(selected_actor);
		}

		// No actor
		else selected_actor = 0;
	}

	// Get all the actors
	std::vector<PxActor*> Scene::GetAllActors()
	{
		// Set the type of actors to select
		physx::PxActorTypeSelectionFlags selection_flag = PxActorTypeSelectionFlag::eRIGID_DYNAMIC | PxActorTypeSelectionFlag::eRIGID_STATIC | PxActorTypeSelectionFlag::eCLOTH;

		// Set the vectors size
		std::vector<PxActor*> actors(px_scene->getNbActors(selection_flag));

		// Get the actors
		px_scene->getActors(selection_flag, (PxActor**)&actors.front(), (PxU32)actors.size());

		// Return the actors
		return actors;
	}

	// Highlight on
	void Scene::HighlightOn(PxRigidDynamic* actor)
	{
		// Store the original colour and adjust brightness of the selected actor
		std::vector<PxShape*> shapes(actor->getNbShapes());

		// Get the shapes
		actor->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

		// Clear the original colours
		sactor_color_orig.clear();

		// Loop through the shapes
		for (unsigned int i = 0; i < shapes.size(); i++)
		{
			// Get the colour
			PxVec3* color = ((UserData*)shapes[i]->userData)->color;

			// Set the colour
			sactor_color_orig.push_back(*color);

			// Add to the colour
			*color += PxVec3(0.2f, 0.2f, 0.2f);
		}
	}

	// Set highlight on
	void Scene::HighlightOff(PxRigidDynamic* actor)
	{
		// Restore the original color
		std::vector<PxShape*> shapes(actor->getNbShapes());

		// Get the shapes
		actor->getShapes((PxShape**)&shapes.front(), (PxU32)shapes.size());

		// Loop through the shapes and set the colour to the original
		for (unsigned int i = 0; i < shapes.size(); i++)
			*((UserData*)shapes[i]->userData)->color = sactor_color_orig[i];
	}

	// Constructor
	Joint::Joint() : joint(0) {}

	// Access to the joint
	PxJoint* Joint::Get() { return joint; }
}