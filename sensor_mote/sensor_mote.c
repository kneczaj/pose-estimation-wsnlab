//#include "sensor_conf.h"
#include "shared/master_selection.h"
#include "shared/settings_vars.h"
#include "shared/common.h"

#include "task_rx.h"
#include "task_tx.h"
#include "sensor_conf.h"

#include <nrk.h>
#include <include.h>
#include <ulib.h>
#include <stdio.h>
#include <avr/sleep.h>
#include <hal.h>
#include <bmac.h>
#include <nrk_error.h>
#include <adc_driver.h>
#include <stdlib.h>

#include <mts310cb_driver.h>

void nrk_create_taskset();

uint8_t rx_buf[RF_MAX_PAYLOAD_SIZE];


int main()
{
    initialization(rx_buf);

    // Initialize the mts310 sensorboard driver
    mts310cb_init();

    // Switch on the power for the acceleration sensor
    mts310cb_Accelerometer_Power(POWER_ON);

    isMaster = (MAC_ADDR == 1);
    if (isMaster)
    	setMaster(MAC_ADDR);

    nrk_create_taskset ();
	nrk_start();
    return 0;
}

void nrk_create_taskset()
{
    initialize_task_tx();
    initialize_task_rx();
}
