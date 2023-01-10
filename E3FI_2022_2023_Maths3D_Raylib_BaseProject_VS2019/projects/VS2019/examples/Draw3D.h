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

void MyDrawWireframeQuad(Quad quad, Color color = DARKGRAY)
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

void MyDrawWireframeDisk(Disk disk, int nSectors, Color color = DARKGRAY)
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
	// Face 1
	Quad quad = { box.ref, {box.extents.x, 0, box.extents.z } };
	quad.ref.Translate({ 0, box.extents.y, 0 });
	MyDrawPolygonQuad(quad);

	// Face 2
	quad = { box.ref,{box.extents.x, 0, box.extents.y} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
	quad.ref.Translate({ 0, 0, box.extents.z });
	MyDrawPolygonQuad(quad);

	// Face 3
	quad = { box.ref,{box.extents.x, 0, box.extents.y} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), 3 * PI / 2));
	quad.ref.Translate({ 0, 0, -box.extents.z });
	MyDrawPolygonQuad(quad);

	// Face 4
	quad = { box.ref,{box.extents.x, 0, box.extents.z} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
	quad.ref.Translate({ 0, -box.extents.y, 0 });
	MyDrawPolygonQuad(quad);

	// Face 5
	quad = { box.ref,{box.extents.y, 0, box.extents.z} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
	quad.ref.Translate({ -box.extents.x, 0, 0 });
	MyDrawPolygonQuad(quad);

	// Face 6
	quad = { box.ref,{box.extents.y, 0, box.extents.z} };
	quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), 3 * PI / 2));
	quad.ref.Translate({ box.extents.x, 0, 0 });
	MyDrawPolygonQuad(quad);
}

void MyDrawWireframeBox(Box box, Color color = LIGHTGRAY)
{
	{
		// Face 1
		Quad quad = { box.ref, {box.extents.x, 0, box.extents.z } };
		quad.ref.Translate({ 0, box.extents.y, 0 });
		MyDrawWireframeQuad(quad);

		// Face 2
		quad = { box.ref,{box.extents.x, 0, box.extents.y} };
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI / 2));
		quad.ref.Translate({ 0, 0, box.extents.z });
		MyDrawWireframeQuad(quad);

		// Face 3
		quad = { box.ref,{box.extents.x, 0, box.extents.y} };
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), 3 * PI / 2));
		quad.ref.Translate({ 0, 0, -box.extents.z });
		MyDrawWireframeQuad(quad);

		// Face 4
		quad = { box.ref,{box.extents.x, 0, box.extents.z} };
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 1,0,0 }), PI));
		quad.ref.Translate({ 0, -box.extents.y, 0 });
		MyDrawWireframeQuad(quad);

		// Face 5
		quad = { box.ref,{box.extents.y, 0, box.extents.z} };
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
		quad.ref.Translate({ -box.extents.x, 0, 0 });
		MyDrawWireframeQuad(quad);

		// Face 6
		quad = { box.ref,{box.extents.y, 0, box.extents.z} };
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), 3 * PI / 2));
		quad.ref.Translate({ box.extents.x, 0, 0 });
		MyDrawWireframeQuad(quad);
	}
}

void MyDrawBox(Box box, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonBox(box, polygonColor);
	if (drawWireframe) MyDrawWireframeBox(box, wireframeColor);
}

// SPHERE
void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY) {

	// Checking GC cache limit
	int numVertex = nMeridians * (nParallels+1) * 6;
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
	float deltaParalel = PI / (nParallels+1);
	float deltaMeridian = (2*PI) / nMeridians;

	// Drawing the sphere meshes
	for (int i = 0; i <= nParallels; i++) {
		for (int j = 0; j < nMeridians; j++) {
			Spherical sph1 = { sphere.radius, deltaMeridian * j, deltaParalel * i };
			Spherical sph2 = { sphere.radius, deltaMeridian * j,  deltaParalel * (i + 1) };
			Spherical sph3 = { sphere.radius, deltaMeridian * (j + 1), deltaParalel * (i + 1) };
			Spherical sph4 = { sphere.radius, deltaMeridian * (j + 1), deltaParalel * i };

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

void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY) {

	// Checking GC cache limit
	int numVertex = nMeridians * (nParallels+1) * 10;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Drawing parameters
	rlBegin(RL_LINES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Sphere Parallels and Meridians
	float deltaParalel = PI / (nParallels+1);
	float deltaMeridian = (2*PI) / nMeridians;

	// Drawing the sphere wireframe
	for (int i = 0; i <= nParallels; i++) {
		for (int j = 0; j < nMeridians; j++) {
			Spherical sph1 = { sphere.radius, deltaMeridian * j, deltaParalel * i };
			Spherical sph2 = { sphere.radius, deltaMeridian * (j+1), deltaParalel * (i+1) };
			Spherical sph3 = { sphere.radius, deltaMeridian * j,  deltaParalel * (i+1) };
			Spherical sph4 = { sphere.radius, deltaMeridian * (j+1), deltaParalel * i };

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

void MyDrawSphere(Sphere sphere, int nMeridians = 10, int nParallels = 10, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonSphere(sphere, nMeridians, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeSphere(sphere, nMeridians, nParallels, wireframeColor);
}

// CYLINDER 
void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY) {
	// Checking GC cache limit
	int numVertex = nSectors * 6;
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

	for (int i = 0;i < nSectors;i++) {
		Cylindrical cyl1 = { cylinder.radius, deltaAngle * (i+1), cylinder.halfHeight };
		Cylindrical cyl2 = { cylinder.radius, deltaAngle * i, -cylinder.halfHeight};
		Cylindrical cyl3 = { cylinder.radius, deltaAngle * (i+1), -cylinder.halfHeight };
		Cylindrical cyl4 = { cylinder.radius, deltaAngle * i, cylinder.halfHeight };

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
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY) {
	// Checking GC cache limit
	int numVertex = nSectors * 4;
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

	for (int i = 0; i < nSectors; i++) {
		Cylindrical cyl1 = { cylinder.radius, deltaAngle * i, cylinder.halfHeight };
		Cylindrical cyl2 = { cylinder.radius, deltaAngle * i, -cylinder.halfHeight };
		Cylindrical cyl3 = { cylinder.radius, deltaAngle * (i + 1), -cylinder.halfHeight };
		Cylindrical cyl4 = { cylinder.radius, deltaAngle * (i + 1), cylinder.halfHeight };

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
	}

	rlEnd();
	rlPopMatrix();
}

void MyDrawCylinder(Cylinder cylinder, int nSectors = 10, bool drawCaps = false, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, drawCaps, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinder(cylinder, nSectors, drawCaps, wireframeColor);
}