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

	int numVertex = nMeridians * nParallels * 2;

	if (rlCheckBufferLimit(numVertex)) rlglDraw();
	// BEGINNING OF SPACE TRANSFORMATION INDUCED BY THE LOCAL REFERENCE FRAME
		// methods should be called in this order: rlTranslatef, rlRotatef & rlScalef
		// so that transformations occur in the opposite order: scale, then rotation, then translation
	rlPushMatrix();
	//TRANSLATION


	rlBegin(RL_TRIANGLES);
	rlColor4ub(color.r, color.g, color.b, color.a);




	//for (int i = 0; i < nMeridians/2; i++) {
	//	for (int j = 0; j < nParallels; j++) {
	//		// First Vertex
	//		// First Point
	//		Spherical sph = { sphere.radius, (2*PI / nParallels) * j, (PI / nMeridians) * i };
	//		Vector3 cart = SphericalToCartesian(sph);
	//		rlVertex3f(cart.x, cart.y, cart.z);

	//		// Second Point
	//		
	//	}
	//	

	//}
	// 
	// 
	// regarder le code suivant pour la sphère 
	/*for (int i = 0; i < (rings + 2); i++)
	{
		for (int j = 0; j < slices; j++)
		{
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * (j * 360 / slices)));

			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * sinf(DEG2RAD * (j * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * i)),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * i)) * cosf(DEG2RAD * (j * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
			rlVertex3f(cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * sinf(DEG2RAD * ((j + 1) * 360 / slices)),
				sinf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))),
				cosf(DEG2RAD * (270 + (180 / (rings + 1)) * (i + 1))) * cosf(DEG2RAD * ((j + 1) * 360 / slices)));
		}
	}*/
	rlEnd();





}
void MyDrawWireframeSphere(Sphere sphere, int nMeridians, int nParallels, Color color = DARKGRAY) {

}


void MyDrawSphere(Sphere sphere, int nMeridians, int nParallels, bool drawPolygon = true, bool drawWireframe = true, Color polygonColor = LIGHTGRAY, Color wireframeColor = DARKGRAY) {
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