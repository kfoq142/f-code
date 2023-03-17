#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

int main(int argument, char const *argv[]){
    printf("\n--- TCP Server ---\n");

    WSADATA data;
    int WSAStart = WSAStartup(MAKEWORD(2, 2), &data);
    if(WSAStart != 0){
        printf("WSAStartup failed: %d\n", WSAStart);
        return 1;
    }

    SOCKET TCPserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPserver == INVALID_SOCKET){
        printf("Socket() failed: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    struct sockaddr_in TCPserver_add;
    TCPserver_add.sin_family = AF_INET;
    TCPserver_add.sin_addr.s_addr = inet_addr("192.168.0.115");
    TCPserver_add.sin_port = htons(5000);

    int sbind = bind(TCPserver,(SOCKADDR *)&TCPserver_add, sizeof(TCPserver_add));
    if(sbind == SOCKET_ERROR){
        printf("Bind() failed: %d\n", WSAGetLastError());
        closesocket(TCPserver);
        WSACleanup();
        return 1;
    }
    int slisten = listen(TCPserver, 5);
    if(slisten == SOCKET_ERROR){
        printf("Listen() failed: %d\n", WSAGetLastError());
        closesocket(TCPserver);
        WSACleanup();
        return 1;
    }

    printf("Connecting ...\n");

    struct sockaddr_in TCPclient;
    int iTCPclient = sizeof(TCPclient); 
    SOCKET sAccept = accept(TCPserver, (SOCKADDR *)&TCPclient, &iTCPclient);
    if(sAccept == SOCKET_ERROR){
        printf("Accept() failed: %d\n", WSAGetLastError());
        closesocket(TCPserver);
        WSACleanup();
        return 1;
    }

    printf("Server accecpted...\n");

    while(1){
        char recvbuffer[20];
        int irecvbuffer = sizeof(recvbuffer);
        int irecv= recv(sAccept, recvbuffer, irecvbuffer, 0);
        if(irecv == SOCKET_ERROR){
            printf("Recv() failed: %d\n", WSAGetLastError());
            closesocket(TCPserver);
            WSACleanup();
            return 1;
        }
        printf("%s", recvbuffer);
        // printf(" ");
        // time_t timer;
        // time(&timer);
        // printf("%s", ctime(&timer));
        memset(recvbuffer, 0, sizeof(recvbuffer));  
    }
    return 0;
}