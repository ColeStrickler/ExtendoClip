#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>
#include <math.h>
#include <string>

// INTERNAL TYPEDEFS
typedef enum _MEMORY_INFORMATION_CLASS {
	MemoryBasicInformation
} MEMORY_INFORMATION_CLASS;
typedef NTSTATUS(__stdcall* NtQueryVirtualMemoryPtr)(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);








namespace mem {	
	std::vector<BYTE> patch(BYTE* dst, BYTE* src, size_t count);
	std::vector<BYTE> nop(BYTE* dst, size_t count);
	std::vector<MEMORY_BASIC_INFORMATION> GetMemoryInfo(HANDLE hProc);
	uintptr_t GetFuncAddress(LPCSTR ModuleName, LPCSTR FunctionName);
	uintptr_t GetBaseAddress(wchar_t* modName);
	uintptr_t ResolvePtrChain(uintptr_t base, std::vector<uintptr_t> offsets);
}
