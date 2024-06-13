/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include <stdint.h>
#include <stdio.h>

#include "7SEG.h"
#include "CLCD.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
//스위치 채터링 방지 변수
bool sw1 = false;
bool sw2 = false;
bool sw3 = false;
bool sw4 = false;

bool sw1_debounced = false;
bool sw2_debounced = false;
bool sw3_debounced2 = false;
bool sw3_debounced = false;
bool sw4_debounced = false;

//총 실행시간 변수
uint64_t time = 0;
uint8_t str[20];

//버튼 누른 시간 변수
uint16_t Press_Time = 0;
uint8_t Press_Mode = 0;

//모드 변수
uint8_t mode = 1;
bool mode_changed = false;

//CLOCK 변수
uint64_t clock_time = 0;
uint8_t hour = 23;
uint8_t minute = 59;
uint8_t second = 50;
uint16_t millisecond = 0;
bool changed = true;
bool buzzer = true;

uint16_t year = 2024;
uint8_t month = 12;
uint8_t day = 31;

bool clock_setmode = false;
uint8_t item_select = 0;

//ALARM 변수
bool alarm_setmode = false;
bool alarm_changed[6] = { true, false, false, false, false, false };
uint8_t alarm_select = 1;
uint8_t alarm_hour[6] = { 0, };
uint8_t alarm_minute[6] = { 0, };
uint8_t item_select3 = 0;
bool alarm_ring = false;

//STOPWATCH 변수
uint64_t stopwatch_time = 0;
uint64_t lap_time[10] = { 0, };
uint8_t lap_time_index = 0;
uint8_t lap_time_click = 0;
bool stopwatch_running = false;  // 스톱워치 실행 여부를 나타내는 플래그

