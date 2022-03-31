#pragma once
#include "gml.h"


// GML - Functions
namespace gml {

	gmint instance_create(double x, double y, gmobject obj)
	{
		GMLVar result = 1;
		GMLVar args[] = { x, y, obj };
		gmlAddresses->instance_create_addr->ncall(result, sizeof(args), args);
		return result.getReal(); 
	}

	gmint show_debug_message(std::string s)
	{
		GMLVar result = 1;
		GMLVar args[] = { s.c_str() };
		gmlAddresses->show_message_addr->ncall(result, sizeof(args), args);
		return result.getReal();
	}

	gmint variable_global_set(std::string varname, std::string content ) 
	{
		GMLVar result = 1;
		GMLVar args[] = { varname.c_str(), content.c_str() };
		// Call variable_global_set to set the variable
		gmlAddresses->variable_global_set->ncall(result, sizeof(args), args);
	}
};

// Init function addresses
/**
 * This part initializes the pointers to functions stored in gmlAddresses (GMAddressTable)
 * 
 * 
 */
dllx gmbool INIT_instance_create(GMLClosure* func)
{
	gmlAddresses->instance_create_addr = func;
	return gmtrue;
}

dllx gmbool INIT_show_debug_message(GMLClosure* func)
{
	gmlAddresses->show_message_addr = func;
	return gmtrue;
}

dllx gmbool INIT_variable_global_set(GMLClosure* func)
{
	gmlAddresses->variable_global_set = func;
	return gmtrue;
}


