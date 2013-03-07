/*
 * master_selection.h
 *
 *  Created on: Jan 21, 2013
 *      Author: ga46kuj
 */

#ifndef MASTER_SELECTION_H_
#define MASTER_SELECTION_H_

#include <stdint.h>
#include <stdbool.h>

extern bool masterConfDone;
extern bool isMaster;

extern bool gatewayConfDone;

extern uint16_t masterMAC;
extern uint16_t gatewayMAC;

void waitForConfiguration();

void setMaster(uint16_t mac);
void configureGateway(uint8_t *buf);
void configureMaster(uint8_t *buf);

void requestConfiguration(uint8_t *tx_buf);
void waitForConfiguration();

//uint16_t extractID(uint8_t *buf);

#endif /* MASTER_SELECTION_H_ */
