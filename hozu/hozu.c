//保津川川柳メインソースコード

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
		"保津川川柳",
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
	static HDC hmdc;	//実際弄るのはこっち
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
			ShellAbout(hwnd, "保津川川柳 のバージョン情報#保津川川柳 Version 2.3", "Copyright (C) 2002-2021 豊充風船商店", LoadIcon((HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), "HOZUICON"));
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

	//ウインドウサイズの調整
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
		MessageBox(hwnd, "画像の表示に失敗しました", NULL, MB_ICONEXCLAMATION);
		SendMessage(hwnd, WM_CLOSE, 0, 0);
		return;
	}

	hbutton = CreateWindowEx(0, "Button", "一句", WS_CHILD | WS_VISIBLE, 0, 0, 64, 24, hwnd, (HMENU)IDM_IKKU, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 0);
	SendMessage(hbutton, 0x0030, (unsigned int)GetStockObject(17), 0);
	hbutton = CreateWindowEx(0, "Button", "終了", WS_CHILD | WS_VISIBLE, 0, 24, 64, 24, hwnd, (HMENU)IDM_QUIT, (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE), 0);
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
	//2.1のネタ
	//static LPCSTR kaminoku[] = {"小倉山", "愛宕山", "嵐山", "保津川で", "なんだこれ", "おおすげえ", "川下り", "なんやねん", "綺麗だな", "うまそうだ", "感動だ", "可愛いな", "おとぼけに", "これからの", "あの池の", "ハンノキの", "うろこ雲", "ジジババが", "ばかばかが", "花粉症", "船の上", "急流で", "保津橋で", "保津小橋", "渡月橋", "大堰川", "ダム放流", "台風で", "リア充が", "え、まじで？"};
	//static LPCSTR nakanoku[] = {"何かあるぞ", "お船の上で", "トロッコ列車", "山登りだぞ", "カラスうようよ", "おっさんだらけ", "ススキいっぱい", "季節外れだ", "頭がアフロ", "ウグイス饅頭", "あの子が好きだ", "いつでも此処に", "あした天気に", "アリの巣穴に", "オッスメッスキッス", "ええジャ内科医", "いい加減にせ", "おもろいやないか", "幽霊出たぞ", "増水してる", "みんなでくしゃみ", "あいつかっこいい", "激しくワロタ", "水漏れしてる", "魚釣ってる", "鉄砲水で", "小便してる", "嵯峨嵐山", "告るチャンス", "赤い車が", "怪しい車", "爆発しろよ"};
	//static LPCSTR simonoku[] = {"流れてる", "焼き鳥だ", "わっはっは", "多すぎる", "嫌われた", "逝ってよし", "生えていた", "食っちゃった", "ホトケノザ", "いっぱいだ", "美しい", "可愛いな", "飛んできた", "うんでいる", "つまずいた", "毛が出てる", "電話でんわ", "オオいけね", "ワルッ買った", "かんにんや", "転覆だ", "怒られた", "クオリティ", "自重しろ", "ぶつかった", "ありえない", "揺れている"};

	static LPCSTR kaminoku[] = { "小倉山", "愛宕山", "嵐山", "保津川で", "なんだこれ", "おおすげえ", "川下り", "なんやねん", "綺麗だな", "うまそうだ", "感動だ", "可愛いな", "おとぼけに", "これからの", "あの池の", "ハンノキの", "うろこ雲", "ジジババが", "ばかばかが", "花粉症", "船の上", "急流で", "保津橋で", "保津小橋", "渡月橋", "大堰川", "ダム放流", "リア充が", "え、まじで？", "風船の" };
	static LPCSTR nakanoku[] = { "何かあるぞ", "お船の上で", "トロッコ列車", "山登りだぞ", "カラスうようよ", "おっさんだらけ", "ススキいっぱい", "季節外れだ", "頭がアフロ", "ウグイス饅頭", "あの子が好きだ", "いつでも此処に", "あした天気に", "アリの巣穴に", "オッスメッスキッス", "ええジャ内科医", "いい加減にせ", "おもろいやないか", "幽霊出たぞ", "みんなでくしゃみ", "あいつかっこいい", "激しくワロタ", "水漏れしてる", "魚釣ってる", "小便してる", "嵯峨嵐山", "告るチャンス", "赤い車が", "怪しい車", "爆発しろよ", "風船フェチだ", "膨らましてる", "空ビ発見" };
	static LPCSTR simonoku[] = { "流れてる", "焼き鳥だ", "わっはっは", "多すぎる", "嫌われた", "逝ってよし", "生えていた", "食っちゃった", "ホトケノザ", "いっぱいだ", "美しい", "可愛いな", "飛んできた", "うんでいる", "つまずいた", "毛が出てる", "電話でんわ", "オオいけね", "ワルッ買った", "かんにんや", "転覆だ", "怒られた", "クオリティ", "自重しろ", "ぶつかった", "ありえない", "揺れている", "風船だ", "恐ろしい", "気持ちいい", "糞ワロタ" };
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
