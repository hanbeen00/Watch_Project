#include "stopwatch.h"
#include <stdio.h>
#include "variables.h"
#include "7SEG.h"
#include "CLCD.h"


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
	if (sw2_released == true) {
		if (lap_time_index == 0) {
			sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
			CLCD_Puts(0, 1, str);
		}
		stopwatch_running = !stopwatch_running;
		sw2_released = false;
	}

	if (stopwatch_running == false && sw3_pressed == true) {
		stopwatch_time = 0;
		lap_time_index = 0;
		sprintf(str, "%16s", ""); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw3_pressed = false;
	}

	else if (stopwatch_running == true && sw3_pressed == true) {
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
		sw3_pressed = false;
	}

	if (lap_time_index != 0 && sw4_released == true) {
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
		sw4_released = false;
	}

	else if (lap_time_index == 0 && sw4_released == true) {
		sprintf(str, "%16s", "NO LAP          "); // 스톱워치 시간 1ms 단위 LCD 출력
		CLCD_Puts(0, 1, str);
		sw4_released = false;
	}

}
