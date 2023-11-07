
#include <cpp-utils/Winutil.h>

#include <iostream>

#include "tests.h"

#ifndef _LIB

#include <Windows.h>
#include <cpputils.h>

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH:
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:

			break;
	}
	return TRUE;
}

#endif

// int main()
//{
//	// test_queue_manager();
//	test_logger();
//	// test_namedpipe();
//
//	getchar();
//	return 0;
// }
