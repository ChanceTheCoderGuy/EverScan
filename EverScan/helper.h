#include "stdafx.h"

//---------------------------------------------------------------------------------------

void writeIt(DWORD address, byte *content, DWORD length);

void writeByte(DWORD address, BYTE content);
void writeDWORD(DWORD address, DWORD content);
void writeFloat(DWORD address, float content);

DWORD readIt(DWORD address, DWORD *content);
Byte readByte(DWORD address, BYTE *content);
float readFloat(DWORD address, float *content);

//---------------------------------------------------------------------------------------

bool hook(DWORD HookAddress, DWORD CaveAddress);

//---------------------------------------------------------------------------------------

char* Dec2Hex(int decVal);

char* intToChar(char *Pointer, int thenumber);