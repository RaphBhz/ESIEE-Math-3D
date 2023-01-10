#pragma once
#include "My3DPrimitives.h"
#include "Referentials.h"

//QUAD
void MyDrawPolygonQuad(Quad quad, Color color = LIGHTGRAY)
{
	int numVertex = 6;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	// BEGINNING OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
		// methods should be called in this order: rlTranslatef, rlRotatef & rlScalef
		// so that transformations occur in the opposite order: scale, then rotation, then translation
	rlPushMatrix();
	//TRANSLATION
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	//ROTATION
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	//SCALING
	rlScalef(quad.extents.x, 1, quad.extents.z);
	// END OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlEnd();

	//EVERY rlPushMatrix method call should be followed by a rlPopMatrix method call
	rlPopMatrix();
}

void MyDrawWireframeQuad(Quad quad, Color color = LIGHTGRAY)
{
	int numVertex = 10;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	rlPushMatrix();
	rlTranslatef(quad.ref.origin.x, quad.ref.origin.y, quad.ref.origin.z);
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(quad.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(quad.extents.x, 1, quad.extents.z);
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	rlVertex3f(1, 0, 1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(1, 0, 1);
	rlVertex3f(-1, 0, -1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(-1, 0, 1);
	rlVertex3f(1, 0, 1);
	rlEnd();
	rlPopMatrix();
}

void MyDrawQuad(Quad quad, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonQuad(quad, polygonColor);
	if (drawWireframe)MyDrawWireframeQuad(quad, wireframeColor);
}

//DISK
void MyDrawPolygonDisk(Disk disk, int nSectors, Color color = LIGHTGRAY)
{
	// Besoin de 20 sommets de triangles pour dessiner le disque
	int numVertex = nSectors * 3;
	// Est-ce qu'on a assez de capacité restante dans le cache de la carte graphique, si non on libère en effectuant les opérations qui s'y trouvent
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	// BEGINNING OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
		// methods should be called in this order: rlTranslatef, rlRotatef & rlScalef
		// so that transformations occur in the opposite order: scale, then rotation, then translation
	rlPushMatrix();
	//TRANSLATION
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);
	//ROTATION
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	//SCALING
	rlScalef(disk.radius, 1, disk.radius);
	// END OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	float deltaAngle = 2 * PI / nSectors;

	for (int i = 0; i < nSectors; i++) {
		Cylindrical cyl1 = { 1, deltaAngle * i, 0 };
		Cylindrical cyl2 = { 1, deltaAngle * (i + 1), 0 };

		Vector3 p1 = CylindricalToCartesian(cyl1);
		Vector3 p2 = CylindricalToCartesian(cyl2);

		rlVertex3f(0, 0, 0);
		rlVertex3f(p1.x, p1.y, p1.z);
		rlVertex3f(p2.x, p2.y, p2.z);
	}

	rlEnd();

	//EVERY rlPushMatrix method call should be followed by a rlPopMatrix method call
	rlPopMatrix();
}

void MyDrawWireframeDisk(Disk disk, int nSectors, Color color = LIGHTGRAY)
{
	// Verifying the chache available space
	int numVertex = nSectors * 4;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	// Translating space
	rlPushMatrix();
	rlTranslatef(disk.ref.origin.x, disk.ref.origin.y, disk.ref.origin.z);

	// Rotating and scaling space
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(disk.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	rlScalef(disk.radius, 1, disk.radius);

	// Drawing
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);
	float deltaAngle = 2 * PI / nSectors;

	for (int i = 0; i < nSectors; i++) {
		Cylindrical cyl1 = { 1, deltaAngle * i, 0 };
		Cylindrical cyl2 = { 1, deltaAngle * (i + 1), 0 };

		Vector3 p1 = CylindricalToCartesian(cyl1);
		Vector3 p2 = CylindricalToCartesian(cyl2);

		rlVertex3f(0, 0, 0);
		rlVertex3f(p1.x, p1.y, p1.z);
		rlVertex3f(p1.x, p1.y, p1.z);
		rlVertex3f(p2.x, p2.y, p2.z);
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawDisk(Disk disk, int nSectors = 20, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonDisk(disk, nSectors, polygonColor);
	if (drawWireframe)MyDrawWireframeDisk(disk, nSectors, wireframeColor);
}

// BOX
void MyDrawPolygonBox(Box box, Color color = LIGHTGRAY)
{
	// Checking GC cache limit
	int numVertex = 36;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// FACE 1
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	// FACE 2
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	// FACE 3
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);

	// FACE 4
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);

	// FACE 5
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);

	// FACE 6
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);

	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeBox(Box box, Color color = LIGHTGRAY)
{
	// Checking GC cache limit
	int numVertex = 24;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);

	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, -box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, -box.extents.y / 2, box.extents.z / 2);
	rlVertex3f(-box.extents.x / 2, box.extents.y / 2, box.extents.z / 2);

	rlEnd();
	rlPopMatrix();
}

