/*
 * timer_driver.c
 *
 *  Created on: May 13, 2024
 *      Author: Kemal
 */

#include "timer_driver.h"
#include "uart_driver.h"
#include "adc_dma_driver.h"

uint8_t first_unwanted_interrupt_bypassed = 0;
timer_range_ms_or_us_e timer_range;

extern uint8_t ready_to_read_new_data;
extern volatile float temp;
char uart_buffer_data[60];

//uint16_t measurements_done_count = 0;
//uint16_t timer_interrupt_triggered_count = 0;

void TIM6_IRQHandler(void)
{
	TIM6->SR &= ~(1 << 0);

//	timer_interrupt_triggered_count++; // for debugging

	if (first_unwanted_interrupt_bypassed == 0)
	{
		first_unwanted_interrupt_bypassed = 1;
	}

	else
	{
		  if (ready_to_read_new_data == 0)
		  {
			  sprintf((char*)uart_buffer_data, "Temp: %.2f *C\r\n",temp);
			  UART_send_byte_array(uart_buffer_data, strlen((char*)uart_buffer_data));
			  ready_to_read_new_data = 1;
			  ADC_start_in_DMA_mode();
		  }
	}
}

void timer_basic_while_delay(uint16_t delay)
{
	TIM6->CNT = 0;
	while ((TIM6->CNT) < delay);
}

void timer_reinit_with_new_settings(timer_range_ms_or_us_e timer_range, uint16_t stoptime)
{

	TIM6->CR1 &= ~(1<<0); // disable the timer temporarily

	if (timer_range == MILLISECONDS)
	{
		TIM6->PSC = 48000-1;
	}
	else if (timer_range == MICROSECONDS)
	{
		TIM6->PSC = 48-1;
	}

	TIM6->ARR = (stoptime-1);

	TIM6->EGR |= (1<<0); // restart the timer with new settings
	TIM6->SR &= ~(1 << 0);

	TIM6->CR1 |= (1<<0); // enable the timer again
	first_unwanted_interrupt_bypassed = 0;
}

void timer_reset_counter(void)
{
	TIM6->CNT = 0;
}

uint16_t timer_get_counter_value(void)
{
	return (TIM6->CNT);
}

void timer_init(timer_range_ms_or_us_e timer_range, uint16_t arr)
{
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);
//	__HAL_RCC_TIM6_CLK_ENABLE();

	if (timer_range == MICROSECONDS)
	{
		TIM6->PSC = 48-1;
	}
	else if (timer_range == MILLISECONDS)
	{
		TIM6->PSC = 48000-1;
	}

	TIM6->ARR = (arr)-1;

	timer_enable_interrupt();
}


void timer_enable(void)
{
	TIM6->CR1 |= (1<<0);
	first_unwanted_interrupt_bypassed = 0;
}

void timer_disable(void)
{

	TIM6->CR1 &= ~(1<<0);
//	first_unwanted_interrupt_bypassed = 0;

}

void timer_enable_interrupt(void)
{
	TIM6->DIER |= (1<<0);
	NVIC_EnableIRQ(TIM6_IRQn);
	NVIC_SetPriority(TIM6_IRQn, 1);
}

void timer_disable_interrupt(void)
{
	TIM6->DIER &= ~(1<<0);
	NVIC_DisableIRQ(TIM6_IRQn);
}
