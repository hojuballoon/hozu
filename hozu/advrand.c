//advapi32.dll�̈Í��p���������֐��ŋ��͂ȗ����𐶐�
//����Ɏg���ɂ͂�����ƃI�[�o�[�X�y�b�N�H

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
