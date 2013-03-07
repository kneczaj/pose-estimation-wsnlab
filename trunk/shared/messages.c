/*
 * messages.c
 *
 *  Created on: Feb 3, 2013
 *      Author: ga46kuj
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "messages.h"
#include "sensor_conf.h"

#include "master_selection.h"

struct masterRequestStruct masterRequest;
struct gatewayRequestStruct gatewayRequest;
struct masterSelectionStruct masterSelection;
struct gatewaySelectionStruct gatewaySelection;
MeasurementMessage measurementMessage;

void initMessages()
{
	strncpy(masterRequest.label, "MR", 2);
	masterRequest.senderMac = MAC_ADDR;

	strncpy(gatewayRequest.label, "GR", 2);
	gatewayRequest.senderMac = MAC_ADDR;

	strncpy(masterSelection.label, "MS", 2);
//	masterSelection.senderMac = MAC_ADDR;

	strncpy(gatewaySelection.label, "GW", 2);
//	gatewaySelection.senderMac = MAC_ADDR;

	strncpy(measurementMessage.label, "AC", 2);
	measurementMessage.senderMac = MAC_ADDR;
}

// get functions

uint8_t getMasterRequestMessage(uint8_t *buf)
{
	memcpy(buf, &masterRequest, sizeof(masterRequest));
	return sizeof(masterRequest);
}

uint8_t getGatewayRequestMessage(uint8_t *buf)
{
	memcpy(buf, &gatewayRequest, sizeof(gatewayRequest));
	return sizeof(gatewayRequest);
}

uint8_t getMasterSelectionMessage(uint8_t *buf)
{
	masterSelection.masterMac = masterMAC;
	memcpy(buf, &masterSelection, sizeof(masterSelection));
	return sizeof(masterSelection);
}

uint8_t getGatewaySelectionMessage(uint8_t *buf)
{
	gatewaySelection.gatewayMac = gatewayMAC;
	memcpy(buf, &gatewaySelection, sizeof(gatewaySelection));
	return sizeof(gatewaySelection);
}

uint8_t getMeasurementMessage(float x, float y, float battery, uint8_t *buf)
{
	measurementMessage.x = x;
	measurementMessage.y = y;
	measurementMessage.battery = battery;
	memcpy(buf, &measurementMessage, sizeof(measurementMessage));
	return sizeof(measurementMessage);
}

// print function

//void genericPrint(uint8_t *buf)
//{
//	GatewaySelection *message = (GatewaySelection *)buf;
//	printString(message->label, 2);
//	printf("#senderMac:%u ", message->senderMac);
//}

void printGatewayRequestMessage(uint8_t *buf)
{
	GatewayRequest *message = (GatewayRequest *)buf;
	printString(message->label, 2);
	printf("#senderMac:%u ", message->senderMac);
}

void printGatewaySelectionMessage(uint8_t *buf)
{
	GatewaySelection *message = (GatewaySelection *)buf;
	printString(message->label, 2);
	printf("#gatewayMac:%u ", message->gatewayMac);
}

void printMasterSelectionMessage(uint8_t *buf)
{
	MasterSelection *message = (MasterSelection *)buf;
	printString(message->label, 2);
	printf("#masterMac:%u ", message->masterMac);
}

void printMasterRequestMessage(uint8_t *buf)
{
	MasterRequest *message = (MasterRequest *)buf;
	printString(message->label, 2);
	printf("#senderMac:%u ", message->senderMac);
}

void printMeasurementMessage(uint8_t *buf)
{
	MeasurementMessage *message = (MeasurementMessage *)buf;
	printString(message->label, 2);
	printf("#senderMac:%u;x:%f;y:%f;bat:%f ", message->senderMac, (double)message->x, (double)message->y, (double)message->battery);
}

bool hasLabel(uint8_t *str, char *label)
{
	return (strncmp((char *)str, label, 2) == 0);
}

void printString(char *buf, int length)
{
	int i;
	for (i = 0; i < length; i++)
		printf ("%c", buf[i]);
}