//TIMER 변수
uint64_t timer_time = 0;
uint64_t timer_time_tmp = 0;
bool timer_setmode = false;
uint8_t item_select2 = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM2_Init(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM7_Init();
	MX_TIM2_Init();

	/* Initialize interrupts */
	MX_NVIC_Init();
	/* USER CODE BEGIN 2 */

	//led 꺼놓기
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);

	HAL_TIM_Base_Start_IT(&htim7); //TIMER7 시작 code
	CLCD_GPIO_Init(); //LCD GPIO 초기화 code
	CLCD_Init(); //LCD 초기화면 초기화 code
	_7SEG_GPIO_Init(); //7SEG 초기화 code

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		if (mode > 4) {
			mode = 1;
			if (mode == 1) {
				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
			}
		}

		if (alarm_changed[1] && !alarm_setmode) {
			if (alarm_hour[1] == hour && alarm_minute[1] == minute) {
				alarm_changed[1] = !alarm_changed[1];
				alarm_ring = true;
			}
		}

		else if (alarm_changed[2] && !alarm_setmode) {
			if (alarm_hour[2] == hour && alarm_minute[2] == minute) {
				alarm_changed[2] = !alarm_changed[2];
				alarm_ring = true;
			}
		}

		else if (alarm_changed[3] && !alarm_setmode) {
			if (alarm_hour[3] == hour && alarm_minute[3] == minute) {
				alarm_changed[3] = !alarm_changed[3];
				alarm_ring = true;
			}
		}

		else if (alarm_changed[4] && !alarm_setmode) {
			if (alarm_hour[4] == hour && alarm_minute[4] == minute) {
				alarm_changed[4] = !alarm_changed[4];
				alarm_ring = true;
			}
		}

		else if (alarm_changed[5] && !alarm_setmode) {
			if (alarm_hour[5] == hour && alarm_minute[5] == minute) {
				alarm_changed[5] = !alarm_changed[5];
				alarm_ring = true;
			}
		}

		switch (mode) {

		/*case 1:
		 Init_basic_operation();
		 Init_display_operation();
		 Init_button_operation();
		 break;*/

		case 1:
			Clock_basic_operation();
			Clock_display_operation();
			Clock_button_operation();
			Init_button_operation();
			break;
		case 2:
			Alarm_basic_operation();
			Alarm_display_operation();
			Alarm_button_operation();
			Init_button_operation();
			break;
		case 3:
			Stopwatch_basic_operation();
			Stopwatch_display_operation();
			Stopwatch_button_operation();
			Init_button_operation();
			break;
		case 4:
			Timer_basic_operation();
			Timer_display_operation();
			Timer_button_operation();
			Init_button_operation();
			break;
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 4;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief NVIC Configuration.
 * @retval None
 */
static void MX_NVIC_Init(void) {
	/* TIM7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(TIM7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	/* EXTI3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	/* EXTI4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI4_IRQn);
	/* EXTI15_10_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 168 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 500;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_ENABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */
	HAL_TIM_MspPostInit(&htim2);

}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void) {

	/* USER CODE BEGIN TIM7_Init 0 */

	/* USER CODE END TIM7_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM7_Init 1 */

	/* USER CODE END TIM7_Init 1 */
	htim7.Instance = TIM7;
	htim7.Init.Prescaler = 84 - 1;
	htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim7.Init.Period = 1000 - 1;
	htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM7_Init 2 */

	/* USER CODE END TIM7_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7
					| GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pins : PE2 PE4 PE5 PE6
	 PE7 PE0 PE1 */
	GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6
			| GPIO_PIN_7 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : PE3 */
	GPIO_InitStruct.Pin = GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pin : PC15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PB0 PB5 */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PD10 PD4 */
	GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pins : PD12 PD13 PD14 */
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : PC6 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void Init_basic_operation() {
	sprintf(str, "%8d", time / 1000); //1초 단위 LCD 출력
	CLCD_Puts(8, 0, str);

//0.1, 0.01초 단위 7SEG 출력
	if (time % 1000 / 100 > 5) { // 0.5초간 7SEG 깜박임
		_7SEG_SetNumber(DGT1, time % 1000 / 100, OFF);
	} else {
		_7SEG_SetNumber(DGT1, time % 1000 / 100, ON);
	}
	_7SEG_SetNumber(DGT2, time % 100 / 10, OFF);

//버튼 누른 시간 LCD 출력
	sprintf(str, "%4d", Press_Time);
	CLCD_Puts(0, 1, str);

	sprintf(str, "%4d", year);
	CLCD_Puts(0, 0, str);

//버튼 PRESS 상태 출력
	if (Press_Time == 0) {
		sprintf(str, "%5s", "no");
		CLCD_Puts(10, 1, str);
	} else if (0 < Press_Time && Press_Time < 700) {
		sprintf(str, "%5s", "short");
		CLCD_Puts(10, 1, str);
	} else if (700 <= Press_Time && Press_Time < 2500) {
		sprintf(str, "%5s", "mid");
		CLCD_Puts(10, 1, str);
	} else if (2500 <= Press_Time) {
		sprintf(str, "%5s", "long");
		CLCD_Puts(10, 1, str);
	}
}
void Init_display_operation() {
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
		Press_Mode = 0;
	}

	else if (0 < Press_Time && Press_Time < 700) { // 짧게 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		Press_Mode = 1;
	}

	else if (700 <= Press_Time && Press_Time < 2500) { // 중간 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
		Press_Mode = 2;
	}

	else if (2500 <= Press_Time) { // 길게 누른 상태
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		Press_Mode = 3;
	}

	if (sw2_debounced) {

		sw2_debounced = false;
	}

}

