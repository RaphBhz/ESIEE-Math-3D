#include <raymath.h>
#include "My3DPrimitives.h"
#pragma once
float g = 9.80665;
Vector3 G = { 0, -g, 0 };

struct Physics {
	float mass;
	float energy;
	float speedValue;

	Vector3 speed;
	Segment travel;
};

Physics InitPhysics(Sphere object, Vector3 initialDirection, float speed, float mass)
{
	Physics phys;
	phys.mass = mass;
	phys.speedValue = speed;
	phys.speed = initialDirection;
	phys.energy = mass/2 * powf(speed, 2) + mass * g * object.ref.origin.y;
	phys.travel.pt1 = object.ref.origin;
	return phys;
}

void ApplyGravity(Sphere sphere, float ellapsedTime, Physics& phys)
{
	phys.speedValue = sqrtf(2 * ((phys.energy - phys.mass * g * sphere.ref.origin.y) / phys.mass));
	phys.speed = Vector3Add(phys.speed, Vector3Scale(G, ellapsedTime));
}

void ApplyCollisions(Sphere sphere, Quad quads[], float ellapsedTime, Physics& phys)
{
	float t;
	Vector3 interPt, interNormal;

	for(int i = 0; i < 1; i++)
	{
		RoundedBox minkowski = { quads[i].ref, quads[i].extents, sphere.radius };
		if (IntersectSegmentQuad(phys.travel, quads[i], t, interPt, interNormal))
		{
			Quaternion q = QuaternionFromVector3ToVector3(phys.speed, interNormal);
			q = QuaternionScale(QuaternionInvert(q), 2);

			printf("COLLISION: speed: %f %f %f --> ", phys.speed.x, phys.speed.y, phys.speed.z);
			phys.speed = Vector3RotateByQuaternion(phys.speed, q);
			printf("%f %f %f\n", phys.speed.x, phys.speed.y, phys.speed.z);
			phys.travel.pt1 = interPt;
		}
	}
}

void UpdatePhysics(Sphere sphere, Quad quads[], float ellapsedTime, Physics& phys)
{
	ApplyGravity(sphere, ellapsedTime, phys);
	ApplyCollisions(sphere, quads, ellapsedTime, phys);

	phys.speed = Vector3Normalize(phys.speed);
	phys.speed = Vector3Scale(phys.speed, phys.speedValue * ellapsedTime);
	phys.travel.pt2 = Vector3Add(sphere.ref.origin, phys.speed);
}