#include <stdio.h>
#include <windows.h>
#include <ViGEm/Client.h>
#include "receiver.h"

PVIGEM_CLIENT client;
PVIGEM_TARGET target;
XUSB_REPORT report;

int isPressed = 0;

void setKeys(unsigned char* packet) {
    short leftRight = (short) packet[0];
    leftRight *= 256;
    short upDown = (short) packet[1];
    upDown *= 256;

    report.sThumbLX = leftRight;
    report.sThumbLY = upDown;
    report.wButtons = 0;

    if (((short) packet[2]) == 1) {
        report.wButtons |= XUSB_GAMEPAD_A;
    }

    if (((short) packet[3]) == 1) {
        report.wButtons |= XUSB_GAMEPAD_B;
    }

    if (((short) packet[4]) == 1) {
        report.wButtons |= XUSB_GAMEPAD_X;
    }

    if (((short) packet[5]) == 1) {
        report.wButtons |= XUSB_GAMEPAD_Y;
    }

    if (!VIGEM_SUCCESS(vigem_target_x360_update(client, target, report))) {
        printf("Failed to update target state.\n");
        vigem_target_remove(client, target);
        vigem_target_free(target);
        vigem_disconnect(client);
        vigem_free(client);
    }
}

int main() {
    printf("o");

    client = vigem_alloc();
    if (client == NULL) {
        printf("Failed to allocate ViGEm client.\n");
        return -1;
    }

    if (!VIGEM_SUCCESS(vigem_connect(client))) {
        printf("Failed to connect to ViGEm bus.\n");
        vigem_free(client);
        return -1;
    }

    target = vigem_target_x360_alloc();

    if (!VIGEM_SUCCESS(vigem_target_add(client, target))) {
        printf("Failed to add target to the bus.\n");
        vigem_target_free(target);
        vigem_disconnect(client);
        vigem_free(client);
        return -1;
    }

    ZeroMemory(&report, sizeof(XUSB_REPORT));

//    vigem_target_remove(client, target);
//    vigem_target_free(target);
//    vigem_disconnect(client);
//    vigem_free(client);

    startUdpBroadcastReceiver();

    startTcpServer(setKeys);
}
