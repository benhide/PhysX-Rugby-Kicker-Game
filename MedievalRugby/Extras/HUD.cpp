#include "HUD.h"

// Visual debugger namespace
namespace VisualDebugger
{
	// Constructor
	HUDScreen::HUDScreen(int screen_id, const PxVec3& _color, const PxReal& _font_size) : id(screen_id), color(_color), font_size(_font_size) {}

	// Add a single line of text
	void HUDScreen::AddLine(string line)
	{
		content.push_back(line);
	}

	//  Set the score
	void HUDScreen::SetScore(string sorceText)
	{
		content[1] = sorceText;
	}

	// Set the power
	void HUDScreen::SetPower(string powerText)
	{
		content[2] = powerText;
	}

	// Set the balls
	void HUDScreen::SetBalls(string ballsText)
	{
		content[3] = ballsText;
	}

	// Set the balls
	void HUDScreen::SetCastles(string castlesText)
	{
		content[4] = castlesText;
	}

	// Set the render timer
	void HUDScreen::SetRenderTimer(string timer)
	{
		content[5] = timer;
	}

	// Set the render timer
	void HUDScreen::SetSimTimer(string timer)
	{
		content[6] = timer;
	}

	// Set the debug info
	void HUDScreen::SetDebugInfo(string info)
	{
		content[7] = info;
	}

	// Render the screen
	void HUDScreen::Render()
	{
		// Loop through the content and render
		for (unsigned int i = 0; i < content.size(); i++)
			Renderer::RenderText(content[i], PxVec2(0.0, 1.0f - (i + 1) * font_size), color, font_size);
	}

	// Clear content of the screen
	void HUDScreen::Clear()
	{
		content.clear();
	}

	// Destructor
	HUD::~HUD()
	{
		for (unsigned int i = 0; i < screens.size(); i++)
			delete screens[i];
	}

	// Add a single line to a specific screen
	void HUD::AddLine(int screen_id, string line)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->AddLine(line);
				return;
			}
		}

		screens.push_back(new HUDScreen(screen_id));
		screens.back()->AddLine(line);
	}

	// Set the score
	void HUD::SetScore(int screen_id, string score)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetScore(score);
				return;
			}
		}
	}

	// Set the power
	void HUD::SetPower(int screen_id, string power)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetPower(power);
				return;
			}
		}
	}

	// Set the balls
	void HUD::SetBalls(int screen_id, string balls)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetBalls(balls);
				return;
			}
		}
	}

	// Set the balls
	void HUD::SetCastles(int screen_id, string castles)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetCastles(castles);
				return;
			}
		}
	}

	// Set the render timer
	void HUD::SetRenderTimer(int screen_id, string timer)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetRenderTimer(timer);
				return;
			}
		}
	}

	// Set the render timer
	void HUD::SetSimTimer(int screen_id, string timer)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetSimTimer(timer);
				return;
			}
		}
	}

	// Set the render timer
	void HUD::SetDebugInfo(int screen_id, string info)
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == screen_id)
			{
				screens[i]->SetDebugInfo(info);
				return;
			}
		}
	}

	// Set the active screen
	void HUD::ActiveScreen(int value)
	{
		active_screen = value;
	}

	// Get the active screen
	int HUD::ActiveScreen()
	{
		return active_screen;
	}

	// Clear a specified screen (or all of them)
	void HUD::Clear(int screen_id)
	{
		if (screen_id == -1)
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				screens[i]->Clear();
			}
		}
		else
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				if (screens[i]->id == screen_id)
				{
					screens[i]->Clear();
					return;
				}
			}
		}
	}

	// Change the font size for a specified screen (-1 = all)
	void HUD::FontSize(PxReal font_size, unsigned int screen_id)
	{
		if (screen_id == -1)
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				screens[i]->font_size = font_size;
			}
		}
		else
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				if (screens[i]->id == screen_id)
				{
					screens[i]->font_size = font_size;
					return;
				}
			}
		}
	}

	// Change the color for a specified screen (-1 = all)
	void HUD::Color(PxVec3 color, unsigned int screen_id)
	{
		if (screen_id == -1)
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				screens[i]->color = color;
			}
		}
		else
		{
			for (unsigned int i = 0; i < screens.size(); i++)
			{
				if (screens[i]->id == screen_id)
				{
					screens[i]->color = color;
					return;
				}
			}
		}
	}

	// Render the active screen
	void HUD::Render()
	{
		for (unsigned int i = 0; i < screens.size(); i++)
		{
			if (screens[i]->id == active_screen)
			{
				screens[i]->Render();
				return;
			}
		}
	}

	// Remove trailing zeros
	string HUD::RemoveZero(string str)
	{
		// Count trailing zeros 
		int i = strlen(str.c_str()) - 1;
		int count = 0;
		while (str[i] == '0')
		{
			if (str[i] == '.')
				break;

			i--;
			count++;
		}

		// The erase function removes i characters 
		// from given index (0 here) 
		str.erase(i + 1, count - 1);

		return str;
	}
};