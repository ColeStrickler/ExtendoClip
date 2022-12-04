#pragma once
#include <Windows.h>
#include <lm.h>
#include <sddl.h>
#include <vector>
#include <string>
#include <cassert>


std::vector<std::wstring> listUsers();
std::string GetCurrentSid();