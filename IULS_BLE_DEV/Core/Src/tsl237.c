/* tsl237.c
 * Description: Function to capture the period of the tsl237 light sensor
 *
 * Author: Bryce Himebaugh and Brian Schwantes
 * Contact: bhimebau@indiana.edu | bschwant@iu.edu
 *
 */

#include "main.h"
#include <string.h>
#include <stdio.h>
#include "tsl237.h"

extern uint32_t period;
extern TIM_HandleTypeDef htim2;

void tsl237_command(char *arguments) {

  // HAL_TIM_Base_Start_IT(&htim2);   // Turn on the IRQ in the timer
  // HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1); // Turn on the IRQ for CH1 input capture
  // HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1); // Turn on the IRQ for CH1 input capture
  float clock_period;
  float sensor_period;
  float sensor_frequency;
  
  if (arguments) {
    printf("NOK\n\r");
  }
  else {
    clock_period = SystemCoreClock;
    clock_period = 1/clock_period;
    sensor_period = clock_period * (float) period;
    sensor_frequency = 1/sensor_period;
    printf("%0.3f hz\n\r", sensor_frequency);
    printf("OK\n\r");
  }

  // HAL_TIM_IC_Stop_IT(&htim2, TIM_CHANNEL_1);
}