#include "stdafx.h"
#include "helper.h"

//---------------------------------------------------------------------------------------
void writeIt(DWORD address, byte *content, DWORD length){
	BYTE *byteP;

	for(int x = 0; x < length; x++, address++)
	{
		byteP=(BYTE*)address;
		*byteP = content[x]; 
	}
}

void writeByte(DWORD address, BYTE content){
	BYTE *byteP;
	byteP=(BYTE*)address;
	*byteP = content; 
}
void writeDWORD(DWORD address, DWORD content){
	DWORD *dwordP;
	dwordP=(DWORD*)address;
	*dwordP = content; 
}
void writeFloat(DWORD address, float content){
	float *floatP;
	floatP=(float*)address;
	*floatP = content;
}

//---------------------------------------------------------------------------------------

DWORD readInt(DWORD address, DWORD *content){
	DWORD *dwordP;
	dwordP=(DWORD*)address;
	*content = *dwordP;
	return *content;
}
Byte readByte(DWORD address, BYTE *content){
	BYTE *byteP;
	byteP=(BYTE*)address;
	*content = *byteP;
	return *content;
}
float readFloat(DWORD address, float *content){
	float *floatP;
	floatP=(float*)address;
	*content = *floatP;
	return *content;
}



//---------------------------------------------------------------------------------------
bool hook(DWORD HookAddress, DWORD CaveAddress){
	DWORD old;
	DWORD *dwordP;
	BYTE *byteP;

	DWORD H_inline=HookAddress;
	VirtualProtect((LPVOID)H_inline,sizeof(DWORD),0x40,&old);
	byteP=(BYTE*)H_inline;
	*byteP = 0xE9;  //Jump
	//------------------- 
	H_inline++;
	dwordP = (DWORD*)H_inline;
	*dwordP = (DWORD)CaveAddress-H_inline-1;

	return true;
}

//---------------------------------------------------------------------------------------

char* Dec2Hex(int decVal){
	char hx[256];
	sprintf(hx,"%x",decVal);
	return hx;
}

char* intToChar(char *Pointer, int thenumber){
	itoa(thenumber,Pointer,10);
	return Pointer;
}