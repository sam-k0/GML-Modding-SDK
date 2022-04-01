/// @author YellowAfterlife and sam-k0
#include "stdafx.h"
#include "functions.h"

// Init function addresses
/**
 * This part initializes the pointers to functions stored in gmlAddresses (GMAddressTable)
 * I thought well maybe it's smarter to just have 1 function for everything LOL
 *
 */
dllx gmbool init_function_address(const char* key, GMLClosure* address)
{
	bool res = gmlAddresses->addFunction(key, address);
	if (res)
	{
		return gmtrue;
	}
	return gmfalse;
}


#pragma region Specific functions 

/**
 * Functions here are exposed DLL functions and can be called from GameMaker or whatever program.
 * Define your functions and stuff here
 */

// This will hopefully create a new object of type obj at 32,32
dllx gmbool spawnObjects(gmobject obj)
{
	gml::instance_create(32, 32, obj);
	return gmtrue;
}

// Old
dllx double test_message(GMLClosure* show_message) {
	GMLVar result = 0; // Set result to false

	GMLVar args[] = { "Hello from C++!" }; // Set args

	show_message->ncall(result, sizeof(args), args); // Set parameters and call

	return result.getReal();
}

dllx double test_create(GMLClosure* instance_create, GMLClosure* variable_global_set, double obj) {
	GMLVar result = 0; // set result
	// Set the variable "global.greet" to String "C++"
	GMLVar args1[] = { "greet", "C++" };
	// Call variable_global_set to set the variable
	variable_global_set->ncall(result, sizeof(args1), args1);
	// Call 
	GMLVar args2[] = { 0, 0, obj };
	instance_create->ncall(result, sizeof(args2), args2);
	//
	return result.getReal();
}
#pragma endregion