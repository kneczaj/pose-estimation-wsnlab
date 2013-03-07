/*
 * master_selection.c
 *
 *  Created on: Jan 21, 2013
 *      Author: ga46kuj
 */

#include "master_selection.h"
#include "settings_vars.h"
#include "common.h"
#include "sensor_conf.h"
#include "messages.h"
#include "sending.h"

#include <bmac.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

bool masterConfDone;
bool isMaster;

bool gatewayConfDone;

uint16_t masterMAC;
uint16_t gatewayMAC;

void waitForConfiguration()
{
	// wait for bmac
	while (!bmac_started ())
	    nrk_wait_until_next_period();

	// wait for master selection
	while(!(gatewayConfDone  && masterConfDone))
		nrk_wait_until_next_period();
}

void requestConfiguration(uint8_t *tx_buf)
{
	uint8_t messageSize;
	printf("MASTER CONFIGURATION\n");
	while(!masterConfDone)
	{
		nrk_led_toggle(YELLOW_LED);
		messageSize = getMasterRequestMessage(tx_buf);
		printMasterRequestMessage(tx_buf);
		sendToEveryone(tx_buf, messageSize);
		nrk_led_toggle(YELLOW_LED);
		nrk_wait_until_next_period ();
	}

	printf("GATEWAY CONFIGURATION\n");
	while(!gatewayConfDone)
	{
		nrk_led_toggle(YELLOW_LED);
		messageSize = getGatewayRequestMessage(tx_buf);
		printGatewayRequestMessage(tx_buf);
		if (isMaster)
			sendToEveryone(tx_buf, messageSize);
		else
			sendToMaster(tx_buf, messageSize);
		nrk_led_toggle(YELLOW_LED);
		nrk_wait_until_next_period();
	}
	printf("CONFIGURATION DONE\n");
}

//uint16_t extractID(uint8_t *buf)
//{
//	int len = strlen((char *)buf);
//	if (len < 7)
//		return -1; // error
//
//	if (buf[2] != '#'
//		|| buf[3] != 'I'
//		|| buf[4] != 'D'
//		|| buf[5] != ':')
//		return -1; // error
//
//	return (uint16_t)(buf[6] - 48);
//}

void configureGateway(uint8_t *buf)
{
	GatewaySelection *message = (GatewaySelection *)buf;
	gatewayMAC = message->gatewayMac;
	gatewayConfDone = true;
	printf("Gateway set to %u\n\r", gatewayMAC);
}

void configureMaster(uint8_t *buf)
{
	MasterSelection *message = (MasterSelection *)buf;
	setMaster(message->masterMac);
}

void setMaster(uint16_t mac)
{
	masterMAC = mac;
	masterConfDone = true;

	if (mac == MAC_ADDR)
	{
		printf("I am the master!\r\n");
		isMaster = true;
		nrk_led_set(RED_LED);
		bmac_set_rf_power(32);
	}
	else
	{
		bmac_set_rf_power(5);
		isMaster = false;
		nrk_led_clr(RED_LED);
	}

	printf("Master set to %d ", masterMAC);
}
