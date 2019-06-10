#pragma once
#include "foundation/PxTransform.h"
#include "foundation/PxMat33.h"

// The visual debbuger namespace
namespace VisualDebugger
{
	// Using physx namespace
	using namespace physx;

	// Camera class
	class Camera
	{
	public:
		// Constructor
		Camera(const PxVec3& _eye, const PxVec3& _dir, const PxReal _speed);

		// Reset view
		void Reset();

		// Handle camera motion
		void Motion(int dx, int dy, PxReal delta_time);

		// Handle analog move
		void AnalogMove(float x, float y);

		// Get camera position
		PxVec3 getEye() const;

		// Set the camera position
		void setEye(PxVec3 position);

		// Get camera direction
		PxVec3 getDir() const;

		// Set the direction
		void setDir(PxVec3 rotation);

		// Get camera transformation
		PxTransform	getTransform() const;

		// Move camera forward
		void MoveForward(PxReal delta_time);

		// Move camera backward
		void MoveBackward(PxReal delta_time);

		// Move camera left
		void MoveLeft(PxReal delta_time);

		// Move camera right
		void MoveRight(PxReal delta_time);

		// Move camera up
		void MoveUp(PxReal delta_time);

		// Move camera down
		void MoveDown(PxReal delta_time);

	private:
		// Eye position
		PxVec3 eye;

		// Initial eye position
		PxVec3 eye_init;

		// Eye direction
		PxVec3 dir;

		// Initial eye direction
		PxVec3 dir_init;

		// Camera speed
		PxReal speed;

		// Initial camera speed
		PxReal speed_init;
	};
}