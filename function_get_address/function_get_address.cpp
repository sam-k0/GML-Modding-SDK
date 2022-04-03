/// @author YellowAfterlife and sam-k0
#include "stdafx.h"
#include "functions.h"
#define FUNCTION_NOT_FOUND ""
// Init function addresses
#ifdef NATIVE_GAME_SDK
/**
 * This part initializes the pointers to functions stored in gmlAddresses (GMAddressTable)
 * I thought well maybe it's smarter to just have 1 function for everything LOL
 *
 */
dllx gmbool init_function_address(const char* key, GMLClosure* address)
{
	std::cout << "Adding function " << key << " : " << address << std::endl;
	std::string keystr = std::string(key);

	bool res = gmlAddresses->addFunction(keystr, address);
	if (res)
	{
		return gmtrue;
	}
	return gmfalse;
}

/**
 * This is called by the initialization script.
 * Do not call this manually
 */
dllx gmbool init_function_done()
{
	gmlAddresses->doneInitializing = true;
	return gmtrue;
}
#endif


dllx const char* get_function_address(const char* key)
{
	if (gmlAddresses->doneInitializing)
	{
		GMLClosure* res = gmlAddresses->getFunction(std::string(key));
		if (res == NULL)
		{
			return FUNCTION_NOT_FOUND;
		}
		else
		{
			const char* addr = (const char*)res;
			std::cout << "Returning addr " << addr << std::endl;
			return addr;
		}
	}
	else
	{
		return FUNCTION_NOT_FOUND;
	}
}

#pragma region Specific functions 

/**
 * Functions here are exposed DLL functions and can be called from GameMaker or whatever program.
 * Define your functions and stuff here
 */
// Used for debugging
dllx gmbool printAll()
{
	gmlAddresses->printAllFunctions();
	return gmtrue;
}

// This will hopefully create a new object of type obj at 32,32
dllx gmbool show_debug_message_a(const char* str)
{
	gml::show_message(std::string(str));
	return gmtrue;
}

dllx gmbool setglobal(const char* x, const char* y)
{
	gml::variable_global_set(x, y);
	return gmtrue;
}

dllx gmbool inst_create(double x, double y, double o)
{
	GMLVar args[] = { x, y, o };
	double result = gml::call_function("instance_create", args);
	return result;
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

#ifndef NATIVE_GAME_SDK
// The code here is compiled when using modding config
bool executePatching()
{
	gml::show_message("Hello from C!");
	return true;
}

// Checks wether the DLL in the game has already finished loading
int address_table_loaded()
{
	if (gmlAddresses == NULL)
	{
		return -1;
	}
	else
	{
		if (gmlAddresses->doneInitializing)
		{
			return 0;
		}
		return -2;
	}

}
#endif //Native game sdk

/**
 * Entry point for the DLL.
 */
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	int res;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // Do the thing here if necessary

#ifdef NATIVE_GAME_SDK
		std::cout << "Loaded NATIVE" << std::endl;
#else
		std::cout << "Loaded as MODDED" << std::endl;
		std::cout << "Native DLL returned code " << address_table_loaded() << std::endl;
		// Call the script
		res = address_table_loaded();
		MessageBoxA(0, std::string("RES is " + res).c_str(), "OK", MB_OK);
		if (res == 0)
		{
			MessageBoxA(0, "Address table success!", "yep", MB_OK);
			std::cout << "Found export address table!" << std::endl;
			executePatching();
		}
		

#endif // !NATIVE_GAME_SDK

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

