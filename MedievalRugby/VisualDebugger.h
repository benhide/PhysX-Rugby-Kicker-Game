#pragma once
#include "Game.h"
#include <iostream> 

// The visual debugger namespace
namespace VisualDebugger
{
	// PhysX namespace
	using namespace physx;

	// Remnder mode enum
	enum RenderMode
	{
		DEBUG,
		NORMAL,
		BOTH
	};

	// Hud state enum
	enum HUDState
	{
		EMPTY = 0,
		HELP = 1,
		PAUSE = 2,
		GAME = 3
	};

	// Function declarations - key press
	void KeyHold();
	void KeySpecial(int key, int x, int y);
	void KeyRelease(unsigned char key, int x, int y);
	void KeyPress(unsigned char key, int x, int y);

	// Function declarations - mouse movement
	void motionCallback(int x, int y);
	void mouseCallback(int button, int state, int x, int y);
	void exitCallback(void);

	// Scene functions
	void RenderScene();
	void ToggleRenderMode();
	void HUDInit();

	// Init visualisation
	void Init(const char *window_name, int width = 512, int height = 512);

	// Start visualisation
	void Start();
}