Alarm_basic_operation() {
	if (alarm_hour[alarm_select] >= 24) {
		alarm_hour[alarm_select] = 0;
	}

	if (alarm_minute[alarm_select] >= 60) {
		alarm_minute[alarm_select] = 0;
		alarm_hour[alarm_select]++;
	}

}
Alarm_display_operation() {

	uint8_t displayHour = (int) alarm_hour[alarm_select] % 12;
	if (displayHour == 0) {
		displayHour = 12; // 0시를 12시로 변환
	}

	if (alarm_setmode) {
		sprintf(str, "SET");
		CLCD_Puts(0, 1, str);

		if (clock_time / 100 > 5) {
			if (item_select3 == 0) {
				sprintf(str, "%02d", alarm_minute[alarm_select]);
				CLCD_Puts(14, 1, str);
				sprintf(str, " ");
				CLCD_Puts(8, 0, str);
			}

			else if (item_select3 == 1) {
				sprintf(str, "%d", alarm_select);
				CLCD_Puts(8, 0, str);
				sprintf(str, "   ");
				CLCD_Puts(13, 0, str);
			}

			else if (item_select3 == 2) {
				if (alarm_changed[alarm_select]) {
					sprintf(str, " ON");
				} else {
					sprintf(str, "OFF");
				}
				CLCD_Puts(13, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 1, str);
			}

			else if (item_select3 == 3) {
				sprintf(str, "%02d", displayHour);
				CLCD_Puts(11, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 1, str);
			}
		}

		else {
			sprintf(str, "ALARM  #%d", alarm_select);
			CLCD_Puts(0, 0, str);

			sprintf(str, "%s  ", (alarm_hour[alarm_select] < 12) ? "AM" : "PM");
			CLCD_Puts(7, 1, str);

			sprintf(str, "%02d:%02d", displayHour, alarm_minute[alarm_select]);
			CLCD_Puts(11, 1, str);

			if (alarm_changed[alarm_select]) {
				sprintf(str, " ON");
				CLCD_Puts(13, 0, str);
			}

			else {
				sprintf(str, "OFF");
				CLCD_Puts(13, 0, str);
			}

		}
	}

	else {
		sprintf(str, "   ");
		CLCD_Puts(0, 1, str);

		sprintf(str, "ALARM  #%d", alarm_select);
		CLCD_Puts(0, 0, str);

		sprintf(str, "%s  ", (alarm_hour[alarm_select] < 12) ? "AM" : "PM");
		CLCD_Puts(7, 1, str);

		sprintf(str, "%02d:%02d", alarm_hour[alarm_select],
				alarm_minute[alarm_select]);
		CLCD_Puts(11, 1, str);

		if (alarm_changed[alarm_select]) {
			sprintf(str, " ON");
			CLCD_Puts(13, 0, str);
		}

		else {
			sprintf(str, "OFF");
			CLCD_Puts(13, 0, str);
		}
	}

	if (clock_time / 100 > 5) {
		_7SEG_SetNumber(DGT2, second % 10, ON);
	} else {
		_7SEG_SetNumber(DGT2, second % 10, OFF);
	}
	_7SEG_SetNumber(DGT1, second / 10, OFF);

}
Alarm_button_operation() {
	if (sw1_debounced) {
		if (Press_Mode >= 2) {
			alarm_setmode = !alarm_setmode; // Toggle timer setting mode
			Press_Mode = 0;
		}
		sw1_debounced = false;
	}

	if (alarm_setmode) {
		if (sw2_debounced) {
			updateTime5();
			sw2_debounced = false;
		}
		if (sw2 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime5();
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime5();
					time = 0;
				}
			}
		}

		if (sw3_debounced) {
			updateTime6();
			sw3_debounced = false;
		}
		if (sw3 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime6();
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime6();
					time = 0;
				}
			}
		}
	}

	else {
		if (sw2_debounced) {
			alarm_select++;
			if (alarm_select >= 6) {
				alarm_select = 1;
			}
			sw2_debounced = false;
		}
	}
}
void updateTime5() {
	switch (item_select3) {
	case 0:
		alarm_select++;
		if (alarm_select >= 6) {
			alarm_select = 1;
		}
		break;
	case 1:
		alarm_changed[alarm_select] = !alarm_changed[alarm_select];
		if (alarm_changed[alarm_select]) {
			alarm_hour[alarm_select] = hour;
			alarm_minute[alarm_select] = minute;
		}

		break;
	case 2:
		if (alarm_hour[alarm_select] >= 23) {
			alarm_hour[alarm_select] = 0;
		}

		else {
			alarm_hour[alarm_select]++;
		}
		break;
	case 3:
		if (alarm_minute[alarm_select] >= 59) {
			alarm_minute[alarm_select] = 0;
		}

		else {
			alarm_minute[alarm_select]++;
		}
		break;
	}
}

void updateTime6() {
	switch (item_select3) {
	case 0:
		alarm_select--;
		if (alarm_select <= 0) {
			alarm_select = 5;
		}
		break;
	case 1:
		alarm_changed[alarm_select] = !alarm_changed[alarm_select];
		if (alarm_changed[alarm_select]) {
			alarm_hour[alarm_select] = hour;
			alarm_minute[alarm_select] = minute;
		}
		break;
	case 2:
		if (alarm_hour[alarm_select] <= 0) {
			alarm_hour[alarm_select] = 23;
		}

		else {
			alarm_hour[alarm_select]--;
		}
		break;
	case 3:
		if (alarm_minute[alarm_select] <= 0) {
			alarm_minute[alarm_select] = 59;
		}

		else {
			alarm_minute[alarm_select]--;
		}
		break;
	}
}
void Clock_basic_operation() {

	if (month >= 13) {
		month = 1;
		year++;
	}

	if ((month == 4 || month == 6 || month == 9 || month == 11) && day >= 31) {
		month++;
		day = 1;
	} else if ((month == 1 || month == 3 || month == 5 || month == 7
			|| month == 8 || month == 10 || month == 12) && day >= 32) {
		if (month >= 12) {
			year++;
			month = 1;
		} else {
			month++;
		}
		day = 1;
	} else if (month == 2) {
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) { // 윤년
			if (day >= 30) {
				month++;
				day = 1;
			}
		} else { // 평년
			if (day >= 29) {
				month++;
				day = 1;
			}
		}
	}
}

