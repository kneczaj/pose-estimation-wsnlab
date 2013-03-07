#ifndef TASK_TX_H
#define TASK_TX_H

#include <nrk.h>
#include <hal.h>
#include <include.h>

void initialize_task_tx();
void task_tx();
void queueToSend(const uint8_t *packet, const uint16_t recipientMac, const uint8_t size);

uint8_t getMeasurements(uint8_t* buf);
struct SendQueueItem* dequeue();
//struct SendQueueItem* dequeue();

//#define TX_ARRAY_SIZE 5

//extern uint8_t tx_tab[TX_ARRAY_SIZE][RF_MAX_PAYLOAD_SIZE];
//extern uint8_t tx_tab_elments;

//extern struct SendQueueItem *firstQueueItem;
//extern struct SendQueueItem *lastQueueItem;

nrk_task_type TX_TASK;
NRK_STK tx_task_stack[NRK_APP_STACKSIZE];

#endif
