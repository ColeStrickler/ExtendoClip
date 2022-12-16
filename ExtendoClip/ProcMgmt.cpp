#include "ProcMgmt.h"



PEB* proc::GetPEB() {
#ifdef _WIN64
	PEB* peb = (PEB*)__readgsqword(0x60);
	return peb;
#else 
	PEB2* peb = (PEB2*)__readfsdword(0x30);
	return peb;
#endif
}



BOOL proc::ProcIdExists(DWORD procId)
{
	BOOL ret = false;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (procEntry.th32ProcessID == procId)
				{
					procId = true;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}


std::wstring proc::GetProcnameFromId(DWORD procId)
{
	std::wstring ret;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (procEntry.th32ProcessID == procId)
				{
					ret = std::wstring(procEntry.szExeFile);
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return ret;
}


DWORD proc::GetProcId(wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (!_wcsicmp(procEntry.szExeFile, procName))
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}
	CloseHandle(hSnap);
	return procId;
}

uintptr_t proc::GetBaseAddress(DWORD procId, wchar_t* modName) { 


	uintptr_t modBase = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap && hSnap != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnap, &modEntry)) {
			do {
				if (!_wcsicmp(modEntry.szModule, modName)) {
					modBase = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
		CloseHandle(hSnap);
		return modBase;
	}
	else {
		return NULL;
	}
}

void proc::getNameCurrentProc(wchar_t* out) {
	wchar_t fullPath[MAX_PATH + 1] = { 0 };
	GetProcessImageFileNameW(GetCurrentProcess(), fullPath, (DWORD)sizeof(fullPath));
	int lastSlash = 0;
	for (int i = 0; i < MAX_PATH; i++) {
		if ((wchar_t)fullPath[i] == (wchar_t)0x5c) { // NEEDS DEBUGGED RIGHT HERE
			lastSlash = i;
		}
	}
	lastSlash++;
	int i = 0;
	while (fullPath[lastSlash] != 0) {
		out[i] = fullPath[lastSlash];
		i++;
		lastSlash++;
	}
	out[i] = 0x00;
	return;
}

