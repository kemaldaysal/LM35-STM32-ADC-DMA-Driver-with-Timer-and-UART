/*
 * application_layer_driver.h
 *
 *  Created on: May 28, 2024
 *      Author: Kemal
 */

#ifndef INC_APPLICATION_LAYER_DRIVER_H_
#define INC_APPLICATION_LAYER_DRIVER_H_

#include "uart_driver.h"
#include "timer_driver.h"
#include "adc_dma_driver.h"

typedef enum {
	ALL_INIT_FAILED=0,
	ALL_INIT_OK
} status_All_Init_e;

typedef enum {
	START_FAILED = 0,
	STARTED_OK
} status_Start_Reading_e;

status_All_Init_e applayer_main_init_function(uint16_t measurement_period_in_ms, uint32_t uart_baudrate);
status_Start_Reading_e applayer_start_reading(void);

#endif /* INC_APPLICATION_LAYER_DRIVER_H_ */
