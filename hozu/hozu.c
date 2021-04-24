//�ےÐ������C���\�[�X�R�[�h

#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include "memapi.h"
#include "advrand.h"
#include "difr.h"
#include "hozu.h"

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprevinst, LPSTR CmdLine, int CmdShow);
static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);
static void WmCreate(HWND hwnd, HDC* lphmdc, HBITMAP* lphmwbmp, HFONT* lphfont);
static void IdmIkku(HWND hwnd, HDC hmdc, int* lpvnum);
static void WmClose(HWND hwnd, int vnum, HDC hmdc, HBITMAP hmwbmp, HFONT hfont);
static void WmPaint(HWND hwnd, HDC hmdc);
static void WmTimer(HWND hwnd, HDC hmdc, int* lpvnum);

int WINAPI WinMain(HINSTANCE hinst, HINSTANCE hprevinst, LPSTR CmdLine, int CmdShow)
{
	HWND hwnd;
	MSG msg;
	WNDCLASSEX wcx;

	ZeroMemory(&wcx, sizeof(WNDCLASSEX));
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.lpfnWndProc = (WNDPROC)WndProc;
	wcx.hInstance = hinst;
	wcx.hIcon = LoadIcon(hinst, "HOZUICON");
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcx.lpszMenuName = "HOZUMENU";
	wcx.lpszClassName = "Hozu";
	if (!RegisterClassEx(&wcx))
		return 0;

	hwnd = CreateWindowEx(
		0,
		"Hozu",
		"�ےÐ���",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPSIBLINGS | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		NULL,
		(HMENU)NULL,
		hinst,
		0
	);

	ShowWindow(hwnd, CmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	static int vnum;
	static HDC hmdc;	//���ۘM��̂͂�����
	static HBITMAP hmwbmp;
	static HFONT hfont;

	switch (message)
	{
	case WM_CREATE:
		WmCreate(hwnd, &hmdc, &hmwbmp, &hfont);
		break;

	case WM_CLOSE:
		WmClose(hwnd, vnum, hmdc, hmwbmp, hfont);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		WmPaint(hwnd, hmdc);
		break;

	case WM_COMMAND:
		switch (LOWORD(wparam))
		{
		case IDM_IKKU:
			IdmIkku(hwnd, hmdc, &vnum);
			break;

		case IDM_QUIT:
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;

		case IDM_README:
			ShellExecute(hwnd, "open", "readme.txt", "", "", SW_NORMAL);
			break;

		case IDM_HP:
			ShellExecute(hwnd, "open", "https://hoju.jp", "", "", SW_NORMAL);
			break;

		case IDM_VERSION:
			ShellAbout(hwnd, "�ےÐ��� �̃o�[�W�������#�ےÐ��� Version 2.3", "Copyright (C) 2002-2021 �L�[���D���X", LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), "HOZUICON"));
			break;
		}

		break;

	case WM_TIMER:
		WmTimer(hwnd, hmdc, &vnum);
		break;

	case MM_MCINOTIFY:
		mciSendString("seek hozubgm to start", NULL, 0, hwnd);
		mciSendString("play hozubgm notify", NULL, 0, hwnd);
		break;
	}

	return DefWindowProc(hwnd, message, wparam, lparam);
}

