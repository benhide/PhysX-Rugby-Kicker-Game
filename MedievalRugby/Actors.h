#pragma once
#include "PhysicsEngine.h"
#include <iostream>
#include <iomanip>

// Physics engine
namespace PhysicsEngine
{
	// Using the namespace
	using namespace std;

	// Plane class
	class Plane : public StaticActor
	{
	public:
		// A plane with default paramters: XZ plane centred at (0,0,0)
		Plane(PxVec3 normal = PxVec3(0.0f, 1.0f, 0.0f), PxReal distance = 0.0f);
	};

	// Sphere class
	class Sphere : public DynamicActor
	{
	public:
		// A sphere with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m
		// - denisty: 1kg/m^3
		Sphere(const PxTransform& pose = PxTransform(PxIdentity), PxReal radius = 1.0f, PxReal density = 1.0f);
	};

	// Box class
	class Box : public DynamicActor
	{
	public:
		// Box with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m x 1m
		// - denisty: 1kg/m^3
		Box(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(0.5f, 0.5f, 0.5f), PxReal density = 1.0f);
	};

	// Capsule class
	class Capsule : public DynamicActor
	{
	public:
		// Capsule with default parameters:
		// - pose in 0,0,0
		// - dimensions: 1m x 1m
		// - denisty: 1kg/m^3
		Capsule(const PxTransform& pose = PxTransform(PxIdentity), PxVec2 dimensions = PxVec2(1.0f, 1.0f), PxReal density = 1.0f);
	};

	// The ConvexMesh class
	class ConvexMesh : public DynamicActor
	{
	public:
		// Convex mesh with default parameters:
		// - pose in 0,0,0
		// - dimensions: verts
		// - denisty: 1kg/m^3
		ConvexMesh(const std::vector<PxVec3>& verts, const PxTransform& pose = PxTransform(PxIdentity), PxReal density = 1.0f);

		// Mesh cooking (preparation)
		PxConvexMesh* CookMesh(const PxConvexMeshDesc& mesh_desc);
	};

	// The TriangleMesh class
	class TriangleMesh : public StaticActor
	{
	public:
		// Convex mesh with default parameters:
		// - pose in 0,0,0
		// - dimensions: verts
		// - denisty: 1kg/m^3
		TriangleMesh(const vector<PxVec3>& verts, const vector<PxU32>& trigs, const PxTransform& pose = PxTransform(PxIdentity));

		// Mesh cooking (preparation)
		PxTriangleMesh* CookMesh(const PxTriangleMeshDesc& mesh_desc);
	};

	// Ball class
	class Ball /*: public ConvexMesh*/
	{
	public:
		// Ball vertices
		vector<PxVec3> vertices =
		{
			PxVec3(-0.475528, 0.000000, 0.247214),
			PxVec3(-0.207813, -0.207814, -0.647214),
			PxVec3(-0.336249, -0.336249, -0.247214),
			PxVec3(-0.336249, -0.336249, 0.247214),
			PxVec3(-0.207813, -0.207813, 0.647214),
			PxVec3(0.000000, -0.293893, -0.647214),
			PxVec3(0.000000, -0.475528, -0.247214),
			PxVec3(0.000000, -0.475528, 0.247214),
			PxVec3(0.000000, -0.293893, 0.647214),
			PxVec3(0.207814, -0.207814, -0.647214),
			PxVec3(0.336249, -0.336249, -0.247214),
			PxVec3(0.336249, -0.336249, 0.247214),
			PxVec3(0.207814, -0.207813, 0.647214),
			PxVec3(0.293893, -0.000000, -0.647214),
			PxVec3(0.475528, -0.000000, -0.247214),
			PxVec3(0.475528, -0.000000, 0.247214),
			PxVec3(0.293893, 0.000000, 0.647214),
			PxVec3(0.000000, -0.000000, -0.800000),
			PxVec3(0.207813, 0.207813, -0.647214),
			PxVec3(0.336249, 0.336249, -0.247214),
			PxVec3(0.336249, 0.336249, 0.247214),
			PxVec3(0.207813, 0.207813, 0.647214),
			PxVec3(0.000000, 0.293893, -0.647214),
			PxVec3(0.000000, 0.475528, -0.247214),
			PxVec3(0.000000, 0.475528, 0.247214),
			PxVec3(0.000000, 0.293893, 0.647214),
			PxVec3(-0.207813, 0.207813, -0.647214),
			PxVec3(-0.336249, 0.336249, -0.247214),
			PxVec3(-0.336249, 0.336249, 0.247214),
			PxVec3(-0.207813, 0.207813, 0.647214),
			PxVec3(0.000000, 0.000000, 0.800000),
			PxVec3(-0.293893, -0.000000, -0.647214),
			PxVec3(-0.475528, -0.000000, -0.247214),
			PxVec3(-0.293893, -0.000000, 0.647214),
		};

