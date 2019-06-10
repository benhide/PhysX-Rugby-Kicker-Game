#include "Actors.h"

// Physics engine
namespace PhysicsEngine
{
	// Constructor
	Plane::Plane(PxVec3 normal, PxReal distance)  : StaticActor(PxTransformFromPlaneEquation(PxPlane(normal, distance))) 
	{
		CreateShape(PxPlaneGeometry());
	}

	// Constructor
	Sphere::Sphere(const PxTransform& pose, PxReal radius, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxSphereGeometry(radius), density);
	}

	// Constructor
	Box::Box(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(dimensions), density);
	}

	// Constructor
	Capsule::Capsule(const PxTransform& pose, PxVec2 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxCapsuleGeometry(dimensions.x, dimensions.y), density);
	}

	// Constructor
	ConvexMesh::ConvexMesh(const vector<PxVec3>& verts, const PxTransform& pose, PxReal density) : DynamicActor(pose)
	{
		// Basic mesh descriptor
		PxConvexMeshDesc mesh_desc;

		// Number of mesh points
		mesh_desc.points.count = (PxU32)verts.size();

		// The offset stride
		mesh_desc.points.stride = sizeof(PxVec3);

		// The mesh data
		mesh_desc.points.data = &verts.front();

		// Set the flag to compute convex mesh
		mesh_desc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		// Set the vertex limit
		mesh_desc.vertexLimit = 256;

		// Create the shape
		CreateShape(PxConvexMeshGeometry(CookMesh(mesh_desc)), density);
	}

	// Mesh cooking (preparation)
	PxConvexMesh* ConvexMesh::CookMesh(const PxConvexMeshDesc& mesh_desc)
	{
		// Output stream
		PxDefaultMemoryOutputStream stream;

		// Throw exception if cannot cook convex mesh
		if (!GetCooking()->cookConvexMesh(mesh_desc, stream)) 
			throw new Exception("ConvexMesh::CookMesh, cooking failed.");

		// Input stream
		PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

		// Return the mesh
		return GetPhysics()->createConvexMesh(input);
	}

	// Constructor
	TriangleMesh::TriangleMesh(const vector<PxVec3>& verts, const std::vector<PxU32>& trigs, const PxTransform& pose) : StaticActor(pose)
	{
		// Basic mesh descriptor
		PxTriangleMeshDesc mesh_desc;

		// Number of mesh points
		mesh_desc.points.count = (PxU32)verts.size();

		// The offset stride
		mesh_desc.points.stride = sizeof(PxVec3);

		// The mesh data
		mesh_desc.points.data = &verts.front();

		// Triangle count
		mesh_desc.triangles.count = (PxU32)trigs.size();

		// Triangle offset stride
		mesh_desc.triangles.stride = 3 * sizeof(PxU32);

		// Triangle data
		mesh_desc.triangles.data = &trigs.front();

		// Create the shape
		CreateShape(PxTriangleMeshGeometry(CookMesh(mesh_desc)));
	}

	// Mesh cooking (preparation)
	PxTriangleMesh* TriangleMesh::CookMesh(const PxTriangleMeshDesc& mesh_desc)
	{
		// Output stream
		PxDefaultMemoryOutputStream stream;

		// Throw exception if cannot cook triangle mesh
		if (!GetCooking()->cookTriangleMesh(mesh_desc, stream))
			throw new Exception("TriangleMesh::CookMesh, cooking failed.");

		// Input stream
		PxDefaultMemoryInputData input(stream.getData(), stream.getSize());

		// Return the mesh
		return GetPhysics()->createTriangleMesh(input);
	}

	//*****JOINTS*****

	// Distance joint with the springs switched on
	DistanceJoint::DistanceJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	{
		PxRigidActor* px_actor0 = 0;
		if (actor0)
			px_actor0 = (PxRigidActor*)actor0->Get();

		joint = (PxJoint*)PxDistanceJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
		((PxDistanceJoint*)joint)->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
		Damping(1.0f);
		Stiffness(1.0f);
	}

	// Stiffness
	void DistanceJoint::Stiffness(PxReal value)
	{
		((PxDistanceJoint*)joint)->setStiffness(value);
	}
	PxReal DistanceJoint::Stiffness()
	{
		return ((PxDistanceJoint*)joint)->getStiffness();
	}

	// Dampening
	void DistanceJoint::Damping(PxReal value)
	{
		((PxDistanceJoint*)joint)->setDamping(value);
	}
	PxReal DistanceJoint::Damping()
	{
		return ((PxDistanceJoint*)joint)->getDamping();
	}

	// Revolute Joint
	RevoluteJoint::RevoluteJoint(Actor* actor0, const PxTransform& localFrame0, Actor* actor1, const PxTransform& localFrame1)
	{
		PxRigidActor* px_actor0 = 0;
		if (actor0)
			px_actor0 = (PxRigidActor*)actor0->Get();

		joint = PxRevoluteJointCreate(*GetPhysics(), px_actor0, localFrame0, (PxRigidActor*)actor1->Get(), localFrame1);
		joint->setConstraintFlag(PxConstraintFlag::eVISUALIZATION, true);
	}

	// Drive velocity
	void RevoluteJoint::DriveVelocity(PxReal value)
	{
		// Wake up the attached actors
		PxRigidDynamic *actor_0, *actor_1;
		((PxRevoluteJoint*)joint)->getActors((PxRigidActor*&)actor_0, (PxRigidActor*&)actor_1);
		if (actor_0)
		{
			//if (actor_0->isSleeping())
			//	actor_0->wakeUp();
		}
		if (actor_1)
		{
			if (actor_1->isSleeping())
				actor_1->wakeUp();
		}
		((PxRevoluteJoint*)joint)->setDriveVelocity(value);
		((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
	}
	PxReal RevoluteJoint::DriveVelocity()
	{
		return ((PxRevoluteJoint*)joint)->getDriveVelocity();
	}

	// Limits
	void RevoluteJoint::SetLimits(PxReal lower, PxReal upper)
	{
		((PxRevoluteJoint*)joint)->setLimit(PxJointAngularLimitPair(lower, upper));
		((PxRevoluteJoint*)joint)->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, true);
	}

	//*****COMPUND ACTORS*****

	// Pitch 
	PitchLines::PitchLines(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		for (int i = 0; i < pitchLineDimensions.size(); i++)
		{
			CreateShape(PxBoxGeometry(pitchLineDimensions[i]), density);
			GetShape(i)->setLocalPose(pitchLineTransforms[i]);
		}
	}

	// Post
	Post::Post(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		for (int i = 0; i < postDimensions.size(); i++)
		{
			CreateShape(PxBoxGeometry(postDimensions[i]), density);
			GetShape(i)->setLocalPose(postTransforms[i]);
		}
	}

	// Catapult
	Kicker::Kicker(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 15.0f, dimensions.y / 15.0f, dimensions.z)), density);
		GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 5.0f, dimensions.y / 15.0f, dimensions.z / 5.0f)), density);
		GetShape(1)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, dimensions.z)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 5.0f, dimensions.y / 20.0f, dimensions.z / 60.0f)), density);
		GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, -dimensions.y / 10.0f, dimensions.z + (dimensions.z / 5.5f))/*, PxQuat(PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))*/));

		//CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 5.0f, dimensions.y / 20.0f, dimensions.z / 20.0f)), density);
		//GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, -dimensions.y / 15.0f, dimensions.z + (dimensions.z / 7.5f)), PxQuat(PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));
	}

	// Drawbridge class
	Drawbridge::Drawbridge(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		for (int i = 0; i < 6; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z)), density);
			GetShape(i)->setLocalPose(PxTransform(PxVec3(-dimensions.x + ((dimensions.x / 2.5f) * i), dimensions.y, 0.0f)));
		}

		for (int i = 0; i < 10; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 30.0f, dimensions.z)), density);
			GetShape(6 + i)->setLocalPose(PxTransform(PxVec3(0.0f, (dimensions.y / 5.0f) + ((dimensions.y / 5.0f) * i), 0.0f)));
		}
	}

	// Players
	Player::Player(bool upJavalin, const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x * 1.5f, dimensions.y * 2.0f, dimensions.z * 3.0f)), density);
		GetShape(0)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 2.0f, dimensions.y, dimensions.z * 2.0f)), density);
		GetShape(1)->setLocalPose(PxTransform(PxVec3(0.0f, dimensions.y * 3.0f, dimensions.z * 3.0f), PxQuat(-PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 1.5f, dimensions.y, dimensions.z * 2.0f)), density);
		GetShape(2)->setLocalPose(PxTransform(PxVec3(0.0f, dimensions.y * 4.0f, dimensions.z * 5.0f), PxQuat(PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x * 2.0f, dimensions.y * 2.0f, dimensions.z * 2.0f)), density);
		GetShape(3)->setLocalPose(PxTransform(PxVec3(0.0f, dimensions.y * 2.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y, dimensions.z / 2.0f)), density);
		GetShape(4)->setLocalPose(PxTransform(PxVec3(dimensions.x * 2.0f, dimensions.y * 2.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y, dimensions.z / 2.0f)), density);
		GetShape(5)->setLocalPose(PxTransform(PxVec3(-dimensions.x * 2.0f, dimensions.y * 2.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y, dimensions.z)), density);
		GetShape(6)->setLocalPose(PxTransform(PxVec3(0.0f, dimensions.y * 5.0f, 0.0f)));

		if (upJavalin)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y * 10.0f, dimensions.z / 10.0f)), density);
			GetShape(7)->setLocalPose(PxTransform(PxVec3(-dimensions.x * 2.5f, dimensions.y * 10.0f, dimensions.z - (dimensions.z / 2.5f))));
		}
		else
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y / 10.0f, dimensions.z * 10.0f)), density);
			GetShape(7)->setLocalPose(PxTransform(PxVec3(-dimensions.x * 2.5f, dimensions.y * 2.0f, dimensions.z * 7.5f)));
		}
	}

	// Catapult base
	KickerBase::KickerBase(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y / 10.0f, dimensions.z * 2.0f)), density);
		GetShape(0)->setLocalPose(PxTransform(PxVec3(dimensions.x - (dimensions.x / 10.0f), 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y / 10.0f, dimensions.z * 2.0f)), density);
		GetShape(1)->setLocalPose(PxTransform(PxVec3(-dimensions.x + (dimensions.x / 10.0f), 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(2)->setLocalPose(PxTransform(PxVec3(dimensions.x - (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 10.0f), 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(3)->setLocalPose(PxTransform(PxVec3(-dimensions.x + (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 10.0f), 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 10.0f, dimensions.z / 10.0f)), density);
		GetShape(4)->setLocalPose(PxTransform(PxVec3(0.0f, dimensions.y * 2.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(5)->setLocalPose(PxTransform(PxVec3(-dimensions.x + (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 3.5f), -dimensions.z + (dimensions.z / 3.5f)), PxQuat(PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(6)->setLocalPose(PxTransform(PxVec3(dimensions.x - (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 3.5f), -dimensions.z + (dimensions.z / 3.5f)), PxQuat(PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(7)->setLocalPose(PxTransform(PxVec3(-dimensions.x + (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 3.5f), dimensions.z - (dimensions.z / 3.5f)), PxQuat(-PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		GetShape(8)->setLocalPose(PxTransform(PxVec3(dimensions.x - (dimensions.x / 10.0f), dimensions.y - (dimensions.y / 3.5f), dimensions.z - (dimensions.z / 3.5f)), PxQuat(-PxPi / 4.0f, PxVec3(1.0f, 0.0f, 0.0f))));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 10.0f, dimensions.z / 10.0f)), density);
		GetShape(9)->setLocalPose(PxTransform(PxVec3(0.0f, 0.0f, dimensions.z * 2.0f)));
	}

	// Walll class
	Wall::Wall(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 2.5f, dimensions.y, dimensions.z)), density);
		GetShape(0)->setLocalPose(PxTransform(PxVec3(-dimensions.x + (dimensions.x / 2.5f), 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 2.5f, dimensions.y, dimensions.z)), density);
		GetShape(1)->setLocalPose(PxTransform(PxVec3(dimensions.x - (dimensions.x / 2.5f), 0.0f, 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y * 1.25f, dimensions.z * 2.5f)), density);
		GetShape(2)->setLocalPose(PxTransform(PxVec3(-dimensions.x, dimensions.y - (dimensions.y / 1.25f), 0.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y * 1.25f, dimensions.z * 2.5f)), density);
		GetShape(3)->setLocalPose(PxTransform(PxVec3(dimensions.x, dimensions.y - (dimensions.y / 1.25f), 0.0f)));

		for (int i = 0; i < 5; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y / 5.0f, dimensions.z)), density);
			GetShape(4 + i)->setLocalPose(PxTransform(PxVec3((dimensions.x / 30.0f + (dimensions.x / 5.0f)) + (i * (dimensions.x / 7.0f)), dimensions.y, 0.0f)));
		}

		for (int i = 0; i < 5; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y / 5.0f, dimensions.z)), density);
			GetShape(9 + i)->setLocalPose(PxTransform(PxVec3((-dimensions.x / 30.0f - (dimensions.x / 5.0f)) - (i * (dimensions.x / 7.0f)), dimensions.y, 0.0f)));
		}

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y, dimensions.z * 36.0f)), density);
		GetShape(14)->setLocalPose(PxTransform(PxVec3(dimensions.x, 0.0f, dimensions.z * 36.0f)));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y, dimensions.z * 36.0f)), density);
		GetShape(15)->setLocalPose(PxTransform(PxVec3(-dimensions.x, 0.0f, dimensions.z * 36.0f)));

		for (int i = 0; i < 15; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y / 5.0f, dimensions.z)), density);
			GetShape(16 + i)->setLocalPose(PxTransform(PxVec3(-dimensions.x, dimensions.y, ((dimensions.z * 72.0f) - dimensions.z) - (i * (dimensions.z * 5.0f)))));
		}

		for (int i = 0; i < 15; i++)
		{
			CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 30.0f, dimensions.y / 5.0f, dimensions.z)), density);
			GetShape(31 + i)->setLocalPose(PxTransform(PxVec3(dimensions.x, dimensions.y, ((dimensions.z * 72.0f) - dimensions.z) - (i * (dimensions.z * 5.0f)))));
		}
	}

	// Ramp
	Ramp::Ramp(float height, float width, float length, PxTransform pose, PxReal density)
	{
		vertices[0].y *= height;	vertices[1].y *= height;	vertices[2].x *= width;		vertices[4].z *= length;		vertices[5].x *= width;
		vertices[0].x *= width;		vertices[1].x *= width;									vertices[5].z *= length;
		vertices[0].z *= length;	vertices[1].z *= length;

		mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)), pose, density);
	}

	// Trampoline
	Trampoline::Trampoline(PxReal stiffness, PxReal damping, const PxVec3& dimensions, PxTransform basePose, PxTransform trampolinePose)
	{
		// Thickness
		PxReal thickness = 0.1f;

		// Set the boxes
		bottom = new Box(PxTransform(PxVec3(basePose.p.x, basePose.p.y, basePose.p.z), PxQuat(basePose.q)), PxVec3(dimensions.x, thickness, dimensions.z));
		ramp = new Ramp((dimensions.y / 5.0f) * 2.0f, dimensions.x * 2.0f, dimensions.z * 2.0f, (PxTransform(PxVec3(trampolinePose.p.x - dimensions.x, trampolinePose.p.y, trampolinePose.p.z + dimensions.z), PxQuat(trampolinePose.q))));

		{
			//top = new Box(PxTransform(PxVec3(trampolinePose.p.x, trampolinePose.p.y /*+ thickness*/, trampolinePose.p.z)), PxVec3(dimensions.x, thickness, dimensions.z));
			//// Set the springs
			//springs.resize(4);
			//springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, dimensions.z)));
			//springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(dimensions.x, -dimensions.y, -dimensions.z)));
			//springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, dimensions.z)));
			//springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), top, PxTransform(PxVec3(-dimensions.x, -dimensions.y, -dimensions.z)));
		}

		// Set the springs
		springs.resize(4);
		springs[0] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, dimensions.z)), ramp->mesh, PxTransform(PxVec3(dimensions.x * 2.0f, -dimensions.y, dimensions.z * 2.0f)));
		springs[1] = new DistanceJoint(bottom, PxTransform(PxVec3(dimensions.x, thickness, -dimensions.z)), ramp->mesh, PxTransform(PxVec3(dimensions.x * 2.0f, -dimensions.y, 0.0f)));
		springs[2] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, dimensions.z)), ramp->mesh, PxTransform(PxVec3(0.0f, -dimensions.y, dimensions.z * 2.0f)));
		springs[3] = new DistanceJoint(bottom, PxTransform(PxVec3(-dimensions.x, thickness, -dimensions.z)), ramp->mesh, PxTransform(PxVec3(0.0f, -dimensions.y, 0.0f)));

		// Set stiffness and dampening
		for (unsigned int i = 0; i < springs.size(); i++)
		{
			springs[i]->Stiffness(stiffness);
			springs[i]->Damping(damping);
		}

		bottom->SetKinematic(true);
	}

	// Add trampoline
	void Trampoline::AddToScene(Scene* scene)
	{
		scene->Add(bottom);
		//scene->Add(top);
		scene->Add(ramp->mesh);
	}

	// Destructor
	Trampoline::~Trampoline()
	{
		for (unsigned int i = 0; i < springs.size(); i++)
			delete springs[i];
	}

	// Target
	Target::Target(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 20.0f, dimensions.y, dimensions.z / 20.0f)), density);
		GetShape(0)->setLocalPose(PxTransform(0.0f, 0.0f, 0.0f));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 40.0f, dimensions.z / 20.0f)), density);
		GetShape(1)->setLocalPose(PxTransform(0.0f, dimensions.y, 0.0f));

		//CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		//GetShape(2)->setLocalPose(PxTransform(dimensions.x, dimensions.y * 3.0f, 0.0f));

		//CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		//GetShape(3)->setLocalPose(PxTransform(-dimensions.x, dimensions.y * 3.0f, 0.0f));

		//CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 10.0f, dimensions.z / 10.0f)), density);
		//GetShape(4)->setLocalPose(PxTransform(0.0f, dimensions.y * 4.0f, 0.0f));

		//CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 10.0f, dimensions.y, dimensions.z / 10.0f)), density);
		//GetShape(3)->setLocalPose(PxTransform(0.0f, dimensions.y, dimensions.z));
	}

	// Constructor
	Cloth::Cloth(PxTransform pose, const PxVec2 & size, PxU32 width, PxU32 height, bool fix_top)
	{
		// Prepare vertices
		PxReal w_step = size.x / width;
		PxReal h_step = size.y / height;

		PxClothParticle* vertices = new PxClothParticle[(width + 1) * (height + 1) * 4];
		PxU32* quads = new PxU32[width * height * 4];

		for (PxU32 j = 0; j < (height + 1); j++)
		{
			for (PxU32 i = 0; i < (width + 1); i++)
			{
				PxU32 offset = i + j * (width + 1);
				vertices[offset].pos = PxVec3(w_step * i, 0.0f, h_step * j);
				if (fix_top && (j == 0))
					vertices[offset].invWeight = 0.0f;
				else
					vertices[offset].invWeight = 1.0f;
			}

			for (PxU32 j = 0; j < height; j++)
			{
				for (PxU32 i = 0; i < width; i++)
				{
					PxU32 offset = (i + j * width) * 4;
					quads[offset + 0] = (i + 0) + (j + 0) * (width + 1);
					quads[offset + 1] = (i + 1) + (j + 0) * (width + 1);
					quads[offset + 2] = (i + 1) + (j + 1) * (width + 1);
					quads[offset + 3] = (i + 0) + (j + 1) * (width + 1);
				}
			}
		}

		// Init cloth mesh description
		mesh_desc.points.data = vertices;
		mesh_desc.points.count = (width + 1) * (height + 1);
		mesh_desc.points.stride = sizeof(PxClothParticle);

		mesh_desc.invMasses.data = &vertices->invWeight;
		mesh_desc.invMasses.count = (width + 1) * (height + 1);
		mesh_desc.invMasses.stride = sizeof(PxClothParticle);

		mesh_desc.quads.data = quads;
		mesh_desc.quads.count = width * height;
		mesh_desc.quads.stride = sizeof(PxU32) * 4;

		// Create cloth fabric (cooking)
		PxClothFabric* fabric = PxClothFabricCreate(*GetPhysics(), mesh_desc, PxVec3(0.0f, -1.0f, 0.0f));

		// Create cloth
		actor = (PxActor*)GetPhysics()->createCloth(pose, *fabric, vertices, PxClothFlags());
		// Collisions with the scene objects
		((PxCloth*)actor)->setClothFlag(PxClothFlag::eSCENE_COLLISION, true);

		colors.push_back(default_color);
		actor->userData = new UserData(&colors.back(), &mesh_desc);
	}

	// Destructor
	Cloth::~Cloth()
	{
		delete (UserData*)actor->userData;
	}

	// Rugby ball
	Ball::Ball(PxTransform pose, PxReal density)
	{
		mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)), pose, density);
	}

	// Wheel class
	Wheel::Wheel(PxTransform pose, PxReal density)
	{
		mesh = new ConvexMesh(vector<PxVec3>(begin(vertices), end(vertices)), pose, density);
	}

	// Cannon class
	Cannon::Cannon(const PxTransform& pose, PxVec3 dimensions, PxReal density) : DynamicActor(pose)
	{
		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 5.0f, dimensions.z * 5.0f)), density);
		GetShape(0)->setLocalPose(PxTransform(0.0f, dimensions.y, 0.0f));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 5.0f, dimensions.y, dimensions.z * 5.0f)), density);
		GetShape(1)->setLocalPose(PxTransform(-dimensions.x, 0.0f, 0.0f));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x / 5.0f, dimensions.y, dimensions.z * 5.0f)), density);
		GetShape(2)->setLocalPose(PxTransform(dimensions.x, 0.0f, 0.0f));

		CreateShape(PxBoxGeometry(PxVec3(dimensions.x, dimensions.y / 5.0f, dimensions.z * 5.0f)), density);
		GetShape(3)->setLocalPose(PxTransform(0.0f, -dimensions.y, 0.0f));
	}
}