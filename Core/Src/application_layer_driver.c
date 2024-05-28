/*
 * application_layer_driver.c
 *
 *  Created on: May 28, 2024
 *      Author: Kemal
 */

#include "application_layer_driver.h"

  char uart_buffer_status[120];

  status_ADC_DMA_Main_Init_e main_init_status;
  status_ADC_DMA_Start_e adc_dma_start_status;
  status_All_Init_e status_all_init;
  uint16_t measurement_period_in_ms_2 = 0;


status_All_Init_e applayer_main_init_function(uint16_t measurement_period_in_ms, uint32_t uart_baudrate)
{
	  if ((UART_init(uart_baudrate)) == HAL_OK)
	  {
		  timer_init(MILLISECONDS, measurement_period_in_ms);
		  measurement_period_in_ms_2 = measurement_period_in_ms;

		  main_init_status = ADC_and_DMA_init_and_calibrate();

		  if (main_init_status == MAIN_INIT_OK)
		  {
			  status_all_init = ALL_INIT_OK;
			  return status_all_init;
		  }
		  else
		  {
			  status_all_init = ALL_INIT_FAILED;
			  return status_all_init;
		  }
	  }
	  else
	  {
		  status_all_init = ALL_INIT_FAILED;
		  return status_all_init;
	  }

}

status_Start_Reading_e applayer_start_reading(void)
{

	if (status_all_init == ALL_INIT_OK)
	{

		adc_dma_start_status = ADC_start_in_DMA_mode();

		if (adc_dma_start_status == ADC_DMA_START_OK) {
			sprintf(uart_buffer_status, "\nLM35, timer and UART init and start process are successful, starting to read temperature in every %u ms.\n\r",measurement_period_in_ms_2);
			UART_send_byte_array(uart_buffer_status, strlen(uart_buffer_status));
			timer_enable();
			return STARTED_OK;
		}
		else
		{

			sprintf(uart_buffer_status, "\nInit ok but couldn't start ADC in DMA mode!!\n\r");
			UART_send_byte_array(uart_buffer_status, strlen(uart_buffer_status));
			return START_FAILED;
		}

	}
	else
	{
		sprintf(uart_buffer_status, "\nCan't start to read because init is failed!\n\r");
		UART_send_byte_array(uart_buffer_status, strlen(uart_buffer_status));
		return START_FAILED;
	}
}
