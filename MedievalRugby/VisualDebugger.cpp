#include <vector>
#include <math.h>
#include "HighResTimer.h"
#include "VisualDebugger.h"
#include "Extras\Camera.h"
#include "Extras\Renderer.h"
#include "Extras\HUD.h"

// Visual debugger namespace
namespace VisualDebugger
{
	// Using physx namespace
	using namespace physx;

	// Simulation objects
	// Camera
	Camera* camera;

	// Timer
	HighResTimer renderTimer;
	float renderTime;
	float lastRenderTime;

	// Timer
	HighResTimer simulationTimer;
	float simTime;
	float lastSimTime;

	// Physics engine
	PhysicsEngine::GameScene* scene;
	vector<PhysicsEngine::GameScene*> extraScenes;

	// Timestep
	PxReal delta_time = 1.0f / 60.0f;

	// Force strength
	PxReal gForceStrength = 20;

	// Render mode
	RenderMode render_mode = NORMAL;

	// Keys
	const int MAX_KEYS = 256;

	// Key presses
	bool key_state[MAX_KEYS];

	// SHow hud
	bool hud_show = true;

	// Hud
	HUD hud;

	// Frame count
	static int updateCount	= 0;
	static int frame		= 0;
	static int time;
	static int timebase		= 0;
	static float fps		= 0.0f;
	static int objectcount  = 0;

	//Init the debugger
	void Init(const char *window_name, int width, int height)
	{
		// Init PhysX
		PhysicsEngine::PxInit();
		scene = new PhysicsEngine::GameScene();
		scene->Init();

		// Init renderer
		Renderer::BackgroundColor(PxVec3((150.0f / 255.0f), (150.0f / 255.0f), (150.0f / 255.0f)));
		Renderer::SetRenderDetail(40);
		Renderer::InitWindow(window_name, width, height);
		Renderer::Init();

		// Make a camera
		camera = new Camera(PxVec3(0.0f, 25.0f, 60.0f), PxVec3(0.0f, -0.35f, -1.0f), 50.0f);

		// Initialise HUD
		HUDInit();
		hud_show = true;

		// Assign callbacks
		// Render
		glutDisplayFunc(RenderScene);

		// Keyboard
		glutKeyboardFunc(KeyPress);
		glutSpecialFunc(KeySpecial);
		glutKeyboardUpFunc(KeyRelease);

		// Mouse
		glutMouseFunc(mouseCallback);
		glutMotionFunc(motionCallback);

		// Exit
		atexit(exitCallback);

		// Init motion callback
		motionCallback(0, 0);
	}

