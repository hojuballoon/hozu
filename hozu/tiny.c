//軽量化エントリーポイント
//出典：https://hp.vector.co.jp/authors/VA016117/tinyexe.html

#include <windows.h>

void __cdecl WinMainCRTStartup(void)

{
    int mainret;
    char* lpszCommandLine;
    STARTUPINFO StartupInfo;

    lpszCommandLine = GetCommandLine();

    if (*lpszCommandLine == '"')  // Check for and handle quoted program name
    {
        do {
            lpszCommandLine++;
        } while (*lpszCommandLine && (*lpszCommandLine != '"'));

        if (*lpszCommandLine == '"')
            lpszCommandLine++;
    }
    else    // First token wasn't a quote
    {
        while (*lpszCommandLine > ' ')
            lpszCommandLine++;
    }

    while (*lpszCommandLine && (*lpszCommandLine <= ' '))
        lpszCommandLine++;

    StartupInfo.dwFlags = 0;
    GetStartupInfo(&StartupInfo);

    mainret = WinMain(GetModuleHandle(NULL),
        NULL,
        lpszCommandLine,
        StartupInfo.dwFlags & STARTF_USESHOWWINDOW
        ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);

    ExitProcess(mainret);
}
