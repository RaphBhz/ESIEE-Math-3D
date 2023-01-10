﻿/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <raymath.h>
#include "rlgl.h"
#include <math.h>
#include <float.h>
#include <vector>
#include "../../projects/VS2019/examples/My3DPrimitives.h"
#include "../../projects/VS2019/examples/Draw3D.h"
#include "../../projects/VS2019/examples/Referentials.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define EPSILON 1.e-6f

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

void MyUpdateOrbitalCamera(Camera* camera, float deltaTime)
{
	static Spherical sphPos = { 10,PI / 4.f,PI / 4.f };
	Spherical sphSpeed = { 2.0f, 0.04f, 0.04f };
	static Vector2 prevMousePos = { 0, 0 };
	float rhoMin = 4; // 4 m
	float rhoMax = 40; // 40 m

	Vector2 mousePos = GetMousePosition();
	Vector2 mouseVect = Vector2Subtract(mousePos, prevMousePos);
	prevMousePos = mousePos;

	sphPos = {
		Clamp(sphPos.rho + GetMouseWheelMove() * sphSpeed.rho, rhoMin, rhoMax),
		sphPos.theta + mouseVect.x * (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ? sphSpeed.theta : 0),
		Clamp(sphPos.phi + mouseVect.y * (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) ? sphSpeed.phi : 0), 1 * DEG2RAD, 179 * DEG2RAD)
	};

	camera->position = SphericalToCartesian(sphPos);
}

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	float screenSizeCoef = .9f;
	const int screenWidth = 1920 * screenSizeCoef;
	const int screenHeight = 1080 * screenSizeCoef;

	InitWindow(screenWidth, screenHeight, "ESIEE - E3FI - 2022 - 2023 -Maths 3D");

	SetTargetFPS(60);

	//CAMERA
	Vector3 cameraPos = { 8.0f, 15.0f, 14.0f };
	Camera camera = { 0 };
	camera.position = cameraPos;
	camera.target = { 0.0f, 0.0f, 0.0f };
	camera.up = { 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.type = CAMERA_PERSPECTIVE;
	SetCameraMode(camera, CAMERA_CUSTOM);  // Set an orbital camera mode


	//--------------------------------------------------------------------------------------

	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		float deltaTime = GetFrameTime();
		float time = (float)GetTime();

		MyUpdateOrbitalCamera(&camera, deltaTime);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		BeginMode3D(camera);
		{
			// DRAWING
			// QUAD DISPLAY TEST
			ReferenceFrame ref = ReferenceFrame(
				{ 0,1,0 },
				QuaternionFromAxisAngle(Vector3Normalize({ 0,0,0 }), 0));
			
			Quad quad = { ref,{1,0,1} };
			Box box = { ref, {1, 2, 3} };
			Disk disk = { ref, 2 };
			Sphere sphere = { ref, 4 };
			Cylinder cylinder = { ref, 3, 3 };
			Capsule capsule = { ref, 3, 3 };

			// MyDrawQuad(quad);
			// MyDrawBox(box);
			// MyDrawDisk(disk);
			// MyDrawSphere(sphere, 50, 50);
			// MyDrawCylinder(cylinder, 50);
			// MyDrawSphere(sphere, 40, 40, 0, PI, 0, PI / 2);
			MyDrawCapsule(capsule);

			//3D REFERENTIAL
			DrawGrid(20, 1.0f);        // Draw a grid
			DrawLine3D({ 0 }, { 0,10,0 }, DARKGRAY);
			DrawSphere({ 10,0,0 }, .2f, RED);
			DrawSphere({ 0,10,0 }, .2f, GREEN);
			DrawSphere({ 0,0,10 }, .2f, BLUE);
		}
		EndMode3D();

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}