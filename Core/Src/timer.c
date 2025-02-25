#include "timer.h"
#include <stdio.h>
#include "variables.h"
#include "7SEG.h"
#include "CLCD.h"


void Timer_button_operation() {
// Enter timer setting mode on long press of button 1 (sw1)
	if (sw1_released) {
		if (Press_Mode >= PRESS_MID) {
			CLCD_Clear();
			timer_setmode = !timer_setmode; // Toggle timer setting mode
			Press_Mode = PRESS_NOT;
		}
		sw1_released = false;
	}

// Setting mode adjustments
	if (timer_setmode) {
		// Adjusting the timer settings using buttons (assuming sw2 to increase hours, sw3 to increase minutes, sw4 to increase seconds)
		if (sw2_released) {
			updateTime3();
			sw2_released = false;
		}

		if (sw2 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime3();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime3();
					Run_Time = 0;
				}
			}
		}

		if (sw3_released) {
			updateTime4();
			sw3_released = false;
		}

		if (sw3 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime4();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime4();
					Run_Time = 0;
				}
			}
		}
		timer_time = timer_time_tmp;

	}

	else {
		// Starting/Stopping the timer using button 2 (sw2)
		if (sw2_released) {
			if (timer_time != 0) {
				timer_start = !timer_start;
			}
			sw2_released = false;
		}

		if (sw3_released) {
			if (!timer_start) {
				timer_time = 0;
				timer_time_tmp = 0;
			}
			sw3_released = false;
		}
		if (sw4_released) {
			sw4_released = false;
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
