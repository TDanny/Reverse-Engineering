#include "pch.h"
#include <Windows.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <WinSock2.h>
#define MESSAGE_LEN 640

using namespace std;
using std::endl;



// #include <fstream> 
// using std::ofstream;
//ofstream log_file("log.txt");

// Typedef for the hooked function signature, such as:
typedef INT(WINAPI* FUNC_PTR)(SOCKET, char*, int, int); //(WINAPI is for _stdcall)

// Ptr to the original function
FUNC_PTR original_recv;

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

int HexToAscii(char hex_message[], char* buf) {
	int len = strlen(hex_message);
	int index = 0;
	for (int i = 0; i < len; i += 2)
	{
		string High_4bits(1,hex_message[i]);
		string Low_4bits(1, hex_message[i + 1]);
		string byte = High_4bits + Low_4bits;
		char ch = (char)stoul(byte, nullptr, 16);
		buf[index++] = ch;
	}
	//std::cout << "finished decryption" << endl;
	buf[index++] = '\0';
	return index;
}

char identifyDEF(const string& buf, int i) {
	//sum the string and we will get the hex of the char
	int ans = (buf[i] - '0');
	ans += (buf[i + 2] - '0');
	if (ans == 0x0d)
		return 'D';
	else if (ans == 0x0e)
		return 'E';
	else if (ans == 0x0f)
		return 'F';
	else
		return 'F';
}

INT WINAPI recvHook(SOCKET s, char* buf, int len, int flags) {
	
	int recive = original_recv(s, buf, len, flags);
	char decrypt_message[MESSAGE_LEN] = { 0 };
	int index = 0;
	for (int i = 0; i < strlen(buf); i++) {
		switch (buf[i]) {
		case 'A':
			decrypt_message[index] = '1';
			index++;
			break;
		case 'J':
			decrypt_message[index] = 'A';
			index++;
			break;
		case 'Q':
			decrypt_message[index] = 'B';
			index++;
			break;
		case 'K':
			decrypt_message[index] = 'C';
			index++;
			break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			//std::cout << "im here - " << i << endl;
			if (buf[i] != '\0') {
				// Incase of '+' this indicates on: D/E/F chars
				if (buf[i + 1] == '+') {
					decrypt_message[index] = identifyDEF(buf, i);
					index++;
					i += 2;
					break;
				}
				//Incase of '-' means 0
				else if (buf[i + 1] == '-') {
					decrypt_message[index] = '0';
					index++;
					i += 2;
					break;
				}
			}
			decrypt_message[index] = buf[i];
			index++;
		}
	}
	//return amount of chars in data recieved from server
	return HexToAscii(decrypt_message, buf);

}

void setHook() {
	HMODULE prog_handle = GetModuleHandle(L"client.exe");
	HMODULE target_dll = GetModuleHandle(L"Ws2_32.dll");

	if ((prog_handle == NULL) || (target_dll == NULL)) {
		std::cout << "Program doesn't exist OR DLL doesn't exist" << endl;
		return;
	}

	original_recv = (FUNC_PTR)GetProcAddress(target_dll, "recv");
	if (original_recv == NULL) {
		std::cout << "Process couldn't find function" << endl;
		return;
	}

	int addr_beginning_of_our_exe = 0x400000;
	int addr_func_to_hook_in_IAT = 0x40A2AC;
	IAT = prog_handle + (addr_func_to_hook_in_IAT - addr_beginning_of_our_exe) / 4;

	JumpTo = (FUNC_PTR)((char*)&recvHook);
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