#include <Windows.h>
#include <string.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    char lpFileName[] = "License.dat";
    int FlagsAndAttributes;
    HANDLE hFile = CreateFileA(&lpFileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE) {
        goto failed;
    }
    DWORD nByte;
    char lpBuffer[100];
    if (ReadFile(hFile, lpBuffer, 39, &nByte, 0) != 0) {
        if (nByte < 30)
            goto failed;
        if (strncmp("pearldarkk", lpBuffer + 6, 10))
            goto failed;
        if (strncmp("8Aha^89xn", lpBuffer + 24, 9))
            goto failed;
        MessageBoxA(0, "Gud gud you did it", "Succeed", MB_OK);
        ExitProcess(0);
    }

failed:
    MessageBoxA(0, "Oops failed. Lesor :}", 0, 0x30l);
    ExitProcess(0);
}