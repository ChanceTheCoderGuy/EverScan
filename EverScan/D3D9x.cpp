#include "stdafx.h"
#include "D3D9x.h"
#include "helper.h"

using namespace std;

//---------------------------------------------------------------------------------------
IDirect3DDevice9 * ppReturnedDeviceInterface;	

unsigned long* pInterface;

typedef HRESULT (WINAPI* oEndScene) (LPDIRECT3DDEVICE9 D3DDevice);
oEndScene EndScene;

typedef HRESULT (WINAPI* oRest) (LPDIRECT3DDEVICE9 D3DDevice,  D3DPRESENT_PARAMETERS *pPresentationParameters);
oRest Rest;

LPD3DXFONT      g_pFont = NULL; //D3D Font
LPD3DXLINE      g_pLine = NULL; //D3D Line

D3DVIEWPORT9    g_ViewPort; //ViewPort

bool CreatedFont = false;

struct ScanInfo
{
	DWORD Address;
	DWORD Size;
};

extern ScanInfo Scaninfo[100];

//---------------------------------------------------------------------------------------
D3DCOLOR fontPink	= D3DCOLOR_ARGB(255, 210, 2, 208);
D3DCOLOR fontWhite  = D3DCOLOR_ARGB(255,255,255,255);
D3DCOLOR fontPurple	= D3DCOLOR_ARGB(255, 0, 127, 192);
//---------------------------------------------------------------------------------------

bool D3DHook()
{
	HMODULE hDLL;
	hDLL=GetModuleHandleA("d3d9");
	LPDIRECT3D9(__stdcall*pDirect3DCreate9)(UINT) = (LPDIRECT3D9(__stdcall*)(UINT))GetProcAddress( hDLL, "Direct3DCreate9");

	LPDIRECT3D9 pD3D = pDirect3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	HRESULT hRes = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm );
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;

	HWND hWnd=FindWindowA(NULL, "World of Warcraft");

	hRes = pD3D->CreateDevice( 
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&d3dpp, &ppReturnedDeviceInterface);

	pD3D->Release();

	pInterface = (unsigned long*)*((unsigned long*)ppReturnedDeviceInterface);

	EndScene = (oEndScene) (DWORD)pInterface[42];
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)EndScene, newEndScene);
	DetourTransactionCommit();

	Rest = (oRest) (DWORD)pInterface[16];
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(PVOID&)Rest, newRest);
	DetourTransactionCommit();

	return true;
}


//---------------------------------------------------------------------------------------
HRESULT WINAPI newRest(LPDIRECT3DDEVICE9 D3DDevice, D3DPRESENT_PARAMETERS *pPresentationParameters)
{	
	if(g_pFont) g_pFont->OnLostDevice();

	int iReturnValue = Rest(D3DDevice, pPresentationParameters);

	if(g_pFont) g_pFont->OnResetDevice();

	return iReturnValue;//Rest(D3DDevice, pPresentationParameters);
}

//---------------------------------------------------------------------------------------

bool ShowScan = true;
DWORD HideCounter = 0;

HRESULT WINAPI newEndScene(LPDIRECT3DDEVICE9 D3DDevice)
{

	if(D3DDevice == nullptr){
		return EndScene(D3DDevice);
	}

	D3DDevice->GetViewport(&g_ViewPort);

	//---------------------------------------------------------------------------------------

	if(!CreatedFont) {
		CreateFont(D3DDevice);
	}

	if((GetAsyncKeyState(VK_F5)) && (HideCounter == 0)){ 
		ShowScan = !ShowScan;
		HideCounter = 10;
	}else if (HideCounter > 0){
		HideCounter--;
	}

	if(ShowScan)
	{
		DrawText("EverScan        F5 - Hide, F6 - Dump Scan", 25, 80, fontPurple);

		int counterY = 100;
		int counterX =   25;	//Test "Address"
		int counterX2 = 110;	//Value

		int counterXa =  190;	//Test "Length"
		int counterXa2 = 265;	//Value

		char *Text = new char[100];

		for(int x = 0; x < 99; x++)
		{
			if(Scaninfo[x].Address == 0)
				break;

			if((counterY+340) > g_ViewPort.Height)
			{
				counterX = counterX + 300;
				counterX2 = counterX2 + 300;

				counterXa = counterXa + 300;
				counterXa2 = counterXa2 + 300;
				counterY = 100;
			}

			DrawText("Address : ", counterX, counterY, fontPink);
			DrawText(Dec2Hex(Scaninfo[x].Address), counterX2, counterY, fontWhite);

			DrawText("Length : ", counterXa, counterY, fontPink);
			DrawText(Dec2Hex(Scaninfo[x].Size), counterXa2, counterY, fontWhite);

			counterY = counterY + 20;
		}
	}

	return EndScene(D3DDevice);
}

//---------------------------------------------------------------------------------------

void  CreateFont(LPDIRECT3DDEVICE9 D3DDevice) 
{

	CreatedFont = true;

	if(g_pFont == NULL) D3DXCreateFont(D3DDevice, 20, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont); //Create fonts Arial

	if(g_pLine == NULL) D3DXCreateLine(D3DDevice, &g_pLine); 

	D3DDevice->GetViewport(&g_ViewPort);
}

//---------------------------------------------------------------------------------------

void DrawText(LPCSTR TextToDraw, int x, int y, D3DCOLOR color)
{
	RECT rct = {x,  y, x + 120, y + 15}; 
	g_pFont->DrawText(NULL, TextToDraw, -1, &rct, DT_NOCLIP, color);

}