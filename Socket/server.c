// #include <winsock2.h>
// #include <bits/stdc++.h>
// using namespace std;

// int main(int argument, char const *argv[]){
//     cout << endl << "--- TCP Server ---" << endl;
//     WSADATA d;
//     if(WSAStartup(MAKEWORD(2, 2), &d)!=0)
//         cout << "Startup failed." << endl;

//     SOCKET TCP_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//     if(TCP_server == INVALID_SOCKET)
//         cout << "Fail creat socket." << endl;
    
//     struct sockaddr_in TCPserver_add;
//     TCPserver_add.sin_family = AF_INET;
//     TCPserver_add.sin_addr.s_addr = inet_addr("192.168.0.115");
//     TCPserver_add.sin_port = htons(5000);

//     if(bind(TCP_server, (SOCKADDR *)&TCPserver_add, sizeof(TCPserver_add)) == SOCKET_ERROR)
//         cout << "Bind Failed && error -> " << WSAGetLastError() << endl;

//     if(listen(TCP_server, 5) == SOCKET_ERROR)
//         cout << "Listen Failed && error -> " << WSAGetLastError() << endl;

//     cout << "Connecting ..." << endl;
//     struct sockaddr_in TCPclient_add;
//     int iTCPclient_add = sizeof(TCPclient_add);
//     SOCKET sAcceptsocket = accept(TCP_server, (SOCKADDR *)&TCPclient_add, &iTCPclient_add);
//     if(sAcceptsocket == INVALID_SOCKET)
//         cout << "Accept Failed && error -> " << WSAGetLastError() << endl;
//     cout << "Server Accepted" << endl;
    
//     //nhan data
//     char *recvbuffer = new char [1024];
//     int irecvbuffer = 1025;
//     if(recv(sAcceptsocket, recvbuffer, irecvbuffer, 0) == SOCKET_ERROR)
//         cout << "Receive Fail && error -> " << WSAGetLastError() << endl;
//     cout << "Client << " << recvbuffer;
//     // cout << endl;

//     //gui data
//     char *sentbuffer = new char [1024];
//     int isentbuffer = strlen(recvbuffer);
//     fill(sentbuffer, sentbuffer+1024, 0);
//     transform(recvbuffer, recvbuffer+isentbuffer, sentbuffer, ::toupper);
//     cout << "Server >> " << sentbuffer << endl;
//     if(send(sAcceptsocket, sentbuffer, isentbuffer, 0) == SOCKET_ERROR)
//         cout << "Sent Fail && error -> " << WSAGetLastError() << endl;

//     if(closesocket(TCP_server) == SOCKET_ERROR)
//         cout << "Close Socket Failed && error -> " << WSAGetLastError() << endl;
    
//     if(WSACleanup() == SOCKET_ERROR)
//         cout << "Clean Fail && error -> " << WSAGetLastError() << endl;
//     return 0;
// }

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>

int main(int argument, char const *argv[]){
    printf("\n---TCP Server ---\n");

    WSADATA winsock;
    int WSAStart = WSAStartup(MAKEWORD(2, 2), &winsock);
    if(WSAStart != 0)
        printf("Startup Failed.\n");
    
    SOCKET TCPserver = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(TCPserver == INVALID_SOCKET)
        printf("Socket Creat Failed.\n");

    struct sockaddr_in TCPserver_add;
    TCPserver_add.sin_family = AF_INET;
    TCPserver_add.sin_addr.s_addr = inet_addr("192.168.0.115");
    TCPserver_add.sin_port = htons(5000);

    int sbind = bind(TCPserver, (SOCKADDR *)&TCPserver_add, sizeof(TCPserver_add));
    if(sbind == SOCKET_ERROR)
        printf("Bind Failed && error -> %d\n", WSAGetLastError());
    
    int slisten = listen(TCPserver, 5);
    if(slisten == SOCKET_ERROR)
        printf("Listen Failed && error -> ", WSAGetLastError());
    
    printf("Connecting ...\n");

    struct sockaddr_in TCPclient;
    int iTCPclient = sizeof(TCPclient);
    SOCKET sAccept = accept(TCPserver,(SOCKADDR *)&TCPclient, &iTCPclient);
    if(sAccept == SOCKET_ERROR)
        printf("Connect Failed && error -> %d\n", WSAGetLastError());
    
    printf("Server Accepted ...\n");

    while(1){
        char recvbuffer[1024];
        int irecvbuffer = sizeof(recvbuffer);
        int irecv = recv(sAccept, recvbuffer, irecvbuffer, 0);
        if(irecv == SOCKET_ERROR)
            printf("Receive Failed && error -> %d\n", WSAGetLastError());
        printf("%c", recvbuffer);
    }
    // char sentbuffer[1024];
    // int isentbuffer = strlen(recvbuffer);
    // memset(sentbuffer, 0, sizeof(sentbuffer));
    // strcpy(sentbuffer, recvbuffer);
    // int isent = send(sAccept, sentbuffer, isentbuffer, 0);
    //     printf("Server >> %s", sentbuffer);
    // if(isent == SOCKET_ERROR)
    //     printf("Send Failed && error -> %d\n", WSAGetLastError());

    int iclose = closesocket(TCPserver);
    if(iclose == SOCKET_ERROR)
        printf("Close Failed && error -> %d\n", WSAGetLastError());

    int iclean = WSACleanup();
    if(iclean == SOCKET_ERROR)
        printf("Clean Failed && error -> %d\n", WSAGetLastError());

    return 0;


}