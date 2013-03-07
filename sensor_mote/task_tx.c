#include "task_tx.h"
#include "sensor_conf.h"
#include "shared/messages.h"
#include "shared/sending.h"
#include "shared/master_selection.h"

#include <stdio.h>
#include <stdlib.h>
#include <ulib.h>

#include <avr/sleep.h>
#include <bmac.h>
#include <nrk_error.h>
#include <adc_driver.h>
#include <mts310cb_driver.h>

// TRANSMIT TASK

#define MAX_QUEUE_SIZE 7

uint8_t tx_buf[RF_MAX_PAYLOAD_SIZE];

struct SendQueueItem
{
	// mac 0 means forward to gateway
	uint16_t recipientMac;
	uint8_t message[RF_MAX_PAYLOAD_SIZE];
	uint8_t size;
};

struct SendQueueItem sendQueue[MAX_QUEUE_SIZE];
uint8_t sendQueueSize;

//struct SendQueueItem *firstQueueItem;
//struct SendQueueItem *lastQueueItem;

void task_tx()
{
	uint8_t messageSize;

    // Wait until the tx_task starts up bmac
    // This should be called by all tasks using bmac that
    // do not call bmac_init()...
    while (!bmac_started ())
        nrk_wait_until_next_period ();

    requestConfiguration(tx_buf);

    // measure
    for(;;)
    {
    	messageSize = getMeasurements(tx_buf);
    	printMeasurementMessage(tx_buf);
		forwardPacket(tx_buf, messageSize);

		// forward queued messages
		while (sendQueueSize != 0)
		{
			struct SendQueueItem *sendItem = dequeue();
			printf("label: %c%c, recipient: %u ", sendItem->message[0], sendItem->message[1], sendItem->recipientMac);
			if (sendItem->recipientMac)
				sendToMac(sendItem->recipientMac, sendItem->message, sendItem->size);
			else
				forwardPacket(sendItem->message, sendItem->size);
		}

		nrk_wait_until_next_period ();
	}
}


uint8_t getMeasurements(uint8_t* buf)
{
    float accelerationX = (float)mts310cb_Accelerometer_x_GetCounts();
    float accelerationY = (float)mts310cb_Accelerometer_y_GetCounts();
    float battery = adc_GetBatteryVoltage();

    return getMeasurementMessage(accelerationX, accelerationY, battery, buf);
}

void dequeueLoop()
{
	// forward queued messages
	while (sendQueueSize != 0)
	{
		struct SendQueueItem *sendItem = dequeue();
		printf("label: %c%c, recipient: %u ", sendItem->message[0], sendItem->message[1], sendItem->recipientMac);
		if (sendItem->recipientMac)
			sendToMac(sendItem->recipientMac, sendItem->message, sendItem->size);
		else
			forwardPacket(sendItem->message, sendItem->size);
	}
}

void queueToSend(const uint8_t *packet, const uint16_t recipientMac, const uint8_t size)
{
	printf("queue");
	struct SendQueueItem *newItem = &sendQueue[sendQueueSize];

	memcpy(newItem->message, packet, size);
	newItem->recipientMac = recipientMac;
	newItem->size = size;

	if (sendQueueSize < MAX_QUEUE_SIZE)
		sendQueueSize++;
}

struct SendQueueItem* dequeue()
{
	printf("dequeue: ");
	sendQueueSize--;
	return &sendQueue[sendQueueSize];
}

void initialize_task_tx()
{
	sendQueueSize = 0;
    TX_TASK.task = task_tx;
    nrk_task_set_stk( &TX_TASK, tx_task_stack, NRK_APP_STACKSIZE);
    TX_TASK.prio = 2;
    TX_TASK.FirstActivation = TRUE;
    TX_TASK.Type = BASIC_TASK;
    TX_TASK.SchType = PREEMPTIVE;
    TX_TASK.period.secs = 1;
    TX_TASK.period.nano_secs = 0;
    TX_TASK.cpu_reserve.secs = 0;
    TX_TASK.cpu_reserve.nano_secs = 300 * NANOS_PER_MS;
    TX_TASK.offset.secs = 0;
    TX_TASK.offset.nano_secs = 0;
    nrk_activate_task (&TX_TASK);
}
