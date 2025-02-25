#ifndef __INIT_CLOCK_H
#define __INIT_CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"  // htim7을 가져오기 위해 필요

void Init_Peripherals(TIM_HandleTypeDef *htim);
void Init_LEDs(void);
void Init_TIMERS(TIM_HandleTypeDef *htim);
void Init_Display(void);
void Init_button_operation();

#ifdef __cplusplus
}
#endif

#endif
