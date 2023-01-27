#pragma once
#include "My3DPrimitives.h"
#include<vector> 
using namespace std;
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
		return localInterPt.rho <= disk.radius;
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

struct tQuad {
	float t;
	Quad quad;
};
bool IntersectSegmentBox(Segment seg, Box box, float& t, Vector3& interPt, Vector3& interNormal) {
	
	vector <tQuad> arrIntersectSeg;


	// Face 1
	Quad quad = { box.ref, {box.extents.x / 2, 0, box.extents.z / 2 } };
	quad.ref.Translate({ 0, box.extents.y / 2, 0 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });

	};

	// Face 2
	quad = { box.ref,{box.extents.x / 2, 0, box.extents.y / 2} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad.ref.Translate({ 0, 0, box.extents.z / 2 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });

	};

	// Face 3
	quad = { box.ref,{box.extents.x / 2, 0, box.extents.y / 2} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), 3 * PI / 2));
	quad.ref.Translate({ 0, 0, -box.extents.z / 2 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });

	};

	// Face 4
	quad = { box.ref,{box.extents.x / 2, 0, box.extents.z / 2} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad.ref.Translate({ 0, -box.extents.y / 2, 0 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });

	};

	// Face 5
	quad = { box.ref,{box.extents.y / 2, 0, box.extents.z / 2} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad.ref.Translate({ -box.extents.x / 2, 0, 0 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });

	};

	// Face 6
	quad = { box.ref,{box.extents.y / 2, 0, box.extents.z / 2} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), 3 * PI / 2));
	quad.ref.Translate({ box.extents.x / 2, 0, 0 });
	if (IntersectSegmentQuad(seg, quad, t, interPt, interNormal)) {
		arrIntersectSeg.push_back({ t, quad });
	};

	

	// if there is nothing in the arrIntersectSeg array it mean that there is no collide
	if (arrIntersectSeg.size() == 0) return false;

	float minT = arrIntersectSeg[0].t;
	int minFaceIndex = 0;

	// we get the face who have the lower t
	for (int i = 0;i < arrIntersectSeg.size();i++) {
		if (minT <= arrIntersectSeg[i].t) continue;
		minT = arrIntersectSeg[i].t;
		minFaceIndex = i;
	}

	IntersectSegmentQuad(seg, arrIntersectSeg[minFaceIndex].quad, minT, interPt, interNormal);

	return true;

}

bool IntersectSegmentInfiniteCylinder(Segment seg, InfiniteCylinder cylinder, float& t, Vector3& interPt, Vector3& interNormal)
{
	Vector3 AB = Vector3Subtract(seg.pt2, seg.pt1);
	Vector3 PQ = Vector3Subtract(Vector3Add(cylinder.ref.origin, cylinder.ref.j), cylinder.ref.origin);
	Vector3 PA = Vector3Subtract(seg.pt1, cylinder.ref.origin);
	float ABPQ = Vector3DotProduct(AB, PQ);
	float PAPQ = Vector3DotProduct(PA, PQ);
	float PQ2 = powf(Vector3Length(PQ), 2);

	float a = powf(Vector3Length(AB), 2) - (powf(ABPQ, 2) / PQ2);
	float b = 2 * Vector3DotProduct(AB, PA) - (2 * PAPQ * ABPQ) / PQ2;
	float c = powf(Vector3Length(PA), 2) - (powf(PAPQ, 2) / powf(Vector3Length(PQ), 2)) - powf(cylinder.radius, 2);

	float delta = powf(b, 2) - 4 * a * c;

	if (delta < 0) return false;
	float d = fabs(delta) <= EPSILON ? 0 : sqrtf(delta);

	float t1 = (-b - d) / (2 * a);
	float t2 = (-b + d) / (2 * a);

	if (t1 < 0)
		t = t2;
	else
		t = t1;

	if (t < 0 || t > 1) return false;

	Vector3 OI = Vector3Subtract(interPt, cylinder.ref.origin);

	interPt = Vector3Add(seg.pt1, Vector3Scale(AB, t));
	interNormal = Vector3Scale(OI, 1 / Vector3Length(OI));

	return true;
}

bool IntersectSegmentCylinder(Segment seg, Cylinder cyl, float& t, Vector3 interPt, Vector3 interNormal)
{
	InfiniteCylinder infCyl = { cyl.ref, cyl.radius };
	if (IntersectSegmentInfiniteCylinder(seg, infCyl, t, interPt, interNormal))
	{
		Vector3 localInterPt = GlobalToLocalPos(interPt, cyl.ref);
		if (localInterPt.y <= cyl.halfHeight || localInterPt.y >= -cyl.halfHeight)
			return true;
	}
	return false;
}

bool IntersectSegmentCapsule(Segment seg, Capsule cap, float& t, Vector3& interPt, Vector3& interNormal)
{
	Cylinder cyl = { cap.ref, cap.halfHeight };

	if (IntersectSegmentCylinder(seg, cyl, t, interPt, interNormal))
	{
		Vector3 localInterPt = GlobalToLocalPos(interPt, cyl.ref);
		if (localInterPt.y <= cap.halfHeight || localInterPt.y >= -cap.halfHeight)
			return true;
	}

	Sphere upperSphere = { cap.ref, cap.radius };
	Sphere underSphere = { cap.ref, cap.radius };
	upperSphere.ref.origin.y += cap.halfHeight;
	underSphere.ref.origin.y -= cap.halfHeight;

	if (IntersectSegmentSphere(seg, upperSphere, t, interPt, interNormal))
	{
		Spherical interSph = CartesianToSpherical(interPt);
		if (interSph.phi >= 0 && interSph.phi <= PI / 2) return true;
	}
	if (IntersectSegmentSphere(seg, underSphere, t, interPt, interNormal))
	{
		Spherical interSph = CartesianToSpherical(interPt);
		if (interSph.phi >= 0 && interSph.phi <= PI / 2) return true;
	}

	return false;
}

bool IntersectSegmentRoundedBox(Segment seg, RoundedBox rndBox, float& t, Vector3& interPt, Vector3& interNormal)
{
	Box box = { rndBox.ref, rndBox.extents };

	if (IntersectSegmentBox(seg, box, t, interPt, interNormal)) return true;

	Capsule cap = { rndBox.ref, rndBox.radius };
	cap.ref.origin.x += rndBox.extents.x / 2;
	cap.ref.origin.z += rndBox.extents.z / 2;

	if (IntersectSegmentCapsule(seg, cap, t, interPt, interNormal))
	{
		Cylindrical interCyl = CartesianToCylindrical(interPt);
		if (interCyl.theta >= 0 && interCyl.theta <= PI / 2)
			return true;
	}
	cap.ref.origin.z -= rndBox.extents.z;

	if (IntersectSegmentCapsule(seg, cap, t, interPt, interNormal))
	{
		Cylindrical interCyl = CartesianToCylindrical(interPt);
		if (interCyl.theta >= PI / 2 && interCyl.theta <= PI)
			return true;
	}
	cap.ref.origin.x -= rndBox.extents.x;

	if (IntersectSegmentCapsule(seg, cap, t, interPt, interNormal))
	{
		Cylindrical interCyl = CartesianToCylindrical(interPt);
		if (interCyl.theta >= PI && interCyl.theta <= (3 * PI) / 2)
			return true;
	}
	cap.ref.origin.z += rndBox.extents.z;

	if (IntersectSegmentCapsule(seg, cap, t, interPt, interNormal))
	{
		Cylindrical interCyl = CartesianToCylindrical(interPt);
		if (interCyl.theta >= (3 * PI) / 2 && interCyl.theta <= 2 * PI)
			return true;
	}
}