void Clock_display_operation() {
	const char *period;
	uint8_t displayHour = (int) hour % 12;
	if (displayHour == 0) {
		displayHour = 12; // 0시를 12시로 변환
	}

	if (clock_setmode) {
		if (clock_time / 100 > 5) {

			if (item_select == 0) {
				sprintf(str, "%02d", (int) second);
				CLCD_Puts(14, 1, str);
				sprintf(str, "    ");
				CLCD_Puts(6, 0, str);
			}

			else if (item_select == 1) {
				sprintf(str, "%04d", (int) year);
				CLCD_Puts(6, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 0, str);
			}

			else if (item_select == 2) {
				sprintf(str, "%02d", (int) month);
				CLCD_Puts(11, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 0, str);
			}

			else if (item_select == 3) {
				sprintf(str, "%02d", (int) day);
				CLCD_Puts(14, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(8, 1, str);
			}

			else if (item_select == 4) {
				sprintf(str, "%02d", displayHour);
				CLCD_Puts(8, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 1, str);
			}

			else if (item_select == 5) {
				sprintf(str, "%02d", (int) minute);
				CLCD_Puts(11, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 1, str);
			}
			_7SEG_SetNumber(DGT2, second % 10, ON);
		}

		else {
			sprintf(str, "%04d.%02d.%02d", (int) year, (int) month, (int) day);
			CLCD_Puts(6, 0, str);
			period = (hour < 12) ? "AM" : "PM";
			sprintf(str, "SET  %s", period);
			CLCD_Puts(0, 1, str);
			sprintf(str, "%02d:%02d:%02d", displayHour, (int) minute,
					(int) second);
			CLCD_Puts(8, 1, str);
			_7SEG_SetNumber(DGT2, second % 10, OFF);

		}
		_7SEG_SetNumber(DGT1, second / 10, OFF);
	}

	else {

		if (clock_time / 100 > 5) {
			sprintf(str, " ");
			CLCD_Puts(13, 1, str);
		} else {
			sprintf(str, ":");
			CLCD_Puts(13, 1, str);
		}

		if (clock_time / 100 > 5) {
			_7SEG_SetNumber(DGT2, second % 10, ON);
		} else {
			_7SEG_SetNumber(DGT2, second % 10, OFF);
		}
		_7SEG_SetNumber(DGT1, second / 10, OFF);

		if (buzzer == false) {
			sprintf(str, "BZ OFF ");
		} else {
			sprintf(str, "BZ ON  ");
		}
		CLCD_Puts(0, 1, str);

		sprintf(str, "%04d.%02d.%02d", (int) year, (int) month, (int) day);
		CLCD_Puts(6, 0, str);

		sprintf(str, "TIME");
		CLCD_Puts(0, 0, str);

		if (changed == false) { // NOT AM/PM
			sprintf(str, "   %02d %02d", (int) hour, (int) minute);
			CLCD_Puts(8, 1, str);
		}

		else { // AM/PM
			period = (hour < 12) ? "AM" : "PM";
			sprintf(str, "%s %02d %02d", period, displayHour, (int) minute);
			CLCD_Puts(8, 1, str);
		}
	}

}
void Clock_button_operation() {
	if (sw1_debounced) {
		if (Press_Mode >= 2) {
			clock_setmode = !clock_setmode; // Toggle timer setting mode
			Press_Mode = 0;
		}
		sw1_debounced = false;
	}

	if (clock_setmode) {
		if (sw2_debounced == true) {
			updateTime();
			sw2_debounced = false;
		}

		if (sw2 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime();
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime();
					time = 0;
				}
			}
		}

		if (sw3_debounced == true) {
			updateTime2(item_select);
			sw3_debounced = false;
		}

		if (sw3 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime2();
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime2();
					time = 0;
				}
			}
		}
	} else {
		if (sw4_debounced == true) {
			changed = !changed;
			sw4_debounced = false;
		}

		if (sw2_debounced == true && !clock_setmode) {
			buzzer = !buzzer;
			sw2_debounced = false;
		}

		if (sw3_debounced) {
			sw3_debounced = false;
		}

	}

}

