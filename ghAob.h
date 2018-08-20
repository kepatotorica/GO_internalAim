#ifndef GO_INTERNALAIM_GHAOB_H
#define GO_INTERNALAIM_GHAOB_H

#include <vector>
#include <Windows.h>
#include <psapi.h>
#pragma comment (lib, "Psapi.lib")

bool ComparePattern(DWORD address, char *pattern, char *mask) {
    int patternLen = strlen(mask);											// Get the pattern length

    for (auto i = 1; i < patternLen; i++) {									// For each byte (after the first) in the pattern
        if (mask[i] != *"?" && *(char*)(address + i) != pattern[i]) {		// Compare each byte of the pattern with each byte after the current scanning address
            return false;													// If any doesn't match return false
        }
    }

    return false;															// Return true if everything matched
}

DWORD InternalSpecificModuleAoB(char *module, char *pattern, char *mask) {
    std::vector<DWORD> matches;

    HMODULE hModule = GetModuleHandle(module);																		// Obtain a handle to the requested module
    MODULEINFO moduleInfo = { 0 };

    if (!hModule) {																									// If I couldn't obtain a handle
        return NULL;																								// Return null for patterns found in that module
    }

    GetModuleInformation(GetCurrentProcess(), hModule, &moduleInfo, sizeof(MODULEINFO));							// Store module info in moduleInfo

    DWORD moduleBase = (DWORD)moduleInfo.lpBaseOfDll;																// Starting location in memory of that module
    DWORD moduleSize = (DWORD)moduleInfo.SizeOfImage;																// Size of the module in memory

    DWORD patternSize = (DWORD)strlen(mask);																		// Size of pattern

    for (DWORD i = 0; i < moduleSize; i++) {																			// For each byte in the module
        if (*(char*)(moduleBase + i) == *(char*)pattern[0] || mask[0] == *"?" && i + patternSize < moduleSize) {	// If the current byte is equal to the beginning of the pattern, without going beyond the memory space of the module
            if (ComparePattern(moduleBase + i, pattern, mask)) {													// Check if the entire pattern matches
                matches.push_back(moduleBase + i);																	// If it does, add it to the vector
            }
        }
    }

    if (matches.size() == 0) {																						// If no matches
        return NULL;																								// Return NULL
    }
    return matches[0];																								// Return the first match (change this and the function return type if you want to return the entire vector
}
#endif //GO_INTERNALAIM_GHAOB_H
