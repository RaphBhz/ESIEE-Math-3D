#include <raymath.h>
#include "My3DPrimitives.h"
#pragma once
float g = 9.80665;
Vector3 G = { 0, -g, 0 };

struct Physics {
	float mass;
	float energy;
	float speed;

	Vector3 position;
	Vector3 direction;
	Vector3 movement;
};

Physics InitPhysics(Sphere object, Vector3 initialDirection, float speed, float mass)
{
	Physics phys;
	phys.position = object.ref.origin;
	phys.mass = mass;
	phys.speed = speed;
	phys.direction = initialDirection;
	phys.movement = { 0,0,0 };
	phys.energy = mass/2 * powf(speed, 2) + mass * g * object.ref.origin.y;
	return phys;
}

void GetNewVelocity(Quad quads[], float ellapsedTime, Physics& phys)
{
	bool collision = false;
	float t;
	Vector3 interPt, interNormal;
	Segment seg = { phys.position, Vector3Add(phys.position, phys.movement) };

	for(int i = 0; i < 1; i++)
	{
		if (IntersectSegmentQuad(seg, quads[i], t, interPt, interNormal))
		{
			collision = true;

			Quaternion q = QuaternionFromVector3ToVector3(phys.direction, interNormal);
			q = QuaternionScale(QuaternionInvert(q), 2);
			phys.direction = Vector3Normalize(Vector3RotateByQuaternion(phys.direction, q));
		}
	}
	
	if(!collision)
		phys.direction = Vector3Normalize(Vector3Add(phys.direction, Vector3Scale(G, ellapsedTime)));
}

void SphereGravitationalTranslation(Quad quads[], float y, float ellapsedTime, Physics& phys)
{
	GetNewVelocity(quads, ellapsedTime, phys);
	phys.speed = sqrtf(2 * ((phys.energy - phys.mass * g * y) / phys.mass));
	phys.movement = Vector3Scale(Vector3Scale(phys.direction, phys.speed), ellapsedTime);
}

void MoveSphere(Physics phys, Sphere& sphere)
{
	sphere.ref.origin = Vector3Add(sphere.ref.origin, phys.movement);
	phys.position = sphere.ref.origin;
}