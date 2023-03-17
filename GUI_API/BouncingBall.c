#include <windows.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
    
//static variables used to keep track of 
//the ball’s position
int dX = 5, dY = 5; //stores direction
int x = 0, y = 0, oldX = 0, oldY = 0;
RECT rect;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPSTR lpszCmdLine, int nCmdShow) {
    HWND hwnd;
    MSG msg;
    WNDCLASSEXA wc;
    //fill the WNDCLASSEX object with the  appropriate values
    wc.cbSize = sizeof(WNDCLASSEXA);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    //wc.hbrBackground = (HBRUSH) COLOR_BACKGROUND;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "BouncingBall";
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    //register the new class
    if (!RegisterClassExA(&wc))
        return 0;
    //create a window
    hwnd = CreateWindowExA(
        0,
        "BouncingBall",
        "Bouncing Ball",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        600,
        600,
        HWND_DESKTOP,
        NULL,
        hInst,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);
    //event loop - handle all messages
    while (GetMessageA(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    //standard return value
    return (msg.wParam);
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam) {
    // decide direction
    SYSTEMTIME lpTime;
    BYTE direction;
    //device context and brush used for drawing
    HDC hdc;
    HBRUSH brush;
    //find out which message is being sent
    switch (nMsg)
    {
    case WM_CREATE:
        GetLocalTime(&lpTime);
        direction = lpTime.wSecond & 3;
        switch (direction) {
        case 0:
            dX = 5;
            dY = 5;
            break;
        case 1:
            dX = -5;
            dY = 5;
            break;
        case 2:
            dX = 5;
            dY = -5;
            break;
        default:
            dX = -5;
            dY = -5;
        }
        // set location
        GetClientRect(hwnd, &rect);
        x = (rect.right >> 1) - 10;
        y = (rect.bottom >> 1) - 10;
        //create the timer (0.02 seconds)
        SetTimer(hwnd, 1, 25, NULL);
        break;
    case WM_TIMER: //when the timer goes off (only one)
    //get the dc for drawing
        hdc = GetDC(hwnd);
        //use pure white
        brush = (HBRUSH)SelectObject(hdc,
            GetStockObject(WHITE_BRUSH));
        //fill a RECT object with the 
        //appropriate values
        RECT temp;
        temp.left = oldX;
        temp.top = oldY;
        temp.right = oldX + 20;
        temp.bottom = oldY + 20;
        //cover the old ellipse
        FillRect(hdc, &temp, brush);
        //get ready to draw the new ellipse
        brush = (HBRUSH)SelectObject(hdc,
            GetStockObject(GRAY_BRUSH));
        //draw it
        Ellipse(hdc, x, y, 20 + x, 20 + y);
        //update the values
        oldX = x;
        oldY = y;
        //prep the new coordinates for next time
        //if the circle is going off the edge then
        //reverse its direction
        if (x + dX + 20 > rect.right || x + dX < 0)
        {
            dX = -dX;
        }
        if (y + dY + 20 > rect.bottom || y + dY < 0)
        {
            dY = -dY;
        }
        x += dX;
        y += dY;
        //put the old brush back
        SelectObject(hdc, brush);
        //release the dc
        ReleaseDC(hwnd, hdc);
        break;
    case WM_DESTROY:
        //destroy the timer
        KillTimer(hwnd, 1);
        //end the program
        PostQuitMessage(0);
        break;
    case WM_SIZE:
        //get the window size and store it in rect
        GetClientRect(hwnd, &rect);
        if (x < 0) x = 0;
        if (x + 20 > rect.right) x = rect.right - 20;
        if (y < 0) y = 0;
        if (y + 20 > rect.bottom) y = rect.bottom - 20;
        break;
    default:
        //let Windows handle every other message
        return(DefWindowProcA(hwnd,
            nMsg, wParam, lParam));
    }
    return 0;
}