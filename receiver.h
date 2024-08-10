//
// Created by momid on 6/9/2024.
//

#ifndef MOMIDCONTROLLER_RECEIVER_H
#define MOMIDCONTROLLER_RECEIVER_H

#endif //MOMIDCONTROLLER_RECEIVER_H

typedef void (*Lambda)(unsigned char*);

void startUdpBroadcastReceiver();

void startTcpServer(Lambda onPacket);
