// #include <winsock2.h>
// #include <bits/stdc++.h>

// using namespace std;

// int main(int argument, char const *argv[]){
//     cout << endl << "---TCP Client--" << endl;

//     WSADATA winsock;
//     int iwsastart = WSAStartup(MAKEWORD(2, 2), &winsock);
//     if(iwsastart != 0)
//         cout << "Start fail && error -> " << WSAGetLastError() << endl;
    
//     // khoi tao socket
//     SOCKET TCPclient = socket(AF_INET, SOCK_STREAM, 0);
//     if(TCPclient == INVALID_SOCKET)
//         cout << "TCP creat fail && error -> " << WSAGetLastError() << endl;

//     struct sockaddr_in TCPserveradd;
//     TCPserveradd.sin_family = AF_INET;
//     TCPserveradd.sin_addr.s_addr = inet_addr ("192.168.0.115");         
//     TCPserveradd.sin_port = htons(5000);

//     int iconnect = connect(TCPclient, (SOCKADDR *)&TCPserveradd, sizeof(TCPserveradd));
//     if(iconnect == SOCKET_ERROR)
//         cout << "Connect fail && error -> " << WSAGetLastError() << endl;
//     cout << "Connected" << endl;

//     //sent data tu client --> server
//     cout << "Client >> ";
//     char sentbuffer[1024];
//     // char *sentbuffer = new char [1024];
//     scanf("\n");
//     fgets(sentbuffer, sizeof(sentbuffer), stdin);
//     int isentbuffer = strlen(sentbuffer);
//     int isend = send(TCPclient, sentbuffer, isentbuffer, 0);
//     if(isend == SOCKET_ERROR)
//         cout << "Sent fail && error -> " << WSAGetLastError() << endl;

//     //nhan data tu server --> client
//     // char recvbuffer[2048];
//     // int irecvbuffer = 2049;
//     char *recvbuffer = new char [1024];
//     int irecvbuffer = 1025;
//     int irecv = recv(TCPclient, recvbuffer, irecvbuffer, 0);
//     if(irecv == SOCKET_ERROR)
//         cout << "Receive fail && error -> " << WSAGetLastError() << endl;
//     cout << "Server << " << recvbuffer << endl;

//     //close socket
//     int iclosesocket = closesocket(TCPclient);
//     if(iclosesocket == SOCKET_ERROR)
//         cout << "Close fail && error -> " << WSAGetLastError() << endl;
//     int iwsaclean = WSACleanup();
//     if(iwsaclean == SOCKET_ERROR)
//         cout << "Clean fail && error -> " << WSAGetLastError() << endl;

//     return 0;
// }


#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <windows.h>


int main(int argument, char const *argv[]){
    printf("\n--- TCP Client ---\n");

    WSADATA winsock;
    int iwsastart = WSAStartup(MAKEWORD(2, 2), &winsock);
    if(iwsastart != 0)
        printf("Start Failed && error -> %d\n", WSAGetLastError());
    
    // khoi tao socket
    SOCKET TCPclient = socket(AF_INET, SOCK_STREAM, 0);
    if(TCPclient == INVALID_SOCKET)
        printf("TCP creat fail && error -> %d\n", WSAGetLastError());

    struct sockaddr_in TCPserveradd;
    TCPserveradd.sin_family = AF_INET;
    TCPserveradd.sin_addr.s_addr = inet_addr ("192.168.0.115");         
    TCPserveradd.sin_port = htons(5000);

    int iconnect = connect(TCPclient, (SOCKADDR *)&TCPserveradd, sizeof(TCPserveradd));
    if(iconnect == SOCKET_ERROR)
        printf("Connect fail && error -> %d\n", WSAGetLastError());
    printf("Connected...\n");

    //sent data tu client --> server
    printf("Client >> ");
    char sentbuffer[1024];
    scanf("\n");
    fgets(sentbuffer, sizeof(sentbuffer), stdin);
    int isentbuffer = strlen(sentbuffer);
    int isend = send(TCPclient, sentbuffer, isentbuffer, 0);
    if(isend == SOCKET_ERROR)
        printf("Sent fail && error -> %d\n", WSAGetLastError());

    //nhan data tu server --> client
    char recvbuffer[1024]={};
    int irecvbuffer = sizeof(recvbuffer);
    int irecv = recv(TCPclient, recvbuffer, irecvbuffer, 0);
    if(irecv == SOCKET_ERROR)
        printf("Receive fail && error -> %d\n", WSAGetLastError());
    printf("Server << %s", recvbuffer);

    //close socket
    int iclosesocket = closesocket(TCPclient);
    if(iclosesocket == SOCKET_ERROR)
        printf("Close fail && error -> %d\n", WSAGetLastError());
    int iwsaclean = WSACleanup();
    if(iwsaclean == SOCKET_ERROR)
        printf("Clean fail && error -> %d\n", WSAGetLastError());

    return 0;
}
