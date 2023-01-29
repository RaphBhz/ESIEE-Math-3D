#include <raymath.h>
#include "My3DPrimitives.h"
#include<vector> 
#pragma once
float g = 9.80665;
float BOX_FRICTION = 1.15;
Vector3 G = { 0, -g/6, 0 };

struct Physics {
	float mass;
	float energy;
	float speedValue;
	float angularSpeed;
	float inertia;

	Vector3 speed;
	Vector3 rotation;
	Vector3 momentum;
	Segment travel;
};

Physics InitPhysics(Sphere object, Vector3 initialDirection, Vector3 initialMomentum, float speed, float angularSpeed, float mass)
{
	Physics phys;
	phys.mass = mass;
	phys.speedValue = speed;
	phys.angularSpeed = angularSpeed;
	phys.inertia = 2. / 5. * (phys.mass * powf(object.radius, 2));
	phys.speed = Vector3Normalize(initialDirection);
	phys.energy = mass/2 * powf(speed, 2) + mass * g * object.ref.origin.y;
	phys.travel.pt1 = object.ref.origin;
	phys.travel.pt2 = object.ref.origin;
	phys.momentum = Vector3Scale(Vector3Normalize(initialMomentum), phys.inertia);
	phys.rotation = Vector3Normalize(phys.momentum);
	phys.rotation = Vector3Scale(phys.rotation, angularSpeed);

	return phys;
}

Vector3 GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(
	Vector3 colPos,
	Vector3 colNormal,
	float deltaTime,
	float colT,
	Sphere& sphere,
	Physics& phys
)
{
	float timeAfterCol = deltaTime - colT;

	Vector3 OC = Vector3Subtract(colPos, sphere.ref.origin);
	Vector3 relativeTangentMoment = Vector3Add(
	Vector3Subtract(
		phys.speed,
		Vector3Scale(colNormal, Vector3DotProduct(phys.speed, colNormal))
		),
		Vector3CrossProduct(phys.rotation, OC)
	);
	phys.momentum = Vector3Subtract(
		phys.momentum,
		Vector3Scale(
			Vector3CrossProduct(
				OC, relativeTangentMoment
			), BOX_FRICTION*colT
		)
	);
	phys.rotation = Vector3Normalize(phys.momentum);
	phys.rotation = Vector3Scale(phys.rotation, phys.angularSpeed);

	phys.speed = Vector3Reflect(phys.speed, colNormal);

	return Vector3Add(colPos, Vector3Scale(Vector3Normalize(phys.speed), phys.speedValue * timeAfterCol));
}

bool GetSphereNewPositionAndVelocityIfMultiCollidingWithRoundedBoxes(
	const std::vector<Box>& boxes,
	float ellapsedTime,
	Sphere& sphere,
	Physics& phys
)
{
	bool collision = false;
	float t, minT, collisionT;
	Vector3 interPt, minInterPt, interNormal, minInterNormal, newPosition, newVelocity;

	for (int i = 0; i < boxes.size(); i++)
	{
		RoundedBox minkowski = { boxes[i].ref, boxes[i].extents, sphere.radius };
		if (IntersectSegmentRoundedBox(phys.travel, minkowski, t, interPt, interNormal))
		{
			if (!collision || t < minT)
			{
				minT = t;
				minInterPt = interPt;
				minInterNormal = interNormal;
				collision = true;
			}
		}
	}

	if (collision)
	{
		Vector3 newPos = GetSphereNewPositionAndVelocityIfCollidingWithRoundedBox(
			minInterPt,
			minInterNormal,
			ellapsedTime,
			ellapsedTime * minT,
			sphere,
			phys);

		sphere.ref.origin = newPos;
		phys.travel.pt1 = newPos;
	}

	return collision;
}

void ApplyGravity(Sphere sphere, float ellapsedTime, Physics& phys)
{
	phys.speedValue = sqrtf(2 * ((phys.energy - phys.mass * g * sphere.ref.origin.y) / phys.mass));
	phys.speed = Vector3Add(phys.speed, Vector3Scale(G, ellapsedTime));
}

void UpdatePositionByPhysics(const std::vector<Box>& boxes, float ellapsedTime, Sphere& sphere, Physics& phys)
{
	ApplyGravity(sphere, ellapsedTime, phys);
	if (!GetSphereNewPositionAndVelocityIfMultiCollidingWithRoundedBoxes(
		boxes, ellapsedTime, sphere, phys
	))
	{
		phys.speed = Vector3Normalize(phys.speed);
		phys.speed = Vector3Scale(phys.speed, phys.speedValue * ellapsedTime);
		sphere.ref.origin = Vector3Add(sphere.ref.origin, phys.speed);
	}

	sphere.ref.RotateByQuaternion(QuaternionFromAxisAngle(
		Vector3Normalize(phys.momentum), phys.inertia * powf(phys.angularSpeed, 2))
	);
	phys.travel.pt2 = sphere.ref.origin;
}
 