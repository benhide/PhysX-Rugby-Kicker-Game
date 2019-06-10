#include "Exception.h"

// Constructor
Exception::Exception(const std::string message) : message(message) 
{
}

// What error message
const std::string& Exception::what()
{
	return message;
}