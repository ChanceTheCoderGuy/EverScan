#include "stdafx.h"

using namespace std;

void PatternThread();
void updateAarray();
void AddToArray(DWORD Address, DWORD Size);

DWORD PatternScan(Byte Patter[], char Mask[], DWORD length);

extern "C" __declspec(dllexport)DWORD WardenCave(void *null);

extern "C" __declspec(dllexport)DWORD SetHooks(void *null);
