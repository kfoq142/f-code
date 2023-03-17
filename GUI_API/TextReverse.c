#include <windows.h>
#include <string.h>

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[] = "DisplayText";

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEXA wincl;        /* Data structure for the windowclass */
    //WNDCLASSEX STRUCT
    //    cbSize              DWORD ? 0
    //    style               DWORD ? 4
    //    lpfnWndProc         QWORD ? 8
    //    cbClsExtra          DWORD ? 10
    //    cbWndExtra          DWORD ? 14
    //    hInstance           QWORD ? 18
    //    hIcon               QWORD ? 20
    //    hCursor             QWORD ? 28
    //    hbrBackground       QWORD ? 30
    //    lpszMenuName        QWORD ? 38
    //    lpszClassName       QWORD ? 40
    //    hIconSm             QWORD ? 48
    //    WNDCLASSEX ENDS
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW;                 /* Catch double-clicks */
    wincl.cbSize = sizeof(WNDCLASSEXA);
    wincl.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIconA(NULL, IDI_APPLICATION);
    //wincl.hIcon = 0;
    //wincl.hIconSm = 0;
    wincl.hCursor = LoadCursorA(NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH)COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassExA(&wincl))
        return 0;
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowExA(
        0,                   /* Extended possibilites for variation */
        szClassName,         /* Classname */
        "TextDisplay",       /* Title Text */
        WS_OVERLAPPEDWINDOW, /* default window */
        CW_USEDEFAULT,       /* Windows decides the position */
        CW_USEDEFAULT,       /* where the window ends up on the screen */
        500,                 /* The programs width */
        130,                 /* and height in pixels */
        HWND_DESKTOP,        /* The window is a child-window to desktop */
        NULL,                /* No menu */
        hThisInstance,       /* Program Instance handler */
        NULL                 /* No Window Creation data */
    );

    /* Make the window visible on the screen */
    ShowWindow(hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage(&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    char text[255];
    char *rev;
    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        CreateWindowA("STATIC",
            "Input string:",
            WS_CHILD | WS_VISIBLE,
            10, 10, 150, 30,
            hwnd, (HMENU)11, NULL, NULL);

        CreateWindowA("EDIT", 
            0,
            WS_BORDER | WS_CHILD | WS_VISIBLE,
            200, 10, 250, 30,
            hwnd, (HMENU)1, NULL, NULL);

        CreateWindowA("STATIC",
            "Reversed string:",
            WS_CHILD | WS_VISIBLE,
            10, 50, 150, 30,
            hwnd, (HMENU)12, NULL, NULL);

        CreateWindowA("EDIT",
            0,
            WS_VISIBLE | WS_CHILD | WS_BORDER | WS_DISABLED,
            200, 50, 250, 30,
            hwnd, (HMENU)2, NULL, NULL);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1:
            GetDlgItemTextA(hwnd, 1, &text, 255);
            rev = _strrev(text);
            SetDlgItemTextA(hwnd, 2, rev);
            break;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProcA(hwnd, message, wParam, lParam);
    }

    return 0;
}