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
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
//스위치 채터링 방지 변수
bool sw1 = false;
bool sw2 = false;
bool sw3 = false;
bool sw4 = false;

bool sw1_debounced = false;
bool sw2_debounced = false;
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM7_Init(void);
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

		if (mode > 3)
			mode = 1;

		switch (mode) {
		case 1:
			Init_basic_operation();
			Init_display_operation();
			Init_button_operation();
			break;
		case 2:
			Stopwatch_basic_operation();
			Stopwatch_display_operation();
			Stopwatch_button_operation();
			Init_button_operation();
			break;
		case 3:
			Timer_basic_operation();
			Timer_display_operation();
			Timer_button_operation();
			Init_button_operation();
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

}
void Init_display_operation() {
	sprintf(str, "%6d", time / 1000); //1초 단위 LCD 출력
	CLCD_Puts(10, 0, str);

	//0.1, 0.01초 단위 7SEG 출력
	if (time % 1000 / 100 > 5) { // 0.5초간 7SEG 깜박임
		_7SEG_SetNumber(DGT1, time % 1000 / 100, OFF);
	} else {
		_7SEG_SetNumber(DGT1, time % 1000 / 100, ON);
	}
	_7SEG_SetNumber(DGT2, time % 100 / 10, OFF);
}
void Init_button_operation() {

	//버튼 PRESS 상태 출력
	if (Press_Time < 700) {
		Press_Mode = 1;
	} else if (Press_Time < 2500) {
		Press_Mode = 2;
	} else {
		Press_Mode = 3;
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
	if (mode == 2 && sw2_debounced == true) {
		if (lap_time_index == 0) {
			sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
			CLCD_Puts(0, 1, str);
		}
		stopwatch_running = !stopwatch_running;
		sw2_debounced = false;
	}

	if (mode == 2 && stopwatch_running == false && sw3_debounced == true) {
		stopwatch_time = 0;
		lap_time_index = 0;
		sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw3_debounced = false;
	}

	else if (mode == 2 && stopwatch_running == true && sw3_debounced == true) {
		lap_time_click = 0;

		if (lap_time_index < 9) {
			lap_time_index++;
			lap_time[lap_time_index] = stopwatch_time;
			sprintf(str, "%2s%d %02d:%02d:%02d.%03d", "LP", lap_time_index,
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
		sw3_debounced = false;
	}

	if (mode == 2 && lap_time_index != 0 && sw4_debounced == true) {
		lap_time_click++;
		if (lap_time_click <= lap_time_index) {
			sprintf(str, "%1d/%1d %02d:%02d:%02d.%03d", lap_time_click,
					lap_time_index, (int) (stopwatch_time / 1000) / 3600,
					(int) ((stopwatch_time / 1000) / 60) % 60,
					(int) (stopwatch_time / 1000) % 60,
					(int) stopwatch_time % 1000);
			CLCD_Puts(0, 1, str);

			if (lap_time_click == lap_time_index) {
				lap_time_click = 0;
			}
		}

		sw4_debounced = false;
	}

	else if (mode == 2 && lap_time_index == 0 && sw4_debounced == true) {
		sprintf(str, "%16s", "NO LAP          "); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw4_debounced = false;
	}
}

void Timer_button_operation() {
    // Enter timer setting mode on long press of button 1 (sw1)
    if (sw1_debounced) {
        if (Press_Mode >= 2) {
            timer_setmode = !timer_setmode; // Toggle timer setting mode
            Press_Mode = 0;
        }
        sw1_debounced = false;
    }

    // Setting mode adjustments
    if (timer_setmode) {
        // Adjusting the timer settings using buttons (assuming sw2 to increase hours, sw3 to increase minutes, sw4 to increase seconds)
        if (sw2_debounced) {
            timer_time_tmp += 3600000; // Increase hours
            sw2_debounced = false;
        }
        if (sw3_debounced) {
            timer_time_tmp += 60000; // Increase minutes
            sw3_debounced = false;
        }
        if (sw4_debounced) {
            timer_time_tmp += 1000; // Increase seconds
            sw4_debounced = false;
        }
    } else {
        // Starting/Stopping the timer using button 2 (sw2)
        if (sw2_debounced) {
            if (timer_time == 0) { // Start the timer
                timer_time = timer_time_tmp;
            } else { // Stop the timer
                timer_time = 0;
            }
            sw2_debounced = false;
        }
    }
}

void Timer_basic_operation() {
    // If the timer is running, decrement the timer
		sprintf(str,"%5s","TIMER");
		CLCD_Puts(0, 0, str);
        if (timer_time == 0) {
            // Timer finished, trigger any required action, e.g., sound an alarm
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET); // Example: turn on an LED
        }

}

void Timer_display_operation() {
    // Display the timer settings or countdown based on mode
    if (timer_setmode) {
        sprintf(str, "SET %02d:%02d:%02d", (int) (timer_time_tmp / 1000) / 3600,
                (int) ((timer_time_tmp / 1000) / 60) % 60,
                (int) (timer_time_tmp / 1000) % 60);
    } else {
        sprintf(str, "    %02d:%02d:%02d.%03d",
                (int) (timer_time_tmp / 1000) / 3600,
                (int) ((timer_time_tmp / 1000) / 60) % 60,
                (int) (timer_time_tmp / 1000) % 60,
                (int) timer_time_tmp % 1000);
    }
    CLCD_Puts(0, 1, str);
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) { //타이머 인터럽트 호출
	if (htim->Instance == TIM7) { //1ms 마다 타이머 인터럽트
		time++; //총 시간 증가
		if (stopwatch_running == true) {
			stopwatch_time++;
		}
		if (sw1 == true && Press_Time <= 2510) { //스위치 누를때 press 시간 증가
			Press_Time++;
		}
		if (timer_time > 0 && timer_time_tmp>0) {
		        timer_time_tmp--;
		}
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) { //외부 인터럽트 호출
	if (GPIO_Pin == GPIO_PIN_3) {
		if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) == GPIO_PIN_SET) {
			if (sw1 == false) {
				sw1 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				Press_Time = 0; // sw1이 눌릴 때 Press_Time 초기화

			}
		} else {
			if (sw1 == true) {
				sw1 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw1_debounced = true;
				if (Press_Mode == 1) {
					mode++;
				}
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_15) {
		if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15) == GPIO_PIN_SET) {
			if (sw2 == false) {
				sw2 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
		} else {
			if (sw2 == true) {
				sw2 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw2_debounced = true;
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_4) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_4) == GPIO_PIN_SET) {
			if (sw3 == false) {
				sw3 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
				sw3_debounced = true;
			}
		} else {
			if (sw3 == true) {
				sw3 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
			}
		}
	}

	else if (GPIO_Pin == GPIO_PIN_10) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_10) == GPIO_PIN_SET) {
			if (sw4 == false) {
				sw4 = true;
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
		} else {
			if (sw4 == true) {
				sw4 = false;
				HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_6);
				sw4_debounced = true;
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
