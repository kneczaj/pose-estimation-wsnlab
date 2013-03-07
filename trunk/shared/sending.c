/*
 * sending.c
 *
 *  Created on: Feb 3, 2013
 *      Author: ga46kuj
 */

#include <bmac.h>
#include "sending.h"
#include "master_selection.h"
#include "messages.h"

int8_t send(uint8_t *buf, uint8_t messageSize)
{
	nrk_led_toggle(RED_LED);
	int8_t result = bmac_tx_pkt(buf, messageSize);
	nrk_led_toggle(RED_LED);
	return result;
}

int8_t forwardPacket(uint8_t *buf, uint8_t messageSize)
{
	if (isMaster)
		return sendToGateway(buf, messageSize);
	else
		return sendToMaster(buf, messageSize);
}

int8_t sendToMaster(uint8_t *buf, uint8_t messageSize)
{
	if (isMaster)
		return sendToGateway(buf, messageSize);

	printf(" --> to MASTER(%u)\n", masterMAC);
//	printPacket(buf);
	bmac_addr_decode_dest_mac(masterMAC);
	return send(buf, messageSize);
}

int8_t sendToGateway(uint8_t *buf, uint8_t messageSize)
{
	printf("--> to GATEWAY(%u)\n", gatewayMAC);
//	printPacket(buf);
	bmac_addr_decode_dest_mac(gatewayMAC);
	return send(buf, messageSize);
}

int8_t sendToEveryone(uint8_t *buf, uint8_t messageSize)
{
	printf(" --> to EVERYONE\n");
//	printPacket(buf);
	bmac_addr_decode_dest_mac(0xFFFF);
	return send(buf, messageSize);
}

int8_t sendToMac(uint16_t mac, uint8_t *buf, uint8_t messageSize)
{
	printf(" --> to %u\n", mac);
//	printPacket(buf);
	bmac_addr_decode_dest_mac(mac);
	return send(buf, messageSize);
}
