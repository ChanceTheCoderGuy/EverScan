#include "stdafx.h"

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")
#include <detours.h>
#pragma comment(lib, "detours.lib")

typedef HRESULT (WINAPI* oEndScene) (LPDIRECT3DDEVICE9 D3DDevice);

typedef HRESULT (WINAPI* oRest) (LPDIRECT3DDEVICE9 D3DDevice,  D3DPRESENT_PARAMETERS *pPresentationParameters);

bool D3DHook();

HRESULT WINAPI newEndScene(LPDIRECT3DDEVICE9 D3DDevice);
HRESULT WINAPI newRest(LPDIRECT3DDEVICE9 D3DDevice, D3DPRESENT_PARAMETERS *pPresentationParameters);

void CreateFont(LPDIRECT3DDEVICE9 D3DDevice);
void DrawText(LPCSTR TextToDraw, int x, int y, D3DCOLOR color);



