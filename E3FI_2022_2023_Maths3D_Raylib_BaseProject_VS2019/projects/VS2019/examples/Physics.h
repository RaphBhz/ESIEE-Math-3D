#include <raymath.h>
#include "My3DPrimitives.h"
#pragma once
float g = 9.80665;
Vector3 G = { 0, -g, 0 };

struct Physics {
	float mass;
	float energy;
	float speed;

	Vector3 velocity;
};

Physics InitPhysics(Sphere object, Vector3 initialVelocity, float speed, float mass)
{
	Physics phys;
	phys.mass = mass;
	phys.speed = speed;
	phys.velocity = initialVelocity;
	phys.energy = mass/2 * powf(speed, 2) + mass * g * object.ref.origin.y;
	return phys;
}

void GetNewVelocity(float y, float ellapsedTime, Physics& phys)
{
	phys.velocity = Vector3Normalize(Vector3Add(phys.velocity, Vector3Scale(G, ellapsedTime)));
}

void GetSphereGravitationalTranslation(float y, float ellapsedTime, Physics& phys)
{
	GetNewVelocity(y, ellapsedTime, phys);
	phys.speed = sqrtf(2 * ((phys.energy - phys.mass * g * y) / phys.mass));
}
