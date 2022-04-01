/*****************************************************************//**
 * \file   functions.h
 * \brief  GML-namespace functions are defined here.
 * 
 * \author sam-k0
 * \date   March 2022
 *********************************************************************/
#pragma once
#include "gml.h"


// GML - Functions
namespace gml {

	gmint instance_create(double x, double y, gmobject obj)
	{
		GMLVar result = 1;
		GMLVar args[] = { x, y, obj };
		GMLClosure* func = gmlAddresses->getFunction("instance_create");
		if (func == NULL)
		{
			std::cout << "ERROR! Function address not initialized!" << "instance_create" << std::endl;
			return gmfalse;
		}
		func->ncall(result, sizeof(args), args);
		return result.getReal(); 
	}

	gmint show_debug_message(std::string s)
	{
		GMLVar result = 1;
		GMLVar args[] = { s.c_str() };
		GMLClosure* func = gmlAddresses->getFunction("show_debug_message");
		if (func == NULL)
		{
			std::cout << "ERROR! Function address not initialized!" << "show_debug_message" << std::endl;
			return gmfalse;
		}
		func->ncall(result, sizeof(args), args);
		return result.getReal();
	}

	gmint variable_global_set(std::string varname, std::string content ) 
	{
		GMLVar result = 1;
		GMLVar args[] = { varname.c_str(), content.c_str() };
		// Call variable_global_set to set the variable
		GMLClosure* func = gmlAddresses->getFunction("variable_global_set");
		if (func == NULL)
		{
			std::cout << "ERROR! Function address not initialized!" << "variable_global_set" << std::endl;
			return gmfalse;
		}
		func->ncall(result, sizeof(args), args);
		return gmtrue;
	}

	// Calls a function by the specified key
	/**
	 * This is used for directly calling function from the map.
	 */
	gmint call_function(std::string key, GMLVar* args)
	{
		GMLClosure* func = gmlAddresses->getFunction(key);
		GMLVar result = 1;

		if (func != NULL)
		{
			func->ncall(result, sizeof(args), args);
		}
		return result.getReal();
	}
};