bool isLeapYear(uint16_t year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void updateTime() {
	switch (item_select) {
	case 0:
		if (year == 9999) {
			year = 0;
		} else {
			year++;
		}
		break;
	case 1:
		if (month == 12) {
			month = 1;
		} else {
			month++;
		}
		break;
	case 2:
		if (isLeapYear(year) && day == 29 && month == 2) {
			day = 1;
		}

		else if (!isLeapYear(year) && day == 28 && month == 2) {
			day = 1;
		}

		else if (day == 31
				&& (month == 1 || month == 3 || month == 5 || month == 7
						|| month == 8 || month == 10 || month == 12)) {
			day = 1;
		}

		else if (day == 30
				&& (month == 4 || month == 6 || month == 9 || month == 11)) {
			day = 1;
		}

		else {
			day++;
		}
		break;
	case 3:
		if (hour == 23) {
			hour = 0;
		} else {
			hour++;
		}
		break;
	case 4:
		if (minute == 59) {
			minute = 0;
		} else {
			minute++;
		}
		break;
	case 5:
		if (second == 59) {
			second = 0;
		} else {
			second++;
		}
		break;
	}

}

void updateTime2() {

	switch (item_select) {
	case 0:
		if (year == 0) {
			year = 0;
		} else {
			year--;
		}
		break;
	case 1:
		if (month == 1) {
			month = 12;
		} else {
			month--;
		}
		break;
	case 2:
		if (isLeapYear(year) && day == 1 && month == 2) {
			day = 29;
		}

		else if (!isLeapYear(year) && day == 1 && month == 2) {
			day = 28;
		}

		else if (day == 1
				&& (month == 1 || month == 3 || month == 5 || month == 7
						|| month == 8 || month == 10 || month == 12)) {
			day = 31;
		}

		else if (day == 1
				&& (month == 4 || month == 6 || month == 9 || month == 11)) {
			day = 30;
		}

		else {
			day--;
		}
		break;
	case 3:
		if (hour == 0) {
			hour = 23;
		} else {
			hour--;
		}
		break;
	case 4:
		if (minute == 0) {
			minute = 59;
		} else {
			minute--;
		}
		break;
	case 5:
		if (second == 0) {
			second = 59;
		} else {
			second--;
		}
		break;
	}

}

void Stopwatch_basic_operation() {

}
void Stopwatch_display_operation() {
	sprintf(str, "%4s%02d:%02d:%02d.%03d", "STW ",
			(int) (stopwatch_time / 1000) / 3600,
			(int) ((stopwatch_time / 1000) / 60) % 60,
			(int) (stopwatch_time / 1000) % 60, (int) stopwatch_time % 1000);
	CLCD_Puts(0, 0, str);

//0.1, 0.01초 단위 7SEG 출력
	if (stopwatch_time % 1000 / 100 > 4) { // 0.5초간 7SEG 깜박임
		_7SEG_SetNumber(DGT1, stopwatch_time / 1000 % 10, OFF);
	} else {
		_7SEG_SetNumber(DGT1, stopwatch_time / 1000 % 10, ON);
	}
	_7SEG_SetNumber(DGT2, stopwatch_time % 1000 / 100, OFF);
}
void Stopwatch_button_operation() {
	if (sw2_debounced == true) {
		if (lap_time_index == 0) {
			sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
			CLCD_Puts(0, 1, str);
		}
		stopwatch_running = !stopwatch_running;
		sw2_debounced = false;
	}

	if (stopwatch_running == false && sw3_debounced2 == true) {
		stopwatch_time = 0;
		lap_time_index = 0;
		sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw3_debounced2 = false;
	}

	else if (stopwatch_running == true && sw3_debounced2 == true) {
		lap_time_click = 0;

		if (lap_time_index < 9) {
			lap_time_index++;
			lap_time[lap_time_index] = stopwatch_time;
			sprintf(str, "LP%d %02d:%02d:%02d.%03d", lap_time_index,
					(int) (stopwatch_time / 1000) / 3600,
					(int) ((stopwatch_time / 1000) / 60) % 60,
					(int) (stopwatch_time / 1000) % 60,
					(int) stopwatch_time % 1000);
			CLCD_Puts(0, 1, str);

		}

		else {
			sprintf(str, "%16s", "LAP FULL(9/9)   "); // 스톱워치 시간 1ms 단위 LCD 출력
			CLCD_Puts(0, 1, str);
		}
		sw3_debounced2 = false;
	}

	if (lap_time_index != 0 && sw4_debounced == true) {
		lap_time_click++;
		if (lap_time_click <= lap_time_index) {
			sprintf(str, "%1d/%1d %02d:%02d:%02d.%03d", lap_time_click,
					lap_time_index,
					(int) (lap_time[lap_time_click] / 1000) / 3600,
					(int) ((lap_time[lap_time_click] / 1000) / 60) % 60,
					(int) (lap_time[lap_time_click] / 1000) % 60,
					(int) lap_time[lap_time_click] % 1000);
			CLCD_Puts(0, 1, str);
			if (lap_time_click == lap_time_index) {
				lap_time_click = 0;
			}
		}

		sw4_debounced = false;
	}

	else if (lap_time_index == 0 && sw4_debounced == true) {
		sprintf(str, "%16s", "NO LAP          "); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw4_debounced = false;
	}

}

bool timer_start = false;

void Timer_button_operation() {
// Enter timer setting mode on long press of button 1 (sw1)
	if (sw1_debounced) {
		if (Press_Mode >= 2) {
			CLCD_Clear();
			timer_setmode = !timer_setmode; // Toggle timer setting mode
			Press_Mode = 0;
		}
		sw1_debounced = false;
	}

// Setting mode adjustments
	if (timer_setmode) {
		// Adjusting the timer settings using buttons (assuming sw2 to increase hours, sw3 to increase minutes, sw4 to increase seconds)
		if (sw2_debounced) {
			updateTime3(item_select2);
			sw2_debounced = false;
		}

		if (sw2 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime3(item_select2);
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime3(item_select2);
					time = 0;
				}
			}
		}

		if (sw3_debounced) {
			updateTime4(item_select2);
			sw3_debounced = false;
		}

		if (sw3 == true) {
			if (Press_Mode == 2) {
				if (time >= 500) {
					updateTime4(item_select2);
					time = 0;
				}
			} else if (Press_Mode == 3) {
				if (time >= 200) {
					updateTime4(item_select2);
					time = 0;
				}
			}
		}
		timer_time = timer_time_tmp;

	}

	else {
		// Starting/Stopping the timer using button 2 (sw2)
		if (sw2_debounced) {
			if (timer_time != 0) {
				timer_start = !timer_start;
			}
			sw2_debounced = false;
		}

		if (sw3_debounced) {
			if (!timer_start) {
				timer_time = 0;
				timer_time_tmp = 0;
			}
			sw3_debounced = false;
		}
		if (sw4_debounced) {
			sw4_debounced = false;
		}
	}

}

