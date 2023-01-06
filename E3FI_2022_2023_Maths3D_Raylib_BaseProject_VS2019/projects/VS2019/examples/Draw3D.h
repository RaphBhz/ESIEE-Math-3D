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
		rlVertex3f(0, 0, 0);
		rlVertex3f(sinf(i * deltaAngle), 0, cosf(i * deltaAngle));
		rlVertex3f(sinf((i + 1) * deltaAngle), 0, cosf((i + 1) * deltaAngle));
	}

	rlEnd();

	//EVERY rlPushMatrix method call should be followed by a rlPopMatrix method call
	rlPopMatrix();
}
void MyDrawWireframeDisk(Disk disk, int nSectors, Color color = DARKGRAY)
{
	// Verifying the chache available space
	int numVertex = nSectors * 3;
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
		rlVertex3f(0, 0, 0);
		rlVertex3f(sinf(i * deltaAngle), 0, cosf(i * deltaAngle));
		rlVertex3f(sinf(i * deltaAngle), 0, cosf(i * deltaAngle));
		rlVertex3f(sinf((i + 1) * deltaAngle), 0, cosf((i + 1) * deltaAngle));
	}

	rlEnd();
	rlPopMatrix();
}
void MyDrawDisk(Disk disk, int nSectors = 20, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY)
{
	if (drawPolygon) MyDrawPolygonDisk(disk, nSectors, polygonColor);
	if (drawWireframe)MyDrawWireframeDisk(disk, nSectors, wireframeColor);
}

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

void MyDrawPolygonSphere(Sphere sphere, int nMeridians, int nParallels, Color color = LIGHTGRAY) {

	// Checking GC cache limit
	int numVertex = nMeridians * nParallels * 2;
	if (rlCheckBufferLimit(numVertex)) rlglDraw();

	rlPushMatrix();

	// Adapting Space
	Vector3 vect;
	float angle;

	rlTranslatef(sphere.ref.origin.x, sphere.ref.origin.y, sphere.ref.origin.z);
	QuaternionToAxisAngle(sphere.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);

	// Drawing parameters
	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);

	// Sphere Parallels and Meridians
	float deltaParallels = (2*PI) / nParallels;
	float deltaMeridians = PI / nMeridians;
	
	// Drawing the sphere
	// We will draw two halves of the sphere at the same time to reduce time complexity
	for (int i = 0; i < nParallels / 2; i++) {
		for (int j = 0; j < nMeridians; j++) {
			// First vertex of the first half
			Spherical sph1 = { sphere.radius, deltaParallels * (j + 1), deltaMeridians * (i + 1) };
			Spherical sph2 = { sphere.radius, deltaParallels * (j + 1), deltaMeridians * i };
			Spherical sph3 = { sphere.radius, deltaParallels * j, deltaMeridians * i };

			Vector3 p1 = SphericalToCartesian(sph1);
			Vector3 p2 = SphericalToCartesian(sph2);
			Vector3 p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// First vertex of the second half
			sph1 = { sph1.rho, sph1.theta, sph1.phi + PI/2 };
			sph2 = { sph2.rho, sph2.theta, sph2.phi + PI/2 };
			sph3 = { sph3.rho, sph3.theta, sph3.phi + PI/2 };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// Second vertex of the first half
			sph1 = { sphere.radius, deltaParallels * j, deltaMeridians * i };
			sph2 = { sphere.radius, deltaParallels * j, deltaMeridians * (i+1) };
			sph3 = { sphere.radius, deltaParallels * (j+1), deltaMeridians * (i+1) };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// Second vertex of the second half
			sph1 = { sph1.rho, sph1.theta, sph1.phi + PI/2 };
			sph2 = { sph2.rho, sph2.theta, sph2.phi + PI/2 };
			sph3 = { sph3.rho, sph3.theta, sph3.phi + PI/2 };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
		}
	}

	rlEnd();
	rlPopMatrix();
}
void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY) {

	// Checking GC cache limit
	int numVertex = nMeridians * nParallels * 2;
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
	float deltaParallels = (2*PI) / nParallels;
	float deltaMeridians = PI / nMeridians;

	// Drawing the sphere
// We will draw two halves of the sphere at the same time to reduce time complexity
	for (int i = 0; i < nParallels / 2; i++) {
		for (int j = 0; j < nMeridians; j++) {
			// First vertex of the first half
			Spherical sph1 = { sphere.radius, deltaParallels * (j + 1), deltaMeridians * (i + 1) };
			Spherical sph2 = { sphere.radius, deltaParallels * (j + 1), deltaMeridians * i };
			Spherical sph3 = { sphere.radius, deltaParallels * j, deltaMeridians * i };

			Vector3 p1 = SphericalToCartesian(sph1);
			Vector3 p2 = SphericalToCartesian(sph2);
			Vector3 p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// First vertex of the second half
			sph1 = { sph1.rho, sph1.theta, sph1.phi + PI / 2 };
			sph2 = { sph2.rho, sph2.theta, sph2.phi + PI / 2 };
			sph3 = { sph3.rho, sph3.theta, sph3.phi + PI / 2 };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// Second vertex of the first half
			sph1 = { sphere.radius, deltaParallels * j, deltaMeridians * i };
			sph2 = { sphere.radius, deltaParallels * j, deltaMeridians * (i + 1) };
			sph3 = { sphere.radius, deltaParallels * (j + 1), deltaMeridians * (i + 1) };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);

			// Second vertex of the second half
			sph1 = { sph1.rho, sph1.theta, sph1.phi + PI / 2 };
			sph2 = { sph2.rho, sph2.theta, sph2.phi + PI / 2 };
			sph3 = { sph3.rho, sph3.theta, sph3.phi + PI / 2 };

			p1 = SphericalToCartesian(sph1);
			p2 = SphericalToCartesian(sph2);
			p3 = SphericalToCartesian(sph3);

			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p2.x, p2.y, p2.z);
			rlVertex3f(p3.x, p3.y, p3.z);
			rlVertex3f(p1.x, p1.y, p1.z);
			rlVertex3f(p3.x, p3.y, p3.z);
		}
	}

	rlEnd();
	rlPopMatrix();
}


void MyDrawSphere(Sphere sphere, int nMeridians = 10, int nParallels = 10, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonSphere(sphere, nMeridians, nParallels, polygonColor);
	if (drawWireframe) MyDrawWireframeSphere(sphere, nMeridians, nParallels, wireframeColor);
}

// cylindre 
void MyDrawPolygonCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY) {
	
	for (float i = 0;i <= nSectors;i++) {
		Quad quad = { cylinder.ref,{1,1,1} };
		
		// rotation perpendiculaire au sol
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,0,1 }), PI / 2));
		
		quad.ref.Translate({ i*2, 0, 0 });
		quad.ref.RotateByQuaternion(QuaternionFromAxisAngle(Vector3Normalize({ 0,1,0 }), ((4 * PI) / (2 * nSectors))*i));
		
		MyDrawPolygonQuad(quad);
		MyDrawWireframeQuad(quad);
	}
	
	
}

void MyDrawWireframeCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, Color color = LIGHTGRAY) {

}

void MyDrawCylinder(Cylinder cylinder, int nSectors, bool drawCaps = false, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
	if (drawPolygon) MyDrawPolygonCylinder(cylinder, nSectors, drawCaps, polygonColor);
	if (drawWireframe) MyDrawWireframeCylinder(cylinder, nSectors, drawCaps, wireframeColor);
}