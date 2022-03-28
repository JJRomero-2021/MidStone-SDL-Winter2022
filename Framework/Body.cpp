#include "Body.h"

Body::Body()
{
	pos.x = 0.0f;
	pos.y = 0.0f;
	pos.z = 0.0f;

	vel.x = 0.0f;
	vel.y = 0.0f;
	vel.z = 0.0f;

	accel.x = 0.0f;
	accel.y = 0.0f;
	accel.z = 0.0f;
	rotationZ = 0.0f;

	mass = 0.0f;
	isMoving(false);
}

Body::Body(Vec3 pos_, Vec3 vel_, Vec3 accel_, float mass_)
{
	pos = pos_;
	vel = vel_;
	accel = accel_;
	mass = mass_;
	rotationZ = 0.0f;
	r = 1.17f;

}

Body::~Body()
{

}
void Body::ApplyForce(Vec3 force)
{
	accel = force / mass;
}

void Body::Update(float deltaTime)
{
	if (is_moving == false)
	{
		return;
	}
	Vec3 gravity(0.0f, 0.0f, 0.0f);
	pos += vel * deltaTime + 0.5f * accel * deltaTime * deltaTime + 0.5f * gravity * deltaTime * deltaTime;
	vel += accel * deltaTime + gravity * deltaTime;
	/// Rigid Body Rotation
	rotationZ += (180.0f / M_PI) * vel.x * deltaTime / r;
}