void updateTime3() {
	switch (item_select2) {
	case 0:
		timer_time_tmp += 3600000; // Increase hours
		break;
	case 1:
		timer_time_tmp += 60000;
		break;
	case 2:
		timer_time_tmp += 1000;
		break;
	}
}
void updateTime4() {
	switch (item_select2) {
	case 0:
		if (timer_time_tmp >= 3600000) {
			timer_time_tmp -= 3600000; // Increase hours
		}
		break;
	case 1:
		if (timer_time_tmp >= 60000) {
			timer_time_tmp -= 60000; // Increase hours
		}
		break;
	case 2:
		if (timer_time_tmp >= 1000) {
			timer_time_tmp -= 1000; // Increase hours
		}
		break;
	}
}
void Timer_basic_operation() {
// If the timer is running, decrement the timer

	if (!timer_start) {
		if (timer_setmode) {
			sprintf(str, "SET");
			CLCD_Puts(0, 1, str);
			sprintf(str, "%16s", "TIMER           ");
			CLCD_Puts(0, 0, str);
		} else {
			sprintf(str, "%16s", "TIMER      START");
			CLCD_Puts(0, 0, str);
			if (timer_time_tmp == 0) {
				timer_time_tmp = timer_time;
			}
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
		}
	}

	else {
		if (timer_time_tmp == 0) {
			sprintf(str, "%16s", "TIMER   FINISHED");
			CLCD_Puts(0, 0, str);
		}

		else if (timer_time_tmp != 0) {
			sprintf(str, "%16s", "TIMER    RUNNING");
			CLCD_Puts(0, 0, str);
		}
	}

}

