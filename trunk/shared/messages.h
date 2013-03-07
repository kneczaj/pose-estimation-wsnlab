/*
 * messages.h
 *
 *  Created on: Feb 3, 2013
 *      Author: ga46kuj
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdint.h>
#include <stdbool.h>

#include "sensor_conf.h"


struct masterRequestStruct
{
	char label[2];
	uint16_t senderMac;
};

struct gatewayRequestStruct
{
	char label[2];
	uint16_t senderMac;
};

struct masterSelectionStruct
{
	char label[2];
	uint16_t masterMac;
};

struct gatewaySelectionStruct
{
	char label[2];
	uint16_t gatewayMac;
};

struct measurementMessageStruct
{
	char label[2];
	uint16_t senderMac;
	float x;
	float y;
	float battery;
};

typedef struct masterSelectionStruct MasterSelection;
typedef struct masterRequestStruct MasterRequest;
typedef struct gatewaySelectionStruct GatewaySelection;
typedef struct gatewayRequestStruct GatewayRequest;
typedef struct measurementMessageStruct MeasurementMessage;

void initMessages();
void printString(char *buf, int length);
bool hasLabel(uint8_t *str, char *label);

/**
 * return size of messages
 */
uint8_t getMasterRequestMessage(uint8_t *buf);
uint8_t getGatewayRequestMessage(uint8_t *buf);
uint8_t getMasterSelectionMessage(uint8_t *buf);
uint8_t getGatewaySelectionMessage(uint8_t *buf);
uint8_t getMeasurementMessage(float x, float y, float battery, uint8_t *buf);

void printGatewayRequestMessage(uint8_t *buf);
void printGatewaySelectionMessage(uint8_t *buf);
void printMasterSelectionMessage(uint8_t *buf);
void printMasterRequestMessage(uint8_t *buf);
void printMeasurementMessage(uint8_t *buf);

#endif /* MESSAGES_H_ */
