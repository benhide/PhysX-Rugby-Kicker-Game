#include <iostream>
#include "VisualDebugger.h"

// Using the std namespace
using namespace std;

// Main execution
int main()
{
	// Try to get the visual debugger
	try 
	{ 
		VisualDebugger::Init("HID16605093 - CGP3012M - PHYSX - MEDIEVAL RUGBY", 1920, 1080);
	}

	// Error no visual debugger
	catch (Exception exc) 
	{ 
		cerr << exc.what() << endl;
		return 0; 
	}

	// Visual debugger start
	VisualDebugger::Start();

	// End
	return 0;
}