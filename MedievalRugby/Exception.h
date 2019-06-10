#pragma once
#include <string>

// Inherit from this class
class Exception
{
	// The error message
	std::string message;

public:
	// Constructor
	Exception(const std::string message);

	// What error message
	virtual const std::string& what();
};