//リソースから任意の形式の画像を読み込んで描画
//OLEを使用するのでGIF形式も読み込むことができる

#include <windows.h>
#include <ole2.h>
#include <olectl.h>
#include "memapi.h"
#include "difr.h"

#define COEF_HM		2540	// INCH-HIMETRIC換算係数

BOOL DrawImageFromResource(HDC hdc, int x, int y, HMODULE hModule, LPCTSTR lpName, LPCTSTR lpType)
{
	HRSRC hrsrc = FindResource(hModule, lpName, lpType);
	HANDLE hres = (hrsrc != NULL) ? LoadResource(hModule, hrsrc) : NULL;
	char* lpresbin;
	DWORD ressize;
	HANDLE hglobal;
	IStream* iStream;
	IPicture* iPicture;
	OLE_XSIZE_HIMETRIC hm_w;
	OLE_YSIZE_HIMETRIC hm_h;
	int pix_w, pix_h;
	HDC hdispdc;

	if (hres == NULL)
		return FALSE;

	lpresbin = (char*)LockResource(hres);
	ressize = SizeofResource(hModule, hrsrc);
	hglobal = GlobalAlloc(GPTR, ressize);
	if (hglobal == NULL)
		return FALSE;

	if (CreateStreamOnHGlobal(hglobal, TRUE, &iStream) != S_OK)
	{
		GlobalFree(hglobal);
		return FALSE;
	}

	MoveMemory(hglobal, lpresbin, ressize);
	if (OleLoadPicture(iStream, ressize, FALSE, &IID_IPicture, (LPVOID*)&iPicture) != S_OK)
	{
		iStream->lpVtbl->Release(iStream);
		return FALSE;
	}

	iPicture->lpVtbl->get_Width(iPicture, &hm_w);
	iPicture->lpVtbl->get_Height(iPicture, &hm_h);
	hdispdc = GetDC((HWND)NULL);
	pix_w = MulDiv(GetDeviceCaps(hdispdc, LOGPIXELSX), hm_w, COEF_HM);
	pix_h = MulDiv(GetDeviceCaps(hdispdc, LOGPIXELSX), hm_h, COEF_HM);
	ReleaseDC((HWND)NULL, hdispdc);
	iPicture->lpVtbl->Render(iPicture, hdc, x, y, pix_w, pix_h, 0, hm_h, hm_w, -hm_h, (LPCRECT)NULL);
	iStream->lpVtbl->Release(iStream);
	iPicture->lpVtbl->Release(iPicture);
	return TRUE;
}