		// Mesh
		ConvexMesh* mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)));

		// Constructor
		Ball(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.0f);
	};

	// Pitch class
	class PitchLines : public DynamicActor
	{
	public:

		// Pitch line dimensions
		vector<PxVec3> pitchLineDimensions =
		{
			PxVec3(0.25f, 1.0f,  100.0f),
			PxVec3(0.25f, 1.0f,  100.0f),
			PxVec3(70.0f, 1.0f,   0.25f),
			PxVec3(70.0f, 1.0f,   0.25f),
			PxVec3(70.0f, 1.0f,   0.25f),
			PxVec3(70.0f, 1.0f,   0.25f),
			PxVec3(70.0f, 1.0f,   0.25f)
		};

		// Pitch line transforms
		vector<PxTransform> pitchLineTransforms =
		{
			PxTransform(PxVec3(70.0f,   0.0f,    0.0f)),
			PxTransform(PxVec3(-70.0f,  0.0f,    0.0f)),
			PxTransform(PxVec3(0.0f,    0.0f,  100.0f)),
			PxTransform(PxVec3(0.0f,    0.0f, -100.0f)),
			PxTransform(PxVec3(0.0f,    0.0f,   80.0f)),
			PxTransform(PxVec3(0.0f,    0.0f,  -80.0f)),
			PxTransform(PxVec3(0.0f,    0.0f,    0.0f))
		};

		// Pitch line
		PitchLines(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Pitch class
	class Post : public DynamicActor
	{
	public:

		// Post dimensions
		vector<PxVec3> postDimensions =
		{
			PxVec3(5.5f,  0.25f, 0.25f),
			PxVec3(0.25f, 12.5f, 0.25f),
			PxVec3(0.25f, 12.5f, 0.25f),
			PxVec3(5.5f,  0.25f, 0.25f),
			PxVec3(0.25f, 12.5f, 0.25f),
			PxVec3(0.25f, 12.5f, 0.25f)
		};

		// Post transforms
		vector<PxTransform> postTransforms =
		{
			PxTransform(PxVec3( 0.0f, 10.0f, -85.0f)),
			PxTransform(PxVec3( 5.5f, 12.5f, -85.0f)),
			PxTransform(PxVec3(-5.5f, 12.5f, -85.0f)),
			PxTransform(PxVec3( 0.0f, 10.0f,  85.0f)),
			PxTransform(PxVec3( 5.5f, 12.5f,  85.0f)),
			PxTransform(PxVec3(-5.5f, 12.5f,  85.0f))
		};

		// Posts
		Post(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Drawbridge class
	class Drawbridge : public DynamicActor
	{
	public:
		// The joint
		Drawbridge(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Kicker class
	class Kicker : public DynamicActor
	{
	public:

		// Catapult
		Kicker(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Kicker base class
	class KickerBase : public DynamicActor
	{
	public:

		// Catapult
		KickerBase(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Player class
	class Player : public DynamicActor
	{
	public:
		// The joint
		Player(bool upJavalin, const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Wall class
	class Wall : public DynamicActor
	{
	public:
		// Constructor
		Wall(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Distance joint with the springs switched on
	class DistanceJoint : public Joint
	{
	public:
		// The joint
		DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

		// Stiffness
		void Stiffness(PxReal value);
		PxReal Stiffness();

		// Dampening
		void Damping(PxReal value);
		PxReal Damping();
	};

	// Revolute Joint
	class RevoluteJoint : public Joint
	{
	public:
		// The joint
		RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1);

		// Drive velocity
		void DriveVelocity(PxReal value);
		PxReal DriveVelocity();

		// Limits
		void SetLimits(PxReal lower, PxReal upper);
	};

	// Target
	class Target : public DynamicActor
	{
	public:
		//Target
		Target(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};

	// Ramp class
	class Ramp
	{
	public:
		// Vertices
		vector<PxVec3> vertices =
		{
			PxVec3(0.5f, 1.0f, 0.0f),
			PxVec3(0.5f, 1.0f, 1.0f),
			PxVec3(1.0f, 0.0f, 0.0f),
			PxVec3(0.0f, 0.0f, 0.0f),
			PxVec3(0.0f, 0.0f, 1.0f),
			PxVec3(1.0f, 0.0f, 1.0f)
		};

		// Mesh
		ConvexMesh* mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)));

		// Ramp
		Ramp(float height = 1.0f, float width = 1.0f, float length = 1.0f, PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.0f);
	};

	// Trampoline classs using distance joints
	class Trampoline
	{
		// Springs
		vector<DistanceJoint*> springs;

		// Parts
		Box* bottom;
		Box* top;
		Ramp* ramp;

	public:
		// Constructor
		Trampoline(PxReal stiffness = 1.0f, PxReal damping = 1.0f, const PxVec3& dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxTransform basePose = PxTransform(PxIdentity), PxTransform trampolinePose = PxTransform(PxIdentity));

		// Add
		void AddToScene(Scene* scene);

		// Destructor
		~Trampoline();
	};

	// Ramp class
	class Wheel
	{
	public:
		// Vertices
		vector<PxVec3> vertices =
		{
			PxVec3(0.250000, -0.168953, -0.849385),
			PxVec3(-0.250000, -0.168953, -0.849385),
			PxVec3(0.250000, -0.331414, -0.800103),
			PxVec3(-0.250000, -0.331414, -0.800103),
			PxVec3(-0.250000, 0.000000, 0.000000),
			PxVec3(0.250000, -0.481138, -0.720074),
			PxVec3(-0.250000, -0.481138, -0.720074),
			PxVec3(0.250000, -0.612372, -0.612372),
			PxVec3(-0.250000, -0.612372, -0.612372),
			PxVec3(0.250000, -0.720074, -0.481138),
			PxVec3(-0.250000, -0.720074, -0.481138),
			PxVec3(0.250000, -0.800103, -0.331414),
			PxVec3(-0.250000, -0.800103, -0.331414),
			PxVec3(0.250000, -0.849385, -0.168953),
			PxVec3(-0.250000, -0.849385, -0.168953),
			PxVec3(0.250000, -0.866025, 0.000000),
			PxVec3(-0.250000, -0.866025, 0.000000),
			PxVec3(0.250000, -0.849385, 0.168953),
			PxVec3(-0.250000, -0.849385, 0.168953),
			PxVec3(0.250000, -0.800103, 0.331414),
			PxVec3(-0.250000, -0.800103, 0.331414),
			PxVec3(0.250000, -0.720074, 0.481138),
			PxVec3(-0.250000, -0.720074, 0.481138),
			PxVec3(0.250000, -0.612372, 0.612372),
			PxVec3(-0.250000, -0.612372, 0.612372),
			PxVec3(0.250000, -0.481138, 0.720074),
			PxVec3(-0.250000, -0.481138, 0.720074),
			PxVec3(0.250000, -0.331414, 0.800103),
			PxVec3(-0.250000, -0.331413, 0.800103),
			PxVec3(0.250000, -0.168953, 0.849385),
			PxVec3(-0.250000, -0.168953, 0.849385),
			PxVec3(0.250000, -0.000000, 0.866025),
			PxVec3(-0.250000, 0.000000, 0.866025),
			PxVec3(0.250000, 0.168953, 0.849385),
			PxVec3(-0.250000, 0.168953, 0.849385),
			PxVec3(0.250000, 0.331413, 0.800103),
			PxVec3(-0.250000, 0.331413, 0.800103),
			PxVec3(0.250000, 0.000000, 0.000000),
			PxVec3(0.250000, 0.481138, 0.720074),
			PxVec3(-0.250000, 0.481138, 0.720074),
			PxVec3(0.250000, 0.612372, 0.612372),
			PxVec3(-0.250000, 0.612372, 0.612372),
			PxVec3(0.250000, 0.720074, 0.481138),
			PxVec3(-0.250000, 0.720073, 0.481138),
			PxVec3(0.250000, 0.800103, 0.331414),
			PxVec3(-0.250000, 0.800103, 0.331413),
			PxVec3(0.250000, 0.849385, 0.168953),
			PxVec3(-0.250000, 0.849385, 0.168953),
			PxVec3(0.250000, 0.866025, 0.000000),
			PxVec3(-0.250000, 0.866025, 0.000000),
			PxVec3(0.250000, 0.849385, -0.168953),
			PxVec3(-0.250000, 0.849385, -0.168953),
			PxVec3(0.250000, 0.800103, -0.331413),
			PxVec3(-0.250000, 0.800103, -0.331413),
			PxVec3(0.250000, 0.720073, -0.481138),
			PxVec3(-0.250000, 0.720073, -0.481138),
			PxVec3(0.250000, 0.612372, -0.612372),
			PxVec3(-0.250000, 0.612372, -0.612372),
			PxVec3(0.250000, 0.481138, -0.720073),
			PxVec3(-0.250000, 0.481138, -0.720073),
			PxVec3(0.250000, 0.331413, -0.800103),
			PxVec3(-0.250000, 0.331413, -0.800103),
			PxVec3(0.250000, 0.168953, -0.849385),
			PxVec3(-0.250000, 0.168953, -0.849384),
			PxVec3(0.250000, -0.000000, -0.866025),
			PxVec3(-0.250000, -0.000000, -0.866025),
		};

		// Mesh
		ConvexMesh* mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)));

		// Ramp
		Wheel(PxTransform pose = PxTransform(PxIdentity), PxReal density = 1.0f);
	};

	// Cloth class
	class Cloth : public Actor
	{
		// Mesh
		PxClothMeshDesc mesh_desc;

	public:
		// Constructor
		Cloth(PxTransform pose = PxTransform(PxIdentity), const PxVec2& size = PxVec2(1.0f, 1.0f), PxU32 width = 1, PxU32 height = 1, bool fix_top = true);

		// Destructor
		~Cloth();
	};

	// Cannon class
	class Cannon : public DynamicActor
	{
	public:
		// Cannon
		Cannon(const PxTransform& pose = PxTransform(PxIdentity), PxVec3 dimensions = PxVec3(1.0f, 1.0f, 1.0f), PxReal density = 1.0f);
	};
}