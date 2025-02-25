#include "initprocess.h"
#include <stdint.h>
#include <stdio.h>
#include "variables.h"
#include "7SEG.h"
#include "CLCD.h"

// Init_Peripherals 함수에 TIM_HandleTypeDef 포인터 매개변수 추가
void Init_Peripherals(TIM_HandleTypeDef *htim) {
    Init_LEDs();
    Init_TIMERS(htim);  // 포인터를 직접 전달
    Init_Display();
}

void Init_LEDs(void) {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

// Init_TIMERS 함수에 TIM_HandleTypeDef 포인터 매개변수 추가
void Init_TIMERS(TIM_HandleTypeDef *htim) {
    if (htim != NULL) {
        HAL_TIM_Base_Start_IT(htim);  // 포인터를 직접 사용하여 TIMER 인터럽트 시작
    }
}

void Init_Display(void) {
    CLCD_GPIO_Init();  // LCD GPIO 초기화
    CLCD_Init();       // LCD 초기화
    _7SEG_GPIO_Init(); // 7세그먼트 초기화
}

void Init_button_operation() {
	if (mode_changed == true) {
		CLCD_Clear();
		item_select = 0;
		item_select2 = 0;
		lap_time_click = 0;
		mode_changed = false;

		item_select3 = 0;
		alarm_select = 1;
	}

	if (Press_Time == 0) { // 안 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		Press_Mode = PRESS_NOT;
	}

	else if (0 < Press_Time && Press_Time < 700) { // 짧게 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		Press_Mode = PRESS_SHORT;
	}

	else if (700 <= Press_Time && Press_Time < 2500) { // 중간 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		Press_Mode = PRESS_MID;
	}

	else if (2500 <= Press_Time) { // 길게 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		Press_Mode = PRESS_LONG;
	}

}
