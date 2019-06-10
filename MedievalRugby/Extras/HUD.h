#pragma once

#include "Renderer.h"
#include <vector>
#include <string>
#include <list>

// Visual debugger namespace
namespace VisualDebugger
{
	// Using the std namespace
	using namespace std;

	// A single HUD screen
	class HUDScreen
	{
		// The hud content
		vector<string> content;

	public:
		// Constructor
		HUDScreen(int screen_id, const PxVec3& _color = PxVec3(1.0f, 1.0f, 1.0f), const PxReal& _font_size = 0.024f);

		// Add a single line of text
		void AddLine(string line);

		//  Set the score
		void SetScore(string sorceText);

		// Set the power
		void SetPower(string powerText);

		// Set the power
		void SetBalls(string ballsText);

		// Set the balls
		void SetCastles(string castles);

		// Set the render timer
		void SetRenderTimer(string timer);

		// Set the simulation timer
		void SetSimTimer(string timer);

		// Set the debug info
		void SetDebugInfo(string info);

		// Render the screen
		void Render();

		// Clear content of the screen
		void Clear();

		// The id 
		int id;

		// The font size
		PxReal font_size;

		// Colour
		PxVec3 color;
	};

	// HUD class containing multiple screens
	class HUD
	{
		// The active screen
		int active_screen;

		// Screens
		vector<HUDScreen*> screens;

	public:
		// Destructor
		~HUD();

		// Add a single line to a specific screen
		void AddLine(int screen_id, string line);

		// Set the score
		void SetScore(int screen_id, string score);

		// Set the power
		void SetPower(int screen_id, string power);

		// Set balls
		void SetBalls(int screen_id, string balls);

		// Set the balls
		void SetCastles(int screen_id, string castles);

		// Set render timer
		void SetRenderTimer(int screen_id, string timer);

		// Set the simulation timer
		void SetSimTimer(int screen_id, string timer);

		// Set the debug info
		void SetDebugInfo(int screen_id, string info);

		// Set the active screen
		void ActiveScreen(int value);

		// Get the active screen
		int ActiveScreen();

		// Clear a specified screen (or all of them)
		void Clear(int screen_id = -1);

		// Change the font size for a specified screen (-1 = all)
		void FontSize(PxReal font_size, unsigned int screen_id = -1);

		// Change the color for a specified screen (-1 = all)
		void Color(PxVec3 color, unsigned int screen_id = -1);

		// Render the active screen
		void Render();

		// Remove trailing zeros
		string RemoveZero(string str);
	};
}