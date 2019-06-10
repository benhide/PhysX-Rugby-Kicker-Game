#pragma once
#include "PxPhysicsAPI.h"

// PhysX namespace
using namespace physx;

//add here any other structures that you want to pass from your simulation to the renderer
class UserData
{
public:
	// Constructor
	UserData(PxVec3* _color = 0, PxClothMeshDesc* _cloth_mesh_desc = 0);

	// Colour
	PxVec3* color;

	// Cloth mesh
	PxClothMeshDesc* cloth_mesh_desc;
};