void MyDrawBox(Box box, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonBox(box, polygonColor);
	if (drawWireframe) MyDrawWireframeBox(box, wireframeColor);
}

// SPHERE
void MyDrawPolygonSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color = LIGHTGRAY)
{
	// Angular range of the sphere portion
	float deltaTheta = endTheta - startTheta;
	float deltaPhi = endPhi - startPhi;

	// Coefficient of the portion of sphere that will be drawn compared to a full sphere
	float drawCoeff = (deltaTheta / (2 * PI)) * (deltaPhi / (PI));

	// Checking GC cache limit
	int numVertex = nMeridians * (nParallels + 1) * 6 * drawCoeff;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Sphere Parallels and Meridians
	float deltaParalel = deltaPhi / (nParallels + 1);
	float deltaMeridian = deltaTheta / nMeridians;

	// Drawing the sphere meshes
	for (float phi = startPhi; phi <= endPhi; phi += deltaParalel) {
		for (float theta = startTheta; theta < endTheta; theta += deltaMeridian) {
			Spherical sph1 = { sphere.radius, theta, phi };
			Spherical sph2 = { sphere.radius, theta,  phi + deltaParalel };
			Spherical sph3 = { sphere.radius, theta + deltaMeridian, phi + deltaParalel };
			Spherical sph4 = { sphere.radius, theta + deltaMeridian, phi };

			Vector3 p1 = SphericalToCartesian(sph1);
			Vector3 p2 = SphericalToCartesian(sph2);
			Vector3 p3 = SphericalToCartesian(sph3);
			Vector3 p4 = SphericalToCartesian(sph4);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p4.x, p4.y, p4.z);
		}
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, Color color = LIGHTGRAY)
{
	// Angular range of the sphere portion
	float deltaTheta = endTheta - startTheta;
	float deltaPhi = endPhi - startPhi;

	// Coefficient of the portion of sphere that will be drawn compared to a full sphere
	float drawCoeff = (deltaTheta / (2 * PI)) * (deltaPhi / (PI));

	// Checking GC cache limit
	int numVertex = nMeridians * (nParallels + 1) * 10 * drawCoeff;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Sphere Parallels and Meridians
	float deltaParalel = (endPhi - startPhi) / (nParallels + 1);
	float deltaMeridian = (endTheta - startTheta) / nMeridians;

	// Drawing the sphere wireframe
	for (float phi = startPhi; phi <= endPhi; phi += deltaParalel) {
		for (float theta = startTheta; theta < endTheta; theta += deltaMeridian) {
			Spherical sph1 = { sphere.radius, theta, phi };
			Spherical sph2 = { sphere.radius, theta + deltaMeridian, phi + deltaParalel };
			Spherical sph3 = { sphere.radius, theta,  phi + deltaParalel };
			Spherical sph4 = { sphere.radius, theta + deltaMeridian, phi };

			Vector3 p1 = SphericalToCartesian(sph1);
			Vector3 p2 = SphericalToCartesian(sph2);
			Vector3 p3 = SphericalToCartesian(sph3);
			Vector3 p4 = SphericalToCartesian(sph4);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p4.x, p4.y, p4.z);
			rlVertex3f(p4.x, p4.y, p4.z);
			rlVertex3f(p2.x, p2.y, p2.z);
		}
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawSpherePortion(Sphere sphere, int nMeridians, int nParallels, float startTheta, float endTheta, float startPhi, float endPhi, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonSpherePortion(sphere, nMeridians, nParallels, startTheta, endTheta, startPhi, endPhi, polygonColor);
	if (drawWireframe) MyDrawWireframeSpherePortion(sphere, nMeridians, nParallels, startTheta, endTheta, startPhi, endPhi, wireframeColor);
}

void MyDrawSphere(Sphere sphere, int nMeridians = 10, int nParallels = 10, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonSpherePortion(sphere, nMeridians, nParallels, 0, 2 * PI, 0, PI, polygonColor);
	if (drawWireframe) MyDrawWireframeSpherePortion(sphere, nMeridians, nParallels, 0, 2 * PI, 0, PI, wireframeColor);
}

// CYLINDER 
void MyDrawPolygonCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, bool drawCaps, Color color = LIGHTGRAY)
{
	// Angular range of the cylinder portion
	float deltaTheta = endTheta - startTheta;

	// Coefficient of the drawn cylinder compared to a full cylinder
	float drawCoeff = deltaTheta / (2 * PI);

	// Checking GC cache limit
	int numVertex = nSectors * 6;
	if (drawCaps) numVertex += nSectors * 6;
	numVertex = (int)(numVertex * drawCoeff);
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	float deltaAngle = 2 * PI / nSectors;

	for (float theta = startTheta; theta < endTheta; theta += deltaAngle) {
		Cylindrical cyl1 = { cylinder.radius, theta + deltaAngle, cylinder.halfHeight };
		Cylindrical cyl2 = { cylinder.radius, theta, -cylinder.halfHeight };
		Cylindrical cyl3 = { cylinder.radius, theta + deltaAngle, -cylinder.halfHeight };
		Cylindrical cyl4 = { cylinder.radius, theta, cylinder.halfHeight };

		Vector3 p1 = CylindricalToCartesian(cyl1);
		Vector3 p2 = CylindricalToCartesian(cyl2);
		Vector3 p3 = CylindricalToCartesian(cyl3);
		Vector3 p4 = CylindricalToCartesian(cyl4);

		rlVertex3f(p1.x, p1.y, p1.z);
		rlVertex3f(p2.x, p2.y, p2.z);
		rlVertex3f(p3.x, p3.y, p3.z);

		rlVertex3f(p4.x, p4.y, p4.z);
		rlVertex3f(p2.x, p2.y, p2.z);
		rlVertex3f(p1.x, p1.y, p1.z);

		if (drawCaps) {
			rlVertex3f(p4.x, p4.y, p4.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(0, cylinder.halfHeight, 0);

			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(0, -cylinder.halfHeight, 0);
		}
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, bool drawCaps, Color color = LIGHTGRAY)
{
	// Angular range of the cylinder portion
	float deltaTheta = endTheta - startTheta;

	// Coefficient of the drawn cylinder compared to a full cylinder
	float drawCoeff = deltaTheta / (2*PI);

	// Checking GC cache limit
	int numVertex = nSectors * 8;
	if (drawCaps) numVertex += nSectors * 8;
	numVertex = (int)(numVertex * drawCoeff);
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(cylinder.ref.origin.x, cylinder.ref.origin.y, cylinder.ref.origin.z);
	QuaternionToAxisAngle(cylinder.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Parameters for drawing
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	float deltaAngle = 2 * PI / nSectors;

	for (float theta = startTheta; theta < endTheta; theta += deltaAngle) {
		Cylindrical cyl1 = { cylinder.radius, theta, cylinder.halfHeight };
		Cylindrical cyl2 = { cylinder.radius, theta, -cylinder.halfHeight };
		Cylindrical cyl3 = { cylinder.radius, theta + deltaAngle, -cylinder.halfHeight };
		Cylindrical cyl4 = { cylinder.radius, theta + deltaAngle, cylinder.halfHeight };

		Vector3 p1 = CylindricalToCartesian(cyl1);
		Vector3 p2 = CylindricalToCartesian(cyl2);
		Vector3 p3 = CylindricalToCartesian(cyl3);
		Vector3 p4 = CylindricalToCartesian(cyl4);

		rlVertex3f(p1.x, p1.y, p1.z);
		rlVertex3f(p2.x, p2.y, p2.z);
		rlVertex3f(p2.x, p2.y, p2.z);
		rlVertex3f(p3.x, p3.y, p3.z);
		rlVertex3f(p3.x, p3.y, p3.z);
		rlVertex3f(p4.x, p4.y, p4.z);
		rlVertex3f(p4.x, p4.y, p4.z);
		rlVertex3f(p1.x, p1.y, p1.z);

		if (drawCaps) {
			rlVertex3f(p4.x, p4.y, p4.z);
			rlVertex3f(0, cylinder.halfHeight, 0);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(0, cylinder.halfHeight, 0);

			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(0, -cylinder.halfHeight, 0);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(0, -cylinder.halfHeight, 0);
		}
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawCylinderPortion(Cylinder cylinder, int nSectors, float startTheta, float endTheta, bool drawCaps = false, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonCylinderPortion(cylinder, nSectors, startTheta, endTheta, drawCaps, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinderPortion(cylinder, nSectors, startTheta, endTheta, drawCaps, wireframeColor);
}

void MyDrawCylinder(Cylinder cylinder, int nSectors = 10, bool drawCaps = false, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonCylinderPortion(cylinder, nSectors, 0, 2*PI, drawCaps, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinderPortion(cylinder, nSectors, 0, 2*PI, drawCaps, wireframeColor);
}

// CAPSULE
void MyDrawPolygonCapsule(Capsule capsule, int nSectors, int nParallels, Color color = LIGHTGRAY)
{
	Cylinder cylinder = { capsule.ref, capsule.halfHeight, capsule.radius };
	MyDrawPolygonCylinderPortion(cylinder, nSectors, 0, 2*PI, false, color);

	Sphere sphere = { capsule.ref, capsule.radius };
	sphere.ref.Translate({ 0, capsule.halfHeight, 0 });
	MyDrawPolygonSpherePortion(sphere, nSectors, nParallels, 0, 2*PI, 0, PI/2, color);
	
	sphere.ref.Translate({ 0, -(capsule.halfHeight*2), 0 });
	MyDrawPolygonSpherePortion(sphere, nSectors, nParallels, 0, 2 * PI, PI/2, PI, color);
}

void MyDrawWireframeCapsule(Capsule capsule, int nSectors, int nParallels, Color color = LIGHTGRAY)
{
	Cylinder cylinder = { capsule.ref, capsule.halfHeight, capsule.radius };
	MyDrawWireframeCylinderPortion(cylinder, nSectors, 0, 2*PI, false, color);

	Sphere sphere = { capsule.ref, capsule.radius };
	sphere.ref.Translate({ 0, capsule.halfHeight, 0 });
	MyDrawWireframeSpherePortion(sphere, nSectors, nParallels, 0, 2 * PI, 0, PI / 2, color);

	sphere.ref.Translate({ 0, -(capsule.halfHeight * 2), 0 });
	MyDrawWireframeSpherePortion(sphere, nSectors, nParallels, 0, 2 * PI, PI / 2, PI, color);
}

void MyDrawCapsule(Capsule capsule, int nSectors = 10, int nParallels = 10, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonCapsule(capsule, nSectors, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeCapsule(capsule, nSectors, nParallels, wireframeColor);
}

// ROUNDED BOX
void MyDrawPolygonRoundedBox(RoundedBox roundedBox, int nSectors, Color color = LIGHTGRAY)
{

}

void MyDrawWireframeRoundedBox(RoundedBox roundedBox, int nSectors, Color color = LIGHTGRAY)
{

}

void MyDrawRoundedBox(RoundedBox roundedBox, int nSectors, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonRoundedBox(roundedBox, nSectors, polygonColor);
	if (drawWireframe) MyDrawWireframeRoundedBox(roundedBox, nSectors, wireframeColor);
}