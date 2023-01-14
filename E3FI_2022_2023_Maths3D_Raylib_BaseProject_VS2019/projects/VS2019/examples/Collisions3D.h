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