	// Initialise the hud
	void HUDInit()
	{
		// Initialise HUD
		// Add a game screen
		hud.AddLine(GAME, "Medieval Rugby");
		hud.AddLine(GAME, "Score: ");
		hud.AddLine(GAME, "Shot Power: ");
		hud.AddLine(GAME, "Ball Count: ");
		hud.AddLine(GAME, "Castles Destroyed: ");
		hud.AddLine(GAME, "Render Time: ");
		hud.AddLine(GAME, "Simulation Time: ");
		hud.AddLine(GAME, "FPS: ");
		hud.AddLine(GAME, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPress 'F4' to switch to help HUD");

		// Add a help screen
		hud.AddLine(HELP, "Press 'C' to increase power of shot");
		hud.AddLine(HELP, "Press 'V' to shoot");
		hud.AddLine(HELP, "Press 'T' to move forward");
		hud.AddLine(HELP, "Press 'G' to move backward");
		hud.AddLine(HELP, "Press 'F' to move / rotate left");
		hud.AddLine(HELP, "Press 'H' to move / rotate right");
		hud.AddLine(HELP, "Press 'P' to switch camera controls");
		hud.AddLine(HELP, "Press 'N' to spawn balls");
		hud.AddLine(HELP, "Press 'M' to create a new game scene");
		hud.AddLine(HELP, "Press 'J' to delete a newly created game scene");
		hud.AddLine(HELP, "W / A /S / D / E / Q / Mouse for free camera controls ");
		hud.AddLine(HELP, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nHow to play:\n - Destroy the castles by hitting the coloured targets above them.\n - Only blue balls can hit blue targets and red balls red targets.\n - Destroying a castle opens the draw bridge a small amount.\n - Hit the ball between the goal posts to score a goal.\n - Use the keys listed above to control the kicking machine.");
		hud.AddLine(HELP, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nPress 'F4' to switch to game HUD");

		// Set font size for all screens
		hud.FontSize(0.025f);

		// Set font color for all screens
		hud.Color(PxVec3(0.0f, 0.0f, 0.0f));
	}

	// Start the main loop
	void Start()
	{ 
		glutMainLoop(); 
	}

	// Render the scene and perform a single simulation step
	void RenderScene()
	{
		// Follow camera
		if (scene->kickerBase != nullptr && scene->followPlayer)
		{
			PxQuat rot = ((PxRigidBody*)scene->kickerBase->Get())->getGlobalPose().q;
			camera->setDir(-PxVec3(rot.y, rot.x, rot.z) + PxVec3(0.0f, -0.35f, -1.0f));
			camera->setEye(((PxRigidBody*)scene->kickerBase->Get())->getGlobalPose().p + PxVec3(0.0f, 15.0f, 25.0f));
		}

		// Reset timer
		renderTimer.ResetHighResTimer();

		// Handle pressed keys
		KeyHold();

		// Start rendering
		Renderer::Start(camera->getEye(), camera->getDir());

		// Set the the render mode - debug
		if ((render_mode == DEBUG) || (render_mode == BOTH))
		{
			Renderer::Render(scene->Get()->getRenderBuffer());
		}

		// Set the render mode - normal
		if ((render_mode == NORMAL) || (render_mode == BOTH))
		{
			std::vector<PxActor*> actors = scene->GetAllActors();
			if (actors.size()) Renderer::Render(&actors[0], (PxU32)actors.size());
		}

		// Set the hud score
		int score = scene->Score();
		hud.SetScore(GAME, "Score: " + to_string(score));

		// Set the hud power 
		float power = scene->Power();
		hud.SetPower(GAME, "Shot Power: " + hud.RemoveZero(to_string(roundf(power * 1000) / 1000)));

		// Set the hud balls 
		int balls = scene->Balls();
		if (balls == 0) hud.SetBalls(GAME, "Last Ball");
		else hud.SetBalls(GAME, "Balls Remaining: " + to_string(balls + 1));

		// Set the hud balls 
		int castles = scene->DestroyedCastles();
		hud.SetCastles(GAME, "Castles Destroyed: " + to_string(castles) + "/4");

		// FPS
		frame++;
		time = glutGet(GLUT_ELAPSED_TIME);
		if (time - timebase > 1000) 
		{
			fps = frame * 1000.0 / (time - timebase);
			timebase = time;
			frame = 0;
		}

		// Update the counter
		updateCount++;
		if (updateCount > 6)
		{
			// Set the render timer
			hud.SetRenderTimer(GAME, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nInfo:\nLast Render Time: " + hud.RemoveZero(to_string(roundf((lastRenderTime / 1000.0f) * 1000) / 1000)) + " ms");

			// Set the simulation timer
			hud.SetSimTimer(GAME, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nLast Simulation Time: " + hud.RemoveZero(to_string(roundf((lastSimTime / 1000.0f) * 1000) / 1000)) + " ms");

			// Reste the counter
			updateCount = 0;

			// Set the simulation timer
			float total = lastSimTime + lastRenderTime;
			float renderPercentOfUpdate = lastRenderTime / total;
			float simPercentOfUpdate = lastSimTime / total;
			hud.SetDebugInfo
			(
				GAME, "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nRender (%): " 
				+ hud.RemoveZero(to_string(roundf(renderPercentOfUpdate * 1000) / 1000))
				+ "%\nSimulation (%): " 
				+ hud.RemoveZero(to_string(roundf(simPercentOfUpdate * 1000) / 1000))
				+ "%\nFPS: " 
				+ hud.RemoveZero(to_string(fps))
				+ "\nObject count in this scene: " 
				+ to_string(scene->Objects())
				+ "\nGame scene count: "
				+ to_string(extraScenes.size() + 1)
			);
		}


		// Adjust the HUD state
		if (!hud_show) // F4
		{
			hud.ActiveScreen(HELP);
		}
		else hud.ActiveScreen(GAME);

		// Render HUD
		hud.Render();

		// Finish rendering
		Renderer::Finish();

		// End of render
		lastRenderTime = renderTime;
		renderTime = renderTimer.GetHighResTimer();

		// Reset the timer
		simulationTimer.ResetHighResTimer();

		// Perform a single simulation step
		scene->Update(delta_time);

		// If the extra scene exsists update it!
		for (int i = 0; i < extraScenes.size(); i++)
		{
			if (extraScenes[i] != nullptr)
				extraScenes[i]->Update(delta_time);
		}

		// End of simulation
		lastSimTime = simTime;
		simTime = simulationTimer.GetHighResTimer();
	}

	// User defined keyboard handlers
	void UserKeyPress(int key)
	{
		switch (toupper(key))
		{
		case 'C': scene->KeyPressHandler(toupper(key)); break;
		case 'V': scene->KeyPressHandler(toupper(key)); break;
		case 'B': scene->KeyPressHandler(toupper(key)); break;
		case 'N': scene->KeyPressHandler(toupper(key)); break;
		case 'M': 
			extraScenes.push_back(new PhysicsEngine::GameScene());
			extraScenes.back()->Init();
			break;
		case 'J':
			if (extraScenes.size() != 0)
			{
				extraScenes.back()->Release();
				delete extraScenes.back();
				extraScenes.back() = NULL;
				extraScenes.pop_back();
			}
			break;
		case 'P': scene->KeyPressHandler(toupper(key)); break;
		default: break;
		}
	}

	// User defined keyboard handlers
	void UserKeyRelease(int key)
	{
		switch (toupper(key))
		{
		case 'C': scene->KeyReleaseHandler(toupper(key)); break;
		case 'V': scene->KeyReleaseHandler(toupper(key)); break;
		case 'B': scene->KeyReleaseHandler(toupper(key)); break;
		case 'T': scene->KeyReleaseHandler(toupper(key)); break;
		case 'F': scene->KeyReleaseHandler(toupper(key)); break;
		case 'G': scene->KeyReleaseHandler(toupper(key)); break;
		case 'H': scene->KeyReleaseHandler(toupper(key)); break;
		default: break;
		}
	}

	// User defined key held down handler
	void UserKeyHold(int key)
	{
		switch (toupper(key))
		{
		case 'C': scene->KeyHoldHandler(toupper(key)); break;
		case 'T': scene->KeyHoldHandler(toupper(key)); break;
		case 'F': scene->KeyHoldHandler(toupper(key)); break;
		case 'G': scene->KeyHoldHandler(toupper(key)); break;
		case 'H': scene->KeyHoldHandler(toupper(key)); break;
		default: break;
		}
	}

	// Handle camera control keys
	void CameraInput(int key)
	{
		// Switch on key
		switch (toupper(key))
		{
		case 'W': camera->MoveForward(delta_time);	break;
		case 'S': camera->MoveBackward(delta_time); break;
		case 'A': camera->MoveLeft(delta_time);		break;
		case 'D': camera->MoveRight(delta_time);	break;
		case 'E': camera->MoveUp(delta_time);		break;
		case 'Q': camera->MoveDown(delta_time);		break;
		default: break;
		}
	}

	// Handle force control keys
	void ForceInput(int key)
	{
	}

	// Handle special keys
	void KeySpecial(int key, int x, int y)
	{
		// Switch on key
		switch (key)
		{
		// Display control
		// Hud on/off
		case GLUT_KEY_F4: hud_show = !hud_show; break;

		// Shadows on/off
		case GLUT_KEY_F5: Renderer::ShowShadows(!Renderer::ShowShadows()); break;

		// Toggle render mode
		case GLUT_KEY_F6: ToggleRenderMode(); break;

		// Reset camera view
		case GLUT_KEY_F7: camera->Reset(); break;

		// Simulation control
		// Select next actor
		case GLUT_KEY_F8: scene->SelectNextActor(); break;

		// Select previous actor
		case GLUT_KEY_F9: scene->SelectNextActor(); break;

		// Toggle scene pause
		case GLUT_KEY_F10: scene->Pause(!scene->Pause()); break;
			
		// Resect scene
		case GLUT_KEY_F12: scene->Reset(); break;
		default: break;
		}
	}

	// Handle single key presses
	void KeyPress(unsigned char key, int x, int y)
	{
		// Do it only once
		if (key_state[key] == true) return;

		// Set the key state
		key_state[key] = true;

		// Exit
		if (key == 27) exit(0);

		// User pressed a key
		UserKeyPress(key);
	}

	// Handle key release
	void KeyRelease(unsigned char key, int x, int y)
	{
		key_state[key] = false;
		UserKeyRelease(key);
	}

	// Handle holded keys
	void KeyHold()
	{
		// Loop through the keys
		for (int i = 0; i < MAX_KEYS; i++)
		{
			// If key down
			if (key_state[i]) 
			{
				// Camera controls - send keys
				CameraInput(i);

				// Force control - send keys
				ForceInput(i);

				// User controls - send keys
				UserKeyHold(i);
			}
		}
	}

	// Mouse handling
	int mMouseX = 0;
	int mMouseY = 0;

	// Mouse motion
	void motionCallback(int x, int y)
	{
		// Get the mouse motion from last frame
		int dx = mMouseX - x;
		int dy = mMouseY - y;

		// Send mouse motion to camera
		camera->Motion(dx, dy, delta_time);

		// Reset the mouse x/y
		mMouseX = x;
		mMouseY = y;
	}

	// Mouse callback
	void mouseCallback(int button, int state, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
	}

	// Toggle the render mode
	void ToggleRenderMode()
	{
		// Set the render mode
		if (render_mode == NORMAL)		render_mode = DEBUG;
		else if (render_mode == DEBUG)	render_mode = BOTH;
		else if (render_mode == BOTH)	render_mode = NORMAL;
	}

	// Exit callback
	void exitCallback(void)
	{
		delete camera;
		delete scene;
		PhysicsEngine::PxRelease();
	}
}