void Timer_display_operation() {
// Display the timer settings or countdown based on mode
	if (timer_setmode) {

		if (clock_time / 100 > 5) {
			if (item_select2 == 0) {
				sprintf(str, "%02d", (int) (timer_time_tmp / 1000) % 60);
				CLCD_Puts(14, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(8, 1, str);
			}

			else if (item_select2 == 1) {
				sprintf(str, "%02d", (int) (timer_time_tmp / 1000) / 3600);
				CLCD_Puts(8, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 1, str);
			}

			else if (item_select2 == 2) {
				sprintf(str, "%02d", (int) ((timer_time_tmp / 1000) / 60) % 60);
				CLCD_Puts(11, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 1, str);
			}
		}

		else {
			sprintf(str, "%02d:%02d:%02d", (int) (timer_time_tmp / 1000) / 3600,
					(int) ((timer_time_tmp / 1000) / 60) % 60,
					(int) (timer_time_tmp / 1000) % 60,
					(int) timer_time_tmp % 1000);
			CLCD_Puts(8, 1, str);

		}
	}

	else {
		sprintf(str, "    %02d:%02d:%02d.%03d",
				(int) (timer_time_tmp / 1000) / 3600,
				(int) ((timer_time_tmp / 1000) / 60) % 60,
				(int) (timer_time_tmp / 1000) % 60,
				(int) timer_time_tmp % 1000);
		CLCD_Puts(0, 1, str);
	}

//0.1, 0.01초 단위 7SEG 출력

	if (timer_time_tmp % 1000 / 100 > 5) { // 0.5초간 7SEG 깜박임
		_7SEG_SetNumber(DGT1, timer_time_tmp % 1000 / 100, OFF);
	} else {
		_7SEG_SetNumber(DGT1, timer_time_tmp % 1000 / 100, ON);
	}
	_7SEG_SetNumber(DGT2, timer_time_tmp % 100 / 10, OFF);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { //타이머 인터럽트 호출
	if (htim->Instance == TIM7) { //1ms 마다 타이머 인터럽트
		time++; //실행 시간 증가
		clock_time++; // 현재 시간 증가

		if (clock_time == 1000) {
			second++;
			if (second == 60) {
				second = 0;
				minute++;
			}
			clock_time = 0;
		}

		if (sw1 == true && Press_Time <= 2610) { //스위치 누를때 press 시간 증가
			Press_Time++;
		}
		if (sw2 == true && Press_Time <= 2610) { //스위치 누를때 press 시간 증가
			Press_Time++;
		}
		if (sw3 == true && Press_Time <= 2610) { //스위치 누를때 press 시간 증가
			Press_Time++;
		}
		if (sw4 == true && Press_Time <= 2610) { //스위치 누를때 press 시간 증가
			Press_Time++;
		}

		if (stopwatch_running == true) {
			stopwatch_time++;
		}
		if (timer_start && timer_time_tmp > 0) {
			timer_time_tmp--;
		}

		if (minute >= 60) {
			minute = 0;
			hour++;
		}

		if (hour >= 24) {
			hour = 0;
			day++;
		}
		if (buzzer) {
			if (Press_Time > 0 && Press_Time <= 100) {
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM 출력 시작
				TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
				TIM2->CCR1 = 500; // 듀티 사이클 초기값으로 되돌림
			} else if (Press_Time >= 700 && Press_Time <= 800) {
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM 출력 시작
				TIM2->ARR = 800 - 1; // 주파수 설정
				TIM2->CCR1 = 400; // 듀티 사이클 설정 (50% duty cycle)
			} else if (Press_Time >= 2500 && Press_Time <= 2600) {
				HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM 출력 시작
				TIM2->ARR = 600 - 1; // 주파수 초기값으로 되돌림
				TIM2->CCR1 = 300; // 듀티 사이클 초기값으로 되돌림
			} else {
				HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1); // PWM 출력 멈춤
				TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
				TIM2->CCR1 = 0; // 듀티 사이클 0으로 설정
			}
		}

		if (alarm_ring && !alarm_setmode) {
			if (clock_time % 100 < 50) { // 0.1초마다 부저 켜기/끄기
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
				if (buzzer) {
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM 출력 시작
					TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
					TIM2->CCR1 = 500; // 듀티 사이클 초기값으로 되돌림
				}
			} else {
				if (buzzer) {
					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1); // PWM 출력 멈춤
					TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
					TIM2->CCR1 = 0; // 듀티 사이클 0으로 설정
				}
			}
		}

		if (timer_time_tmp == 0 && timer_start) {
			if (clock_time % 100 < 50) { // 0.1초마다 부저 켜기/끄기
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
				if (buzzer) {
					HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // PWM 출력 시작
					TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
					TIM2->CCR1 = 500; // 듀티 사이클 초기값으로 되돌림
				}
			} else {
				if (buzzer) {
					HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1); // PWM 출력 멈춤
					TIM2->ARR = 1000 - 1; // 주파수 초기값으로 되돌림
					TIM2->CCR1 = 0; // 듀티 사이클 0으로 설정
				}
			}
		}

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { //외부 인터럽트 호출
	if (GPIO_Pin == GPIO_PIN_3) {
		if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET) { // GPIOE 포트의 GPIO_PIN_3 핀이 현재 눌린 상태(높은 전압 상태)인지 확인
			if (sw1 == false) { // 이전 상태는 눌리지 않은 상태
				sw1 = true; // 이전 상태를 현재 상태로 업데이트(스위치가 눌렸음을 표시)
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화

			}
		} else {	// GPIOE 포트의 GPIO_PIN_3 핀이 현재 떼진 상태(낮은 전압 상태)인지 확인
			if (sw1 == true) {	// 이전 상태는 눌린 상태
				sw1 = false; // 이전 상태를 현재 상태로 업데이트(스위치가 떼졌음을 표시)
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw1_debounced = true; // sw1_debounced 변수를 true로 설정하여 디바운스 처리 완료 표시
				Press_Time = 0;	// sw1이 떼질 때 Press_Time 초기화
				if (!alarm_ring) {
					if (Press_Mode == 1 && !clock_setmode && !timer_setmode
							&& !alarm_setmode) { //짧게 누르고 수정상태 아니면 모드 변경
						mode++;
						mode_changed = true;
					}

					if (Press_Mode == 1 && clock_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select++;
						if (item_select == 6) {
							item_select = 0;
						}
					}

					if (Press_Mode == 1 && timer_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select2++;
						if (item_select2 == 3) {
							item_select2 = 0;
						}
					}

					if (Press_Mode == 1 && alarm_setmode) { //짧게 누르고 수정상태이면 수정값 증가시키기
						item_select3++;
						if (item_select3 == 4) {
							item_select3 = 0;
						}
					}
				}

				if (alarm_ring && !alarm_setmode) {
					alarm_ring = false;
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
				}
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_15) {
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_SET) { // GPIOC 포트의 GPIO_PIN_15 핀이 현재 눌린 상태(높은 전압 상태)인지 확인
			if (sw2 == false) { 	// 이전 상태는 눌리지 않은 상태
				sw2 = true; 	// 이전 상태를 현재 상태로 업데이트(스위치가 눌렸음을 표시)
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; 	// sw2이 눌릴 때 Press_Time 초기화
			}
		} else { 	// GPIOC 포트의 GPIO_PIN_15 핀이 현재 떼진 상태(낮은 전압 상태)인지 확인
			if (sw2 == true) { 	// 이전 상태는 눌린 상태
				sw2 = false; 	// 이전 상태를 현재 상태로 업데이트(스위치가 떼졌음을 표시)
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw2_debounced = true; // sw2_debounced 변수를 true로 설정하여 디바운스 처리 완료 표시
				Press_Time = 0; 	// sw1이 떼질 때 Press_Time 초기화
				if (timer_time_tmp == 0 && timer_start && mode != 4) {
					timer_start = !timer_start;
				}
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_4) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET) {
			if (sw3 == false) {
				sw3 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				sw3_debounced2 = true;
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화
			}
		} else {
			if (sw3 == true) {
				sw3 = false;
				sw3_debounced2 = false;
				sw3_debounced = true;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				Press_Time = 0;
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_10) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET) {
			if (sw4 == false) {
				sw4 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화
			}
		} else {
			if (sw4 == true) {
				sw4 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw4_debounced = true;
				Press_Time = 0;
			}
		}
	}
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
