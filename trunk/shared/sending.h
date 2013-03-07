/*
 * sending.h
 *
 *  Created on: Feb 3, 2013
 *      Author: ga46kuj
 */

#ifndef SENDING_H_
#define SENDING_H_

#include <stdint.h>

int8_t send(uint8_t *buf, uint8_t messageSize);
int8_t sendToMaster(uint8_t *buf, uint8_t messageSize);
int8_t sendToGateway(uint8_t *buf, uint8_t messageSize);
int8_t sendToEveryone(uint8_t *buf, uint8_t messageSize);
int8_t sendToMac(uint16_t mac, uint8_t *buf, uint8_t messageSize);
int8_t forwardPacket(uint8_t *buf, uint8_t messageSize);

#endif /* SENDING_H_ */
