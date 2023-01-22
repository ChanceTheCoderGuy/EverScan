// D3D9.cpp : Defines the exported functions for the DLL application.
//
//	By   : DarkLinux
//	Site : Ever-Devs.com
//	For  : ownedCore.com
//

#include "stdafx.h"
#include "EverScan.h"
#include "helper.h"
#include "D3D9x.h"

using namespace std;

//---------------------------------------------------------------------------------------

struct ScanInfo
{
	DWORD Address;
	DWORD Size;
};

//---------------------------------------------------------------------------------------
#define LISTSIZE 100
ScanInfo Scaninfo[LISTSIZE];

ofstream dump;

DWORD GetBack = 0x0;

//---------------------------------------------------------------------------------------
extern "C" __declspec(dllexport)DWORD SetHooks(void *null)
{
	D3DHook();

	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)PatternThread,NULL,NULL,NULL);

	string filename = "EverScan.txt"; 
	dump.open(filename);
	dump << "EverScan \n";
	dump << "By : Darklinux @ Ever-Devs.com / OwnedCore.com \n";
	dump << "\n";
	return true;
}

//---------------------------------------------------------------------------------------

void PatternThread()
{
	BYTE ScanPattern[]={0x74, 0x02, 0xF3, 0xA5, 0xB1, 0x03, 0x23, 0xCA};
	char ScanPatternMask[]={'x' , 'x' , 'x' , 'x' , 'x' , 'x' , 'x' , 'x'};
	DWORD PatternAdress =  PatternScan(ScanPattern,ScanPatternMask,sizeof(ScanPattern));	

	GetBack = PatternAdress + 0xE;
	hook(PatternAdress, (DWORD)WardenCave+2);

	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)updateAarray,NULL,NULL,NULL);
}

//---------------------------------------------------------------------------------------

DWORD PatternScan(Byte *Signature, char *Mask, DWORD length)
{
	DWORD currentAddr = (DWORD)GetModuleHandle(NULL);
	DWORD Max;
	int i = 0;
	DWORD old;
	MEMORY_BASIC_INFORMATION mbi;

	do{
		VirtualQuery((LPVOID)currentAddr,&mbi,sizeof(mbi));

		if((mbi.RegionSize <= 0x9000) && (mbi.State == 4096) && (mbi.Type == 131072))
		{
			VirtualProtect((LPVOID)currentAddr,mbi.RegionSize,0x40,&old);

			if(currentAddr < Max) {
				return false;
			}else
				Max = currentAddr;

			for(int x=currentAddr; x < (currentAddr+mbi.RegionSize); x++)
			{
				if((*(BYTE*)x == Signature[i]) || Mask[i] == '?')
					i++;
				else 
					i = 0;

				if(i >= length){
					return (x - length + 1);
				}
			}
		}
		currentAddr+=mbi.RegionSize;

	}while(true);
}

//---------------------------------------------------------------------------------------

DWORD ScanAddress;
DWORD ScanLength;

extern "C" __declspec(dllexport)DWORD WardenCave(void *null)
{
	__asm
	{
		mov ScanAddress,esi		//Address
		mov ScanLength,edx		//Length

		je Je1					//Warden Code
		repe movsd
		Je1:
		mov cl,03
		and ecx,edx
		je Je2
		repe movsb 
		Je2:
		pop edi
		pop esi

		jmp dword ptr GetBack; //Jump back...
	};

}

//---------------------------------------------------------------------------------------

void updateAarray()
{
	DWORD lastAddress = 0;
	while(true){

		if((ScanAddress != lastAddress) && (ScanAddress < ((DWORD)GetModuleHandle(NULL)+0xEECFFF)  && (ScanLength != 0))){
			lastAddress = ScanAddress;

			for(int x = 0; x < 99; x++)
			{
				if(Scaninfo[x].Address == (ScanAddress-(DWORD)GetModuleHandle(NULL)))
					break;	

				if(Scaninfo[x].Address == 0){
					Scaninfo[x].Address = (ScanAddress-(DWORD)GetModuleHandle(NULL));
					Scaninfo[x].Size = ScanLength;
					dump << "Address : 0x" << (Dec2Hex(Scaninfo[x].Address)) << "   Size : 0x" <<  (Dec2Hex(Scaninfo[x].Size)) << "\n";
					break;
				}
			}
		}

		if(GetAsyncKeyState(VK_F6))
			dump.close();

		Sleep(1);
	};
}

