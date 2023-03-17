#include <Windows.h>
#include <Psapi.h>
#include <stdio.h>
#include <string.h>

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK nextWindow(HWND, LPARAM);

int WINAPI WinMain(HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASSEXA wc;
    wc.hInstance = hIns;
    wc.lpszClassName = "AntiBrowsers";
    wc.lpfnWndProc = WinProc;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wc.lpszMenuName = NULL;                
    wc.cbClsExtra = 0;                     
    wc.cbWndExtra = 0;                     
    wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    if (!RegisterClassExA(&wc))
        return 0;

    hwnd = CreateWindowExA(0, "AntiBrowsers", "Anti-browsers", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 330, 100, HWND_DESKTOP, NULL, hIns, NULL);

    ShowWindow(hwnd, nCmdShow);

    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateWindowExA(0, "STATIC", "No browsers allowed while I'm running!", WS_CHILD | WS_VISIBLE, 20, 20, 270, 20, hwnd, (HMENU) 1, NULL, NULL);
        EnumWindows(nextWindow, NULL);
        SetTimer(hwnd, 1, 5000, NULL);    // every 5 seconds
        break;
    
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        break;

    case WM_TIMER:
        EnumWindows(nextWindow, NULL);
        break;

    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

char buf[100] = { 0 };
int len;
DWORD dwProcId = 0;
HANDLE hProc;
int i;

BOOL CALLBACK nextWindow(HWND hwnd, LPARAM lparam) {
    if (hwnd != 0) {
        GetWindowThreadProcessId(hwnd, &dwProcId);
        hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcId);
        GetProcessImageFileNameA(hProc, &buf, 100);
        for (i = strlen(buf); i > 0; --i)
            if (buf[i] == '\\') 
                break;
        ++i;
        if (!_stricmp(buf + i, "firefox.exe", 12) || !_stricmp(buf + i, "chrome.exe", 11) || !_stricmp(buf + i, "msedge.exe", 11))
            SendMessageA(hwnd, WM_DESTROY, 0, 0); 
        CloseHandle(hProc);

    }
    return TRUE;
}
