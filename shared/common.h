#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdbool.h>

void printPacket(uint8_t *buf);

void initialization(uint8_t* rx_buf);

void printRXPacketDebugMessage(uint8_t *packet, uint8_t length, int8_t rssi);

void receiveLoop(void (*receiveFunction)(uint8_t *buf), bool *boolVar, bool boolValue);

void infReceiveLoop(void (*receiveFunction)(uint8_t *buf));

void requestConfiguration(uint8_t *tx_buf);

#endif
