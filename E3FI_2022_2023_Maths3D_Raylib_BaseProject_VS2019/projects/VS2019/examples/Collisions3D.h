#pragma once
#include "My3DPrimitives.h"

float EPSILON = powf(10., -6);

bool IntersectLinePlane(Line line, Plane plane, float& t, Vector3& interPt, Vector3& interNormal)
{
	// no intersection if line is parallel to the plane
	float dotProd = Vector3DotProduct(plane.n, line.dir);
	if (fabsf(dotProd) < EPSILON) return false;

	// intersection: t, interPt & interNormal
	t = (plane.d - Vector3DotProduct(plane.n, line.pt)) / dotProd;
	interPt = Vector3Add(line.pt, Vector3Scale(line.dir, t)); // OM = OA+tAB
	interNormal = Vector3Scale(plane.n, Vector3DotProduct(Vector3Subtract(line.pt, interPt), plane.n) < 0 ? -1.f : 1.f);
	return true;
}

bool IntersectSegmentPlane(Segment seg, Plane plane, float& t, Vector3& interPt, Vector3& interNormal)
{
	if (!IntersectLinePlane({ seg.pt1, Vector3Subtract(seg.pt2, seg.pt1) }, plane, t, interPt, interNormal))
		return false;
	return t >= 0 && t <= 1;
}

bool IntersectSegmentQuad(Segment seg, Quad quad, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 quadNormal = quad.ref.j;
	Plane quadPlane = {
		quadNormal,
		Vector3DotProduct(quadNormal, quad.ref.origin)
	};

	if (IntersectSegmentPlane(seg, quadPlane, t, interPt, interNormal))
	{
		Vector3 localInterPt = GlobalToLocalPos(interPt, quad.ref);
		return localInterPt.x <= quad.extents.x && localInterPt.x >= -quad.extents.x
			&& localInterPt.z <= quad.extents.z && localInterPt.z >= -quad.extents.z;
	}

	return false;
}

//NEEDS FIX
bool IntersectSegmentDisk(Segment segment, Disk disk, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 diskNormal = disk.ref.j;
	Plane diskPlane = {
		diskNormal,
		Vector3DotProduct(diskNormal, disk.ref.origin)
	};

	if (IntersectSegmentPlane(segment, diskPlane, t, interPt, interNormal))
	{
		Cylindrical localInterPt = CartesianToCylindrical(GlobalToLocalPos(interPt, disk.ref));
		return localInterPt.rho <= disk.radius && localInterPt.rho >= -disk.radius;
	}

	return false;
}

bool IntersectSegmentSphere(Segment seg, Sphere s, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 AB = Vector3Subtract(seg.pt2, seg.pt1);
	Vector3 OA = Vector3Subtract(seg.pt1, s.ref.origin);

	float a = powf(Vector3Length(AB), 2);
	float b = 2 * Vector3DotProduct(AB, OA);
	float c = powf(Vector3Length(OA), 2) - powf(s.radius, 2);
	float delta = powf(b, 2) - 4 * a * c;

	if (delta < 0) return false;
	float d = fabs(delta) <= EPSILON ? 0 : sqrtf(delta);

	t = (-b - d) / (2 * a);
	if (t < 0 || t > 1) return false;

	Vector3 OI = Vector3Subtract(interPt, s.ref.origin);

	interPt = Vector3Add(seg.pt1, Vector3Scale(AB, t));
	interNormal = Vector3Scale(OI, 1 / Vector3Length(OI));

	return true;
}
