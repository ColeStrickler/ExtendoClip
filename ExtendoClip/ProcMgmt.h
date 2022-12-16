#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include <winternl.h>
#include <Psapi.h>




// UTILS
#include "PebUtils.h"


namespace proc {
	PEB* GetPEB();
	BOOL ProcIdExists(DWORD procId);
	std::wstring GetProcnameFromId(DWORD procId);
	DWORD GetProcId(wchar_t* procName);
	uintptr_t GetBaseAddress(DWORD procId, wchar_t* modName);
	void getNameCurrentProc(wchar_t* out);
}

