/*
 * adc_dma_driver.h
 *
 *  Created on: May 28, 2024
 *      Author: Kemal
 */

#ifndef HARDWARE_DRIVERS_INC_ADC_DMA_DRIVER_H_
#define HARDWARE_DRIVERS_INC_ADC_DMA_DRIVER_H_

#include "stm32f0xx_hal.h"


#define ADC_CHANNEL_NUMBER ADC_CHANNEL_0
#define ADC_REF_VOLTAGE ( 3.316f )
#define ADC_RESOLUTION_SETTING (ADC_RESOLUTION_12B)
#define ADC_RESOLUTION ((uint16_t) 4096-1)
#define ADC_CONTINOUS_CONV_MODE_SETTING (ENABLE)
#define ADC_SAMPLING_TIME_SETTING (ADC_SAMPLETIME_239CYCLES_5)

#define TEN_MV_TO_ONE_V_CONVERSION_COEFFICIENT ((uint8_t) 100)

// ALSO, DMA CONTINOUS REQUESTS SETTING NEEDS TO BE ENABLED IN ADC CONFIG ON STM32CUBEMX !!

typedef enum{

	ADC_DMA_START_FAILED = 0,
	ADC_DMA_START_OK

}status_ADC_DMA_Start_e;

typedef enum{

	ADC_DMA_STOP_FAILED = 0,
	ADC_DMA_STOP_OK

}status_ADC_DMA_Stop_e;

typedef enum{

	ADC_INIT_FAILED = 0,
	ADC_INIT_OK

}status_ADC_Init_e;

typedef enum {

	MAIN_INIT_FAILED = 0,
	MAIN_INIT_OK

}status_ADC_DMA_Main_Init_e;

status_ADC_DMA_Main_Init_e ADC_and_DMA_init_and_calibrate(void);
status_ADC_DMA_Start_e ADC_start_in_DMA_mode(void);
status_ADC_DMA_Stop_e ADC_stop(void);

#endif /* HARDWARE_DRIVERS_INC_ADC_DMA_DRIVER_H_ */
