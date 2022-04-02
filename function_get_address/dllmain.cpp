// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "functions.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: // Do the thing here if necessary

	#ifndef NATIVE_GAME_SDK
		// if we use this to mod
		if (gmlAddresses != NULL)
		{
			delete gmlAddresses; // Delete the old thing
			gmlAddresses = new GMLAddressTable();
		}
	#endif // !NATIVE_GAME_SDK

	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

