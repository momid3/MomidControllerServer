#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "receiver.h"

#pragma comment(lib, "Ws2_32.lib")

#define TCP_PORT "3338"
#define DEFAULT_BUFLEN 3000

void startTcpServer(Lambda onPacket) {
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = 11;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, TCP_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return;
    }

    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    while (1) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            continue;
        }

        while (1) {
            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);

                int size = iResult;

                if (size != 11) {
                    printf("size is not as expected");
                    break;
                }

                onPacket((unsigned char *) &(recvbuf));

                iSendResult = send(ClientSocket, recvbuf, iResult, 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed: %d\n", WSAGetLastError());
                    break;
                }
                printf("Bytes sent: %d\n", iSendResult);
            } else if (iResult == 0) {
                printf("Connection closing...\n");
                break;
            } else {
                printf("recv failed: %d\n", WSAGetLastError());
                break;
            }
        }
    }

    closesocket(ClientSocket);
    WSACleanup();
}
