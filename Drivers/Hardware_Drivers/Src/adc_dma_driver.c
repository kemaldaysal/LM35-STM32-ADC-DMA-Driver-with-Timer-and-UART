/*
 * adc_dma_driver.c
 *
 *  Created on: May 28, 2024
 *      Author: Kemal
 */

#include "adc_dma_driver.h"

ADC_HandleTypeDef hadc;
DMA_HandleTypeDef hdma_adc;

uint32_t adcval;
volatile float temp;
uint8_t ready_to_read_new_data = 1;

static void MX_DMA_Init(void);
static status_ADC_Init_e MX_ADC_Init(void);
extern void Error_Handler(void);

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (ready_to_read_new_data == 1)
	{
		ADC_stop();
		ready_to_read_new_data = 0;
		temp = ((adcval * ADC_REF_VOLTAGE) / ADC_RESOLUTION) * TEN_MV_TO_ONE_V_CONVERSION_COEFFICIENT;
	}
}

status_ADC_DMA_Main_Init_e ADC_and_DMA_init_and_calibrate(void)
{

	status_ADC_Init_e adc_init_status;

	MX_DMA_Init();
	adc_init_status = MX_ADC_Init();

	if (adc_init_status == ADC_INIT_OK)
	{
		if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
		{
			return MAIN_INIT_FAILED;
		}

		else
		{
			return MAIN_INIT_OK;
		}
	}
	else
	{

		return MAIN_INIT_FAILED;
	}

}


status_ADC_DMA_Start_e ADC_start_in_DMA_mode(void)
{
	  if (HAL_ADC_Start_DMA(&hadc, &adcval, 1) == HAL_OK)
	  {
		  return ADC_DMA_START_OK;
	  }

	  else
	  {
		  return ADC_DMA_START_FAILED;
	  }
}

status_ADC_DMA_Stop_e ADC_stop(void)
{
	  if (HAL_ADC_Stop_DMA(&hadc) == HAL_OK)
	  {
		  return ADC_DMA_STOP_OK;
	  }

	  else
	  {
		  return ADC_DMA_STOP_FAILED;
	  }
}


/**
  * @brief ADC Initialization Function
  * @param None
  * @retval None
  */
static status_ADC_Init_e MX_ADC_Init(void)
{

  /* USER CODE BEGIN ADC_Init 0 */

  /* USER CODE END ADC_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC_Init 1 */

  /* USER CODE END ADC_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_SETTING;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = ADC_CONTINOUS_CONV_MODE_SETTING;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = ENABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  if (HAL_ADC_Init(&hadc) != HAL_OK)
  {
	  return ADC_INIT_FAILED;
//    Error_Handler();
  }

  /** Configure for the selected ADC regular channel to be converted.
  */
  sConfig.Channel = ADC_CHANNEL_NUMBER;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLING_TIME_SETTING;
  if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
  {
	return ADC_INIT_FAILED;
    Error_Handler();
  }
  /* USER CODE BEGIN ADC_Init 2 */

  return ADC_INIT_OK;

  /* USER CODE END ADC_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}
