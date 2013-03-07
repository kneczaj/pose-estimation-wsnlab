#include "task_rx.h"
#include "task_tx.h"
#include "sensor_conf.h"

#include "shared/common.h"
#include "shared/messages.h"
#include "shared/settings_vars.h"
#include "shared/master_selection.h"
#include "shared/sending.h"

#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <bmac.h>
#include <nrk_error.h>
#include <adc_driver.h>
#include <stdlib.h>
#include <stdint.h>

// RECEIVE TASK

#define BATTERY_MULTIPLIER 1.02

uint8_t buf[RF_MAX_PAYLOAD_SIZE];
uint8_t reply_buf[RF_MAX_PAYLOAD_SIZE];

void initialize_task_rx()
{
    RX_TASK.task = task_rx;
    nrk_task_set_stk( &RX_TASK, rx_task_stack, NRK_APP_STACKSIZE);
    RX_TASK.prio = 2;
    RX_TASK.FirstActivation = TRUE;
    RX_TASK.Type = BASIC_TASK;
    RX_TASK.SchType = PREEMPTIVE;
    RX_TASK.period.secs = 1;
    RX_TASK.period.nano_secs = 0;
    RX_TASK.cpu_reserve.secs = 0;
    RX_TASK.cpu_reserve.nano_secs = 300 * NANOS_PER_MS;
    RX_TASK.offset.secs = 0;
    RX_TASK.offset.nano_secs = 0;
    nrk_activate_task (&RX_TASK);
}

void task_rx()
{
    infReceiveLoop(receiveFunction);
}

void receiveFunction(uint8_t *buf)
{
	uint8_t messageSize;

	// gateway selection
	if (hasLabel(buf, "GW"))
	{
		printGatewaySelectionMessage(buf);
		configureGateway(buf);
	}
	// master selection
	else if (hasLabel(buf, "MS"))
	{
		printMasterSelectionMessage(buf);
		configureMaster(buf);
	}
	// accelerometer measurement
	else if (gatewayConfDone && masterConfDone && hasLabel(buf, "AC"))
	{
		MeasurementMessage *received = (MeasurementMessage *)buf;
		printMeasurementMessage(buf);
		// inform about the change of the master
		if (!isMaster)
		{
			printf("Wrongly addressed; ");
			printf("old master: %u, new: %u; ", masterMAC, received->senderMac);
			messageSize = getMasterSelectionMessage(reply_buf);
			queueToSend(reply_buf, received->senderMac, messageSize);
//			sendToMac(received->senderMac, reply_buf, messageSize);
		}
		else // isMaster
		{
			// reconfiguration
			if (received->battery > BATTERY_MULTIPLIER * adc_GetBatteryVoltage())
			{
				setMaster(received->senderMac);
				messageSize = getMasterSelectionMessage(reply_buf);
				queueToSend(reply_buf, 0xFFFF, messageSize); // to everyone (broadcast)
			}
			// forward to the gateway
			queueToSend(buf, 0, sizeof(MeasurementMessage));
//			forwardPacket(buf, sizeof(*received));
		}
	}
	else if (isMaster)
	{
		// master id request
		if (hasLabel(buf, "MR"))
		{
			printMasterRequestMessage(buf);
			MasterRequest *received = (MasterRequest *)buf;
			messageSize = getMasterSelectionMessage(reply_buf);
			queueToSend(reply_buf, received->senderMac, messageSize);
//			sendToMac(received->senderMac, reply_buf, messageSize);
		}
		// gateway id request
		else if (hasLabel(buf, "GR"))
		{
			printf("GR rec; ");
			printGatewayRequestMessage(buf);
			GatewayRequest *received = (GatewayRequest *)buf;
			if (gatewayConfDone)
			{
				messageSize = getGatewaySelectionMessage(reply_buf);
				queueToSend(reply_buf, received->senderMac, messageSize);
//				sendToMac(received->senderMac, reply_buf, messageSize);
			}
		}
	}
	else
	    printf("Unknown message, label %c%c ", buf[0], buf[1]);
}
