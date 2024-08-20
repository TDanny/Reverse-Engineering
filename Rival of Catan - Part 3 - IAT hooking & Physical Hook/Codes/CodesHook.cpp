#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;
using std::endl;


//Global variable that will give us indicetion if we enter strcmp for the first time or the second time
INT NUM_STRCMP = 0;

// #include <fstream> 
// using std::ofstream;
//ofstream log_file("log.txt");

// Typedef for the hooked function signature, such as:
//typedef INT(WINAPI* FUNC_PTR)(const char*, const char*);
typedef LPVOID FUNC_PTR;
// Ptr to the original function
FUNC_PTR original_strcmp;

// Global variables
DWORD lpProtect = 0;
LPVOID IAT;
FUNC_PTR JumpTo;
/*
// Helper function to remove the hook (for the current call):
void _stdcall remove_hook() {
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT, &original_recv, 0x4);
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READ, &lpProtect);
}
*/
// Helper function to restore the hook (for the next calls)
void _stdcall restore_hook() {
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READWRITE, &lpProtect);
	memcpy(IAT, &JumpTo, 0x4);
	VirtualProtect((char*)IAT, 0x4, PAGE_EXECUTE_READ, &lpProtect);
}


int strcmpHook(const char* string1, const char* string2) {
	NUM_STRCMP++;
	//std::cout << "val of static : " << NUM_STRCMP << endl;
	if (NUM_STRCMP == 1) {
		//std::cout << "first time in strcmp" << endl;
		return 1;
	}
	if (NUM_STRCMP == 2) {
		//std::cout << "second time in strcmp" << endl;
		return 0;
	}
	//else just do strcmp regulary
	return strcmp(string1, string2);
}

void setHook() {
	HMODULE prog_handle = GetModuleHandle(NULL);
	HMODULE target_dll = GetModuleHandle(L"msvcrt.dll");

	if ((prog_handle == NULL) || (target_dll == NULL)) {
		std::cout << "Program doesn't exist OR DLL doesn't exist" << endl;
		return;
	}

	original_strcmp = (FUNC_PTR)GetProcAddress(target_dll, "strcmp");
	if (original_strcmp == NULL) {
		std::cout << "Process couldn't find function" << endl;
		return;
	}

	int addr_beginning_of_our_exe = 0x400000;
	int addr_func_to_hook_in_IAT = 0x409294;
	IAT = prog_handle + (addr_func_to_hook_in_IAT - addr_beginning_of_our_exe) / 4;

	JumpTo = (FUNC_PTR)((char*)&strcmpHook);
	restore_hook();
}


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		setHook();
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}