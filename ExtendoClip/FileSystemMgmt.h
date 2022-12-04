#pragma once
#include <Windows.h>
#include <string>
#include <fstream>
#include <filesystem>

BOOL DirExists(LPCSTR szPath);
BOOL FileExists(LPCTSTR szPath);
std::string ReadFileToStringA(std::string path);

