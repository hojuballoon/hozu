//advapi32.dllの暗号用乱数生成関数で強力な乱数を生成
//川柳に使うにはちょっとオーバースペック？

#include <Windows.h>
#include "advrand.h"

BOOL AdvapiLoad()
{
	HMODULE hadvapi = LoadLibrary("advapi32.dll");

	if (hadvapi == NULL)
		return FALSE;

	return TRUE;
}

BOOL AdvapiFree()
{
	HMODULE hadvapi = GetModuleHandle("advapi32.dll");

	if (hadvapi == NULL)
		return FALSE;

	return FreeLibrary(hadvapi);
}

BOOLEAN WINAPI AdvapiRtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength)
{
	HMODULE hadvapi = GetModuleHandle("advapi32.dll");
	LPRTLGR lprtlgr = (LPRTLGR)((hadvapi != NULL) ? GetProcAddress(hadvapi, "SystemFunction036") : NULL);

	if (lprtlgr == NULL)
		return 0;

	return lprtlgr(RandomBuffer, RandomBufferLength);
}
