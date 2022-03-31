/// @author YellowAfterlife and sam-k0
#include "stdafx.h"
#include "functions.h"



#pragma region Functions to set references to gmlAddresses
dllx gmbool addr_instance_create(GMLClosure* addr) // instance_create
{
	gmlAddresses->instance_create_addr = addr;
	return gmtrue;
}

dllx gmbool addr_show_message(GMLClosure* addr) //show_message
{
	gmlAddresses->show_message_addr = addr;
	return gmtrue;
}

dllx gmbool addr_variable_global_set(GMLClosure* addr) // variable_global_set
{
	gmlAddresses->variable_global_set = addr;
	return gmtrue;
}

#pragma endregion

#pragma region Specific functions 

/**
 * Functions here are exposed DLL functions and can be called from GameMaker or whatever program.
 * Define your functions and stuff here, placeholder function is here:
 */
dllx gmbool placeholder()
{
	GMLVar result = 0;		// Result is defaulting 0 
	GMLVar args[] = {NULL}; // TODO: Set args fitting
	gmlAddresses->instance_create_addr->ncall(result, sizeof(args), args); // Call the thing from address table
	return result.getReal(); //TODO: Change this to fitting type of the original GML function...
}

dllx gmbool spawnObjects()
{
	
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