#pragma once
#include "My3DPrimitives.h"

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
	int numVertex = nSectors*3;
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
	int numQuads = 6;
	if (rlCheckBufferLimit(numQuads)) rlglDraw();

	rlPushMatrix();
	//TRANSLATION
	rlTranslatef(box.ref.origin.x, box.ref.origin.y, box.ref.origin.z);
	//ROTATION
	Vector3 vect;
	float angle;
	QuaternionToAxisAngle(box.ref.q, &vect, &angle);
	rlRotatef(angle * RAD2DEG, vect.x, vect.y, vect.z);
	//SCALING
	rlScalef(box.extents.x, box.extents.y, box.extents.z);
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