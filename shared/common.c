#include "common.h"
#include "sensor_conf.h"
#include "messages.h"

#include "master_selection.h"
#include "settings_vars.h"

#include <bmac.h>
#include <stdlib.h>
#include <nrk_error.h>
#include <stdio.h>

uint8_t aes_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};

void receiveLoop(void (*receiveFunction)(uint8_t *buf), bool *boolVar, bool boolValue)
{
	uint8_t len;
	int8_t rssi;
	uint8_t *rx_local_buf;

	while (*boolVar == boolValue)
	{
		// Wait until an RX packet is received
		bmac_wait_until_rx_pkt ();

		nrk_led_set (GREEN_LED);
		rx_local_buf = bmac_rx_pkt_get (&len, &rssi);
		printf("R ");

		if (len > 2)
			receiveFunction(rx_local_buf);

//		printRXPacketDebugMessage(rx_local_buf, len, rssi);
		printf(" l: %u, r: %d\n", len, rssi);

		nrk_led_clr (GREEN_LED);

		// Release the RX buffer so future packets can arrive
		bmac_rx_pkt_release ();
	}
}

/**
 * infinite receive loop
 */
void infReceiveLoop(void (*receiveFunction)(uint8_t *buf))
{
	bool fakeBool = true;
	receiveLoop(receiveFunction, &fakeBool, 1);
}

void printPacket(uint8_t *buf)
{
	uint8_t i;

	int length = strlen((char *)buf);
	printf ("[");

	for (i = 0; i < length; i++)
		printf ("%c", buf[i]);

	printf ("]\r\n");
	fflush(stdout);
}

void printRXPacketDebugMessage(uint8_t *packet, uint8_t length, int8_t rssi)
{
	printf ("RX: len=%d RSSI=%d ", length, rssi);
	printPacket(packet);
}

void initialization(uint8_t* rx_buf)
{
	isMaster = false;
	gatewayConfDone = false;
	masterConfDone = false;

	initMessages();

	nrk_setup_ports ();
	nrk_setup_uart (UART_BAUDRATE_115K2);

	nrk_init ();

	nrk_time_set (0, 0);

	nrk_kprintf(PSTR("Booting...\r\n"));
//	adc_Powersave();
	printf("My MAC is %u\n", MAC_ADDR);

	// bmac - the radio interface configuration

	bmac_task_config ();

	// init bmac on channel 15 --- CHANGE THIS ACCORDING TO YOUR GROUP NUMBER
	// channel may be in the range 11..26, see wiki for channel assignment according to your group number
	bmac_init (17);

	// must be after bmac_init
	bmac_addr_decode_set_my_mac(MAC_ADDR);
	bmac_addr_decode_enable();

	// Enable AES 128 bit encryption
	// When encryption is active, messages from plaintext
	// source will still be received.
	bmac_encryption_set_key(aes_key,16);
	bmac_encryption_enable();
	// bmac_encryption_disable();

	// By default the RX check rate is 100ms
	// below shows how to change that
	nrk_time_t check_period;
	int8_t val;

	check_period.secs = 0;
	check_period.nano_secs = 25*NANOS_PER_MS;
	val = bmac_set_rx_check_rate(check_period);

	if(val == NRK_ERROR)
		nrk_kprintf( PSTR("ERROR setting bmac rate\r\n" ));

	// The default Clear Channel Assement RSSI threshold is -45
	// Setting this value higher means that you will only trigger
	// receive with a very strong signal.  Setting this lower means
	// bmac will try to receive fainter packets.  If the value is set
	// too high or too low performance will suffer greatly.
	bmac_set_cca_thresh(-50);

	// This sets the next RX buffer.
	// This can be called at anytime before releaseing the packet
	// if you wish to do a zero-copy buffer switch
	bmac_rx_pkt_set_buffer (rx_buf, RF_MAX_PAYLOAD_SIZE);

}

