#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <bmac.h>
#include <nrk_error.h>

#include "shared/common.h"

#include "sensor_conf.h"
#include "shared/master_selection.h"
#include "shared/messages.h"
#include "shared/sending.h"
#include "shared/settings_vars.h"

nrk_task_type RX_TASK;
nrk_task_type TX_TASK;

NRK_STK rx_task_stack[NRK_APP_STACKSIZE];
NRK_STK tx_task_stack[NRK_APP_STACKSIZE];
void rx_task (void);

void nrk_create_taskset ();
void receivePacket(uint8_t *local_rx_buf);

uint8_t tx_buf[RF_MAX_PAYLOAD_SIZE];
uint8_t rx_buf[RF_MAX_PAYLOAD_SIZE];

//#define uprintf(buf, value, args...) sprintf((char*)buf, value, args)
// look up args..

int main()
{
	initialization(rx_buf);
	gatewayConfDone = true;
	gatewayMAC = MAC_ADDR;

	printf("I'm the GATEWAY!, MAC: %u\n", gatewayMAC);
	nrk_led_toggle(YELLOW_LED);

	nrk_create_taskset ();
	nrk_start ();

	return 0;
}

void rx_task ()
{
    while (!bmac_started ())
        nrk_wait_until_next_period ();

    infReceiveLoop(&receivePacket);
}

void tx_task ()
{
    while (!bmac_started ())
        nrk_wait_until_next_period ();

    requestConfiguration(tx_buf);
    for (;;)
    	nrk_wait_until_next_period ();
}


void receivePacket(uint8_t *local_rx_buf)
{
	uint8_t messageSize;
    if (hasLabel(local_rx_buf, "MS"))
    {
    	printMasterSelectionMessage(local_rx_buf);
    	configureMaster(local_rx_buf);
    }

    // sends gateway id
    else if (hasLabel(local_rx_buf, "GR"))
    {
    	printGatewayRequestMessage(local_rx_buf);
    	GatewayRequest *message = (GatewayRequest *)local_rx_buf;
    	messageSize = getGatewaySelectionMessage(tx_buf);

    	printGatewaySelectionMessage(tx_buf);

    	sendToMac(message->senderMac, tx_buf, messageSize);
    	printGatewaySelectionMessage(tx_buf);
    }
    else if (hasLabel(local_rx_buf, "AC"))
    	printMeasurementMessage(local_rx_buf);
    else
    	printf("Unknown message, label %c%c ", local_rx_buf[0], local_rx_buf[1]);
}

void nrk_create_taskset ()
{
	RX_TASK.task = rx_task;
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

    TX_TASK.task = tx_task;
    nrk_task_set_stk( &TX_TASK, tx_task_stack, NRK_APP_STACKSIZE);
    TX_TASK.prio = 2;
    TX_TASK.FirstActivation = TRUE;
    TX_TASK.Type = BASIC_TASK;
    TX_TASK.SchType = PREEMPTIVE;
    TX_TASK.period.secs = 5;
    TX_TASK.period.nano_secs = 0;
    TX_TASK.cpu_reserve.secs = 0;
    TX_TASK.cpu_reserve.nano_secs = 300 * NANOS_PER_MS;
    TX_TASK.offset.secs = 0;
    TX_TASK.offset.nano_secs = 0;
    nrk_activate_task (&TX_TASK);

	nrk_activate_task (&RX_TASK);
}


