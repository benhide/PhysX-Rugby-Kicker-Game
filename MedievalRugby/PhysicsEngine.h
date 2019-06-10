#pragma once
#include <vector>
#include "PxPhysicsAPI.h"
#include "Exception.h"
#include "Extras\UserData.h"
#include <string>

// Pyhsics engine namespace
namespace PhysicsEngine
{
	// Using the physx and std namespaces
	using namespace physx;
	using namespace std;
	
	// Initialise PhysX framework
	void PxInit();

	// Release PhysX resources
	void PxRelease();

	// Get the PxPhysics object
	PxPhysics* GetPhysics();

	// Get the cooking object
	PxCooking* GetCooking();

	// Get the specified material
	PxMaterial* GetMaterial(PxU32 index = 0);

	// Create a new material
	PxMaterial* CreateMaterial(PxReal sf = 0.0f, PxReal df = 0.0f, PxReal cr = 0.0f);

	// Defualt colour
	static const PxVec3 default_color(0.8f, 0.8f, 0.8f);

	// Abstract Actor class
	class Actor
	{
	public:
		// Constructor
		Actor() : actor(0) {}

		// Get the actor
		PxActor* Get();

		// Set thecolour
		void Color(PxVec3 new_color, PxU32 shape_index = -1);

		// Get the colour
		const PxVec3* Color(PxU32 shape_indx = 0);

		// Set the actor name
		void Actor::Name(const string& name);
		
		// Get the actor name
		string Actor::Name();

		// Set the material
		void Material(PxMaterial* new_material, PxU32 shape_index = -1);

		// Get the shape
		PxShape* GetShape(PxU32 index = 0);

		// Get the shapes
		std::vector<PxShape*> Actor::GetShapes(PxU32 index = -1);

		// Create a shape
		virtual void CreateShape(const PxGeometry& geometry, PxReal density) {}

		// Set as trigger
		void SetTrigger(bool value, PxU32 shape_index = -1);

		// Setup filtering
		void SetupFiltering(PxU32 filterGroup, PxU32 filterMask, PxU32 shape_index = -1);

	protected:
		// The actor
		PxActor* actor;
		
		// Tvector of colours
		std::vector<PxVec3> colors;
		
		// The actor name
		std::string name;
	};

	// Dynamic actor class
	class DynamicActor : public Actor
	{
	public:
		// Constructor
		DynamicActor(const PxTransform& pose);

		// Destructor
		~DynamicActor();

		// Create the shape
		void CreateShape(const PxGeometry& geometry, PxReal density);

		// Set kinematic
		void SetKinematic(bool value, PxU32 index = -1);
	};

	// Static actor class
	class StaticActor : public Actor
	{
	public:
		// Constructor
		StaticActor(const PxTransform& pose);

		// Destructor
		~StaticActor();

		// Create the shape
		void CreateShape(const PxGeometry& geometry, PxReal density = 0.0f);
	};

	// Generic Joint class
	class Joint
	{
	protected:
		// The joint
		PxJoint* joint;

	public:
		// Constructor
		Joint();

		// Access to the joint
		PxJoint* Get();
	};

	// Generic scene class
	class Scene
	{
	public:
		// Constructor
		Scene(PxSimulationFilterShader customFilteShader = PxDefaultSimulationFilterShader) : filterShader(customFilteShader) {}

		// Init the scene
		void Init();

		// User defined initialisation
		virtual void CustomInit() {}

		// Perform a single simulation step
		void Update(PxReal dt);

		// User defined update step
		virtual void CustomUpdate(PxReal dt) {}

		// Add actors
		void Add(Actor* actor);

		// Get the PxScene object
		PxScene* Get();

		// Reset the scene
		void Reset();
		
		// Release the scene
		void Release();

		// Set pause
		void Pause(bool value);

		// Get pause
		bool Pause();

		// Get the selected dynamic actor on the scene
		PxRigidDynamic* GetSelectedActor();

		// Switch to the next dynamic actor
		void SelectNextActor();

		// Switch to the previous dynamic actor
		void SelectPreviousActor();

		// List with all actors
		std::vector<PxActor*> GetAllActors();

		// Get objects count
		int ObjectsCount();

		// Object counter
		int objects = 0;

	protected:
		// Set highlight on
		void HighlightOn(PxRigidDynamic* actor);

		// Set highlight off
		void HighlightOff(PxRigidDynamic* actor);

		// PhysX scene object
		PxScene* px_scene;

		// Pause simulation
		bool pause;

		// Selected dynamic actor on the scene
		PxRigidDynamic* selected_actor;

		//original and modified colour of the selected actor
		std::vector<PxVec3> sactor_color_orig;

		// Filter shader
		PxSimulationFilterShader filterShader;
	};
}
