#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <tlhelp32.h>

int main() {
    HANDLE hProcessSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

    PROCESSENTRY32 ProcessEntry = {0};
    ProcessEntry.dwSize = sizeof(ProcessEntry);

    BOOL Return = FALSE;
Label:
    Return = Process32First(hProcessSnapShot, &ProcessEntry);

    if (!Return) {
        goto Label;
    }

    do {
        int value = _tcsicmp(ProcessEntry.szExeFile, _T("msedge.exe"));
        //replace the taskmgr.exe to the process u want to remove.
        if (value == 0) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, ProcessEntry.th32ProcessID);
            TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
        }

    } while (Process32Next(hProcessSnapShot, &ProcessEntry));

    CloseHandle(hProcessSnapShot);
}