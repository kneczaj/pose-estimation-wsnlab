#ifndef TASK_RX_H
#define TASK_RX_H

#include <nrk.h>
#include <hal.h>
#include <include.h>
#include <nrk_task.h>

void initialize_task_rx();
void task_rx();
uint8_t compareBatteryVoltage(uint8_t *buf);
void receiveFunction(uint8_t *buf);

uint8_t rx_buf[RF_MAX_PAYLOAD_SIZE];
nrk_task_type RX_TASK;
NRK_STK rx_task_stack[NRK_APP_STACKSIZE];

#endif
