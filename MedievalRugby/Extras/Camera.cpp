#include "Camera.h"

// Visual debbuger
namespace VisualDebugger
{
	// Using the physx namespace
	using namespace physx;

	// Constructor
	Camera::Camera(const PxVec3 &_eye, const PxVec3& _dir, PxReal _speed)
	{
		// Initialise the variables
		eye_init = _eye;
		dir_init = _dir.getNormalized();
		speed_init = _speed;
		Reset();
	}

	// Reset the camera
	void Camera::Reset()
	{
		eye = eye_init;
		dir = dir_init;
		speed = speed_init;
	}

	// Camera motion
	void Camera::Motion(int dx, int dy, PxReal delta_time)
	{
		// Get the y view
		PxVec3 viewY = dir.cross(PxVec3(0.0f, 1.0f, 0.0f)).getNormalized();

		// Get the x rotation
		PxQuat qx(PxPi * dx * speed * delta_time / 180.0f, PxVec3(0.0f, 1.0f, 0.0f));

		// Set the x direction from the rotation
		dir = qx.rotate(dir);

		// Get the y rotation
		PxQuat qy(PxPi * dy * speed * delta_time / 180.0f, viewY);

		// Set the y direction from the rotation
		dir = qy.rotate(dir);

		// Normalise the direction
		dir.normalize();
	}

	// Move camera analog
	void Camera::AnalogMove(float x, float y)
	{
		PxVec3 viewY = dir.cross(PxVec3(0.0f, 1.0f, 0.0f)).getNormalized();
		eye += dir * y;
		eye += viewY * x;
	}

	// Get the eye position
	PxVec3 Camera::getEye() const 
	{ 
		return eye; 
	}

	// Set the camera eye
	void Camera::setEye(PxVec3 position)
	{
		eye = position;
	}

	// Get the eye direction
	PxVec3 Camera::getDir() const 
	{ 
		return dir; 
	}

	// Set the direction
	void Camera::setDir(PxVec3 rotation)
	{
		dir = rotation;
	}

	// Get the camera transform
	PxTransform	Camera::getTransform() const
	{
		// Get the y view
		PxVec3 viewY = dir.cross(PxVec3(0.0f, 1.0f, 0.0f));

		// If the normailsed y view - return the eye transform 
		if (viewY.normalize() < 1e-6f) 
			return PxTransform(eye);

		// Get the trnasform matrix
		PxMat33 m(dir.cross(viewY), viewY, -dir);

		// Return the transform
		return PxTransform(eye, PxQuat(m));
	}

	// Move camera forward
	void Camera::MoveForward(PxReal delta_time) 
	{ 
		eye += dir * speed * delta_time; 
	}

	// Move camera backwards
	void Camera::MoveBackward(PxReal delta_time) 
	{ 
		eye -= dir * speed * delta_time; 
	}

	// Move the camera left
	void Camera::MoveLeft(PxReal delta_time) 
	{
		PxVec3 viewY = dir.cross(PxVec3(0.0f, 1.0f, 0.0f)).getNormalized();
		eye -= viewY * speed * delta_time; 
	}

	// Move the camera right
	void Camera::MoveRight(PxReal delta_time) 
	{ 
		PxVec3 viewY = dir.cross(PxVec3(0.0f, 1.0f, 0.0f)).getNormalized();
		eye += viewY * speed * delta_time; 
	}

	// Move the camera up
	void Camera::MoveUp(PxReal delta_time)
	{
		eye += PxVec3(0.0f, 1.0f, 0.0f) * speed * delta_time; 
	}

	// Move the camera down
	void Camera::MoveDown(PxReal delta_time)
	{
		eye -= PxVec3(0.0f, 1.0f, 0.0f) * speed * delta_time; 
	}
}