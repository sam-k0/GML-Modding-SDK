/// @author YellowAfterlife and sam-k0
#include "stdafx.h"
#include "functions.h"
#define FUNCTION_NOT_FOUND ""

#ifdef NATIVE_GAME_SDK // We do not need this code for "just modding"
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

// Get the address of a saved function
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

// Execute patching & checking address table loading
#ifndef NATIVE_GAME_SDK
// The code here is compiled when using modding config
bool executePatching() // This is where your patch code goes.
{
	gml::show_message("Hello from C!"); 
	GMLVar val = gml::variable_instance_get(100001, "sus"); // Instance IDs are pretty handy
	gml::show_debug_message("Sus value is " + to_string(val.getReal()));
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
#endif //NATIVE_GAME_SDK

/**
 * Entry point for the DLL.
 */
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	int res; // Result of trying to load the address table


	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // Do the thing here if necessary
		std::cout << "---GML-Modding-SDK---"
				  << "[GMLMOS] Compiled at " __TIMESTAMP__ << std::endl;
		#ifdef NATIVE_GAME_SDK // This changes with configuration: config.h
		{
			std::cout << "[GMLMOS] Loaded NATIVE" << std::endl;
		}
		#else // Modding config
		{
			
			std::cout << "[GMLMOS] Loaded DLL as MOD" << std::endl;
			std::cout << "[GMLMOS] Connection Code: " << address_table_loaded() << " (0 is success)" << std::endl;
			// Call the script
			res = address_table_loaded();
			if (res == 0) // No error when loading addr table
			{
				std::cout << "[GMLMOS] AddressTable found!" << std::endl;
				executePatching(); // Execute the patching method
			}
			else // Failed to load address table, abort loading
			{
				std::cout << "[GMLMOS] AddressTable failed to load. Exit with Code " << res << std::endl;
				return FALSE;
			}
		}
		#endif //NATIVE_GAME_SDK
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

