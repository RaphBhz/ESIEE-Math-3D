#pragma once
#include <raymath.h>
#include <math.h>
#include <float.h>
#include <vector>
#include "My3DPrimitives.h"

struct Polar {
	float rho;
	float theta;
};

struct Cylindrical {
	float rho;
	float theta;
	float y;
};

struct Spherical {
	float rho;
	float theta;
	float phi;
};

Polar CartesianToPolar(Vector2 cart, bool keepThetaPositive = true)
{
	Polar polar = { Vector2Length(cart),atan2f(cart.y,cart.x) };
	if (keepThetaPositive && polar.theta < 0) polar.theta += 2 * PI;
	return polar;
}

Vector2 PolarToCartesian(Polar polar)
{
	return Vector2Scale({ cosf(polar.theta),sinf(polar.theta) }, polar.rho);
}

Vector3 CylindricalToCartesian(Cylindrical cyl) {
	return { cyl.rho * sinf(cyl.theta), cyl.y, cyl.rho * cosf(cyl.theta) };
}

Cylindrical CartesianToCylindrical(Vector3 cart) {
	Cylindrical cyl = {
		sqrt((double)powf(cart.x, 2) + (double)powf(cart.z, 2)),
		atan2(cart.x, cart.z),
		cart.y
	};

	if (cyl.theta <= 0)
		cyl.theta += 2 * PI;
	return cyl;
}

Vector3 SphericalToCartesian(Spherical sph) {
	return {
		sph.rho * sinf(sph.phi) * sinf(sph.theta),
		sph.rho * cosf(sph.phi),
		sph.rho * sinf(sph.phi) * cosf(sph.theta)
	};
}

Spherical CartesianToSpherical(Vector3 cart) {
	Spherical sph = {
		sqrt((double)pow(cart.x, 2) + (double)pow(cart.y, 2) + (double)pow(cart.z, 2)),
		atan2(cart.x, cart.z),
		acosf(cart.y / sph.rho)
	};

	if (sph.theta <= 0)
		sph.theta += 2 * PI;
	return sph;
}

Vector3 LocalToGlobalVect(Vector3 localVect, ReferenceFrame localRef)
{
	Vector3 globalVect = { localRef.i.x * localVect.x, localRef.j.y * localVect.y, localRef.k.z * localVect.z };
	Vector3RotateByQuaternion(globalVect, { -localRef.q.x, -localRef.q.y, -localRef.q.z, -localRef.q.w });

	return globalVect;
}

Vector3 GlobalToLocalVect(Vector3 globalVect, ReferenceFrame localRef)
{
	Vector3 localVect = { localRef.i.x * globalVect.x, localRef.j.y * globalVect.y, localRef.k.z * globalVect.z };
	Vector3RotateByQuaternion(localVect, localRef.q);

	return localVect;
}

Vector3 LocalToGlobalPos(Vector3 localPos, ReferenceFrame localRef)
{
	return Vector3Add(localPos, localRef.origin);
}

Vector3 GlobalToLocalPos(Vector3 globalPos, ReferenceFrame localRef)
{
	return Vector3Subtract(globalPos, localRef.origin);
}

Vector3 ProjectedPointOnLine(Vector3 linePt, Vector3 lineUnitDir, Vector3 pt)
{
	Vector3 projectedPoint = Vector3Subtract(pt, linePt);
	float projectionQuantity = Vector3DotProduct(lineUnitDir, projectedPoint);

	return {
		lineUnitDir.x * projectionQuantity,
		lineUnitDir.y * projectionQuantity,
		lineUnitDir.z * projectionQuantity
	};
}

float SqDistPointSegment(Segment seg, Vector3 pt)
{
	float distance = abs((seg.pt2.x - seg.pt1.x) * (seg.pt1.y - pt.y) - (seg.pt1.x - pt.x) * (seg.pt2.y - seg.pt1.y))
		/ sqrt(powf(seg.pt2.x - seg.pt1.x, 2) + powf(seg.pt2.y - seg.pt1.y, 2));

	return powf(distance, 2);
}

bool IsPointInsideBox(Box box, Vector3 globalPt)
{
	Vector3 localPt = GlobalToLocalPos(globalPt, box.ref);

	return localPt.x <= box.extents.x && localPt.x >= -box.extents.x
		&& localPt.y <= box.extents.y && localPt.y >= -box.extents.y
		&& localPt.z <= box.extents.z && localPt.z >= -box.extents.z;
}