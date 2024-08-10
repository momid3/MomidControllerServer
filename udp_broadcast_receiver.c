#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFLEN 3000
#define PORT 3333

void startUdpBroadcastReceiver() {
    SOCKET s;
    struct sockaddr_in server, client;
    int slen, recv_len;
    char buf[BUFLEN];
    WSADATA wsa;

    slen = sizeof(client);

    printf("initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("failed. error code: %d\n", WSAGetLastError());
        return;
    }
    printf("intialized.\n");

    if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
        printf("could not create socket: %d\n", WSAGetLastError());
        return;
    }
    printf("socket created.\n");

    BOOL reuseAddr = TRUE;
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseAddr, sizeof(reuseAddr)) == SOCKET_ERROR) {
        printf("setsockopt(SO_REUSEADDR) failed with error code: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return;
    }

    BOOL broadcastPermission = TRUE;
    if (setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&broadcastPermission, sizeof(broadcastPermission)) == SOCKET_ERROR) {
        printf("setsockopt(SO_BROADCAST) failed with error code: %d\n", WSAGetLastError());
        closesocket(s);
        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind the socket
    if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code: %d\n", WSAGetLastError());
        return;
    }
    printf("bind done.\n");

        printf("waiting for data...\n");
        fflush(stdout);

        memset(buf, '\0', BUFLEN);

        if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *)&client, &slen)) == SOCKET_ERROR) {
            printf("recvfrom() failed with error code: %d\n", WSAGetLastError());
            return;
        }

        printf("received packet from %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        printf("data: %s\n", buf);

        if (sendto(s, buf, BUFLEN, 0, (struct sockaddr *)&client, slen) == SOCKET_ERROR) {
            printf("sendto() failed with error code: %d\n", WSAGetLastError());
            return;
        }
        printf("acknowledgment sent to %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

    closesocket(s);
    WSACleanup();
}