static void WmCreate(HWND hwnd, HDC* lphmdc, HBITMAP* lphmwbmp, HFONT* lphfont)
{
	HDC hsdc;
	RECT rect, wrect;
	HWND hbutton;

	AdvapiLoad();

	//�E�C���h�E�T�C�Y�̒���
	SetRect(&rect, 0, 0, 320, 240);
	AdjustWindowRectEx(&rect, GetWindowLong(hwnd, GWL_STYLE), FALSE, 0);
	GetWindowRect(hwnd, &wrect);
	MoveWindow(hwnd, wrect.left, wrect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

	hsdc = GetDC(hwnd);
	*lphmdc = CreateCompatibleDC(hsdc);
	*lphmwbmp = CreateCompatibleBitmap(hsdc, GetDeviceCaps(*lphmdc, HORZRES), GetDeviceCaps(*lphmdc, VERTRES));
	ReleaseDC(hwnd, hsdc);
	SelectObject(*lphmdc, *lphmwbmp);
	PatBlt(*lphmdc, 0, 0, GetDeviceCaps(*lphmdc, HORZRES), GetDeviceCaps(*lphmdc, VERTRES), WHITENESS);
	SetBkMode(*lphmdc, TRANSPARENT);

	*lphfont = CreateFont(16, 16, 0, 0, FW_DONTCARE, 0, 0, 0, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 0, "FixedSys");
	SelectObject(*lphmdc, *lphfont);

	if (!DrawImageFromResource(*lphmdc, 0, 0, NULL, "ATAGO", "GIF"))
	{
		MessageBox(hwnd, "�摜�̕\���Ɏ��s���܂���", NULL, MB_ICONEXCLAMATION);
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		return;
	}

	hbutton = CreateWindowEx(0, "Button", "���", WS_CHILD | WS_VISIBLE, 0, 0, 64, 24, hwnd, (HMENU)IDM_IKKU, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 0);
	SendMessage(hbutton, 0x0030, (unsigned int)GetStockObject(17), 0);
	hbutton = CreateWindowEx(0, "Button", "�I��", WS_CHILD | WS_VISIBLE, 0, 24, 64, 24, hwnd, (HMENU)IDM_QUIT, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 0);
	SendMessage(hbutton, 0x0030, (unsigned int)GetStockObject(17), 0);

	mciSendString("open bgm.mid alias hozubgm", NULL, 0, hwnd);
	mciSendString("play hozubgm notify", NULL, 0, hwnd);
}

static void IdmIkku(HWND hwnd, HDC hmdc, int* lpvnum)
{
	RECT rect;
	HBRUSH hbrush = CreateSolidBrush(RGB(107, 181, 255));

	//if(*lpvnum)
	//	KillTimer(hwnd, 1);

	SetRect(&rect, 100, 0, 250, 54);
	FillRect(hmdc, &rect, hbrush);
	DeleteObject(hbrush);
	SetTimer(hwnd, 1, 500, NULL);
	*lpvnum = 1;
	SendMessage(hwnd, WM_TIMER, 0, 0);
}

static void WmClose(HWND hwnd, int vnum, HDC hmdc, HBITMAP hmwbmp, HFONT hfont)
{
	if (vnum)
		KillTimer(hwnd, 1);

	mciSendString("close hozubgm", NULL, 0, hwnd);
	DeleteObject(hmdc);
	DeleteObject(hmwbmp);
	DeleteObject(hfont);
	AdvapiFree();
	DestroyWindow(hwnd);
}

static void WmPaint(HWND hwnd, HDC hmdc)
{
	PAINTSTRUCT ps;
	HDC hsdc = BeginPaint(hwnd, &ps);

	BitBlt(hsdc, 0, 0, GetDeviceCaps(hmdc, HORZRES), GetDeviceCaps(hmdc, VERTRES), hmdc, 0, 0, SRCCOPY);
	EndPaint(hwnd, &ps);
}

static void WmTimer(HWND hwnd, HDC hmdc, int* lpvnum)
{
	//2.1�̃l�^
	//static LPCSTR kaminoku[] = {"���q�R", "�����R", "���R", "�ےÐ��", "�Ȃ񂾂���", "����������", "�쉺��", "�Ȃ��˂�", "�Y�킾��", "���܂�����", "������", "������", "���Ƃڂ���", "���ꂩ���", "���̒r��", "�n���m�L��", "���낱�_", "�W�W�o�o��", "�΂��΂���", "�ԕ���", "�D�̏�", "�}����", "�ےË���", "�ےÏ���", "�n����", "�剁��", "�_������", "�䕗��", "���A�[��", "���A�܂��ŁH"};
	//static LPCSTR nakanoku[] = {"�������邼", "���D�̏��", "�g���b�R���", "�R�o�肾��", "�J���X���悤��", "�������񂾂炯", "�X�X�L�����ς�", "�G�ߊO�ꂾ", "�����A�t��", "�E�O�C�X�\��", "���̎q���D����", "���ł�������", "�������V�C��", "�A���̑�����", "�I�b�X���b�X�L�b�X", "�����W�����Ȉ�", "���������ɂ�", "�����낢��Ȃ���", "�H��o����", "�������Ă�", "�݂�Ȃł������", "��������������", "�����������^", "���R�ꂵ�Ă�", "���ނ��Ă�", "�S�C����", "���ւ��Ă�", "���㗒�R", "����`�����X", "�Ԃ��Ԃ�", "��������", "���������"};
	//static LPCSTR simonoku[] = {"����Ă�", "�Ă�����", "����͂���", "��������", "����ꂽ", "�����Ă悵", "�����Ă���", "�H���������", "�z�g�P�m�U", "�����ς���", "������", "������", "���ł���", "����ł���", "�܂�����", "�т��o�Ă�", "�d�b�ł��", "�I�I������", "�����b������", "����ɂ��", "�]����", "�{��ꂽ", "�N�I���e�B", "���d����", "�Ԃ�����", "���肦�Ȃ�", "�h��Ă���"};

	static LPCSTR kaminoku[] = { "���q�R", "�����R", "���R", "�ےÐ��", "�Ȃ񂾂���", "����������", "�쉺��", "�Ȃ��˂�", "�Y�킾��", "���܂�����", "������", "������", "���Ƃڂ���", "���ꂩ���", "���̒r��", "�n���m�L��", "���낱�_", "�W�W�o�o��", "�΂��΂���", "�ԕ���", "�D�̏�", "�}����", "�ےË���", "�ےÏ���", "�n����", "�剁��", "�_������", "���A�[��", "���A�܂��ŁH", "���D��" };
	static LPCSTR nakanoku[] = { "�������邼", "���D�̏��", "�g���b�R���", "�R�o�肾��", "�J���X���悤��", "�������񂾂炯", "�X�X�L�����ς�", "�G�ߊO�ꂾ", "�����A�t��", "�E�O�C�X�\��", "���̎q���D����", "���ł�������", "�������V�C��", "�A���̑�����", "�I�b�X���b�X�L�b�X", "�����W�����Ȉ�", "���������ɂ�", "�����낢��Ȃ���", "�H��o����", "�݂�Ȃł������", "��������������", "�����������^", "���R�ꂵ�Ă�", "���ނ��Ă�", "���ւ��Ă�", "���㗒�R", "����`�����X", "�Ԃ��Ԃ�", "��������", "���������", "���D�t�F�`��", "�c��܂��Ă�", "��r����" };
	static LPCSTR simonoku[] = { "����Ă�", "�Ă�����", "����͂���", "��������", "����ꂽ", "�����Ă悵", "�����Ă���", "�H���������", "�z�g�P�m�U", "�����ς���", "������", "������", "���ł���", "����ł���", "�܂�����", "�т��o�Ă�", "�d�b�ł��", "�I�I������", "�����b������", "����ɂ��", "�]����", "�{��ꂽ", "�N�I���e�B", "���d����", "�Ԃ�����", "���肦�Ȃ�", "�h��Ă���", "���D��", "���낵��", "�C��������", "�������^" };
	LPCSTR cp;
	UINT urand;

	AdvapiRtlGenRandom(&urand, sizeof(urand));
	switch (*lpvnum)
	{
	case 1:
		cp = kaminoku[urand % (sizeof(kaminoku) / sizeof(LPCSTR))];
		TextOut(hmdc, 100, 0, cp, lstrlen(cp));
		*lpvnum = 2;
		break;

	case 2:
		cp = nakanoku[urand % (sizeof(nakanoku) / sizeof(LPCSTR))];
		TextOut(hmdc, 100, 16, cp, lstrlen(cp));
		*lpvnum = 3;
		break;

	case 3:
		cp = simonoku[urand % (sizeof(simonoku) / sizeof(LPCSTR))];
		TextOut(hmdc, 100, 32, cp, lstrlen(cp));
		KillTimer(hwnd, 1);
		*lpvnum = 0;
		break;
	}

	InvalidateRect(hwnd, NULL, 0);
}
