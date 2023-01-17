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

// WIP, NOT WORKING YET
bool IntersectSegmentQuad(Segment seg, Quad quad, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 u = LocalToGlobalVect({ quad.extents.x, 0, 0 }, quad.ref);
	Vector3 v = LocalToGlobalVect({ 0, 0, quad.extents.z }, quad.ref);

	Vector3 quadNormal = Vector3Normalize(Vector3CrossProduct(u, v));
	Plane quadPlane = {
		quadNormal,
		quadNormal.x * quad.ref.origin.x + quadNormal.y * quad.ref.origin.y + quadNormal.z * quad.ref.origin.z
	};

	if (IntersectSegmentPlane(seg, quadPlane, t, interPt, interNormal))
	{
		Vector3 localInterPt = GlobalToLocalPos(interPt, quad.ref);
		return localInterPt.x <= quad.extents.x && localInterPt.x >= -quad.extents.x
			&& localInterPt.z <= quad.extents.z && localInterPt.z >= -quad.extents.z;
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

	interPt = Vector3Add(seg.pt1, Vector3Scale(AB, t));
	interNormal = Vector3Scale(interPt, 1 / Vector3Length(interPt));

	return true;
}



bool IntersectSegmentDisk(Segment segment, Disk disk, float& t, Vector3& interPt, Vector3& interNormal) {
	if (t > 1) return false;

	float R = disk.radius;
	Vector3 AB = Vector3Subtract(segment.pt2, segment.pt1);
	Vector3 OA = Vector3Subtract(segment.pt1, disk.ref.origin);
	Plane plan;
	IntersectSegmentPlane(segment, GetPlaneFromAnything(disk.ref), t, interPt, interNormal);
	Vector3 OM = Vector3Add(OA, Vector3Scale(AB, t));



	// we verify if OM.OM <= R
	// if not we return false
	//if(Vector3Multiply(OM,OM) > R) return false

}

Plane GetPlaneFromAnything(float x, float y, float z) {
	// https://www.kartable.fr/ressources/mathematiques/methode/montrer-que-trois-points-definissent-un-plan/4534
	Plane plane;
	Vector3 vect;
	vect.x = x;
	vect.y = y;
	vect.z = z;
	plane.n = vect;
	return plane;
}
