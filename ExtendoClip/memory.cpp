#include "memory.h"
std::vector<BYTE> mem::patch(BYTE* dst, BYTE* src, size_t count)
{
    BOOL success;
    DWORD oldProtect;
    std::vector<BYTE> ret;

    for (int i = 0; i < count; i++) {
        ret.push_back(dst[i]);
    }

    success = VirtualProtect(dst, count, PAGE_EXECUTE_READWRITE, &oldProtect);
    if (!success) {
        ret.clear();
        return ret;
    }
    memcpy(dst, src, count);
    success = VirtualProtect(dst, count, oldProtect, &oldProtect);
    if (!success) {
        ret.clear();
        return ret;
    }

    return ret;
}




std::vector<BYTE> mem::nop(BYTE* dst, size_t count)
{
    BOOL success;
    DWORD oldProtect;
    std::vector<BYTE> ret;

    for (int i = 0; i < count; i++) {
        ret.push_back(dst[i]);
    }

    success = VirtualProtect(dst, count, PAGE_EXECUTE_READWRITE, &oldProtect);
    if (!success) {
        ret.clear();
        return ret;
    }
    memset(dst, 0x90, count);
    success = VirtualProtect(dst, count, oldProtect, &oldProtect);
    if (!success) {
        ret.clear();
        return ret;
    }

    return ret;
}




std::vector<MEMORY_BASIC_INFORMATION> mem::GetMemoryInfo(HANDLE hProc)
{
    std::vector<MEMORY_BASIC_INFORMATION> MemInfo;

    HMODULE hMod = GetModuleHandle(L"ntdll.dll");
    if (!hMod) {
        return MemInfo;
    }
    NtQueryVirtualMemoryPtr NtQueryVirtualMemory = (NtQueryVirtualMemoryPtr)GetProcAddress(hMod, "NtQueryVirtualMemory");
    if (!NtQueryVirtualMemory) {
        return MemInfo;
    }

    MEMORY_BASIC_INFORMATION memory_information;
    uintptr_t current_address = 0;
    SIZE_T return_length;



    for (current_address = 0; NtQueryVirtualMemory(hProc, (PVOID)current_address, MemoryBasicInformation, &memory_information, 0x30, &return_length) >= 0; \
        current_address = (uintptr_t)memory_information.BaseAddress + memory_information.RegionSize)
    {
        MemInfo.push_back(memory_information);
    }


    return MemInfo;
}



uintptr_t mem::ResolvePtrChain(uintptr_t base, std::vector<uintptr_t> offsets)
{
    uintptr_t ret = base;

    for (auto& offset: offsets) {
        ret = *(uintptr_t*)ret;
        ret += offset;
    }

    return ret;
}


uintptr_t mem::GetFuncAddress(LPCSTR ModuleName, LPCSTR FunctionName)
{
    uintptr_t ret = 0;
    HMODULE hModule = GetModuleHandleA(ModuleName);
    if (!hModule) {
        return ret;
    }
    return (uintptr_t)GetProcAddress(hModule, FunctionName);
}


uintptr_t mem::GetBaseAddress(wchar_t* modName) {
    uintptr_t modBase = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
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