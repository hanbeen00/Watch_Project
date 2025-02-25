#include "clock.h"
#include <stdio.h>
#include <stdint.h>
#include "variables.h"
#include "7SEG.h"
#include "CLCD.h"

bool isLeapYear(uint16_t year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void Clock_basic_operation() {
    // 월별 최대 일 수 배열 (0번 인덱스는 사용하지 않음)
    int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 윤년이면 2월을 29일로 설정
    if (isLeapYear(year)) {
        days_in_month[2] = 29;
    }

    // 현재 일이 해당 월의 최대 일 수를 초과하면 월 증가
    if (day > days_in_month[month]) {
        day = 1;
        month++;
    }

    // 12월을 초과하면 연도 증가
    if (month > 12) {
        month = 1;
        year++;
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
			switch(item_select){
			case 0:
				sprintf(str, "%02d", (int) second);
				CLCD_Puts(14, 1, str);
				sprintf(str, "    ");
				CLCD_Puts(6, 0, str);
				break;
			case 1:
				sprintf(str, "%04d", (int) year);
				CLCD_Puts(6, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 0, str);
				break;
			case 2:
				sprintf(str, "%02d", (int) month);
				CLCD_Puts(11, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 0, str);
				break;
			case 3:
				sprintf(str, "%02d", (int) day);
				CLCD_Puts(14, 0, str);
				sprintf(str, "  ");
				CLCD_Puts(8, 1, str);
				break;
			case 4:
				sprintf(str, "%02d", displayHour);
				CLCD_Puts(8, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(11, 1, str);
				break;
			case 5:
				sprintf(str, "%02d", (int) minute);
				CLCD_Puts(11, 1, str);
				sprintf(str, "  ");
				CLCD_Puts(14, 1, str);
				break;
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
			_7SEG_SetNumber(DGT2, second % 10, ON);
		} else {
			sprintf(str, ":");
			CLCD_Puts(13, 1, str);
			_7SEG_SetNumber(DGT2, second % 10, OFF);
		}
		_7SEG_SetNumber(DGT1, second / 10, OFF);


		if (buzzer == false) {
			sprintf(str, "BZ OFF ");
		}
		else {
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
	if (sw1_released) {
		if (Press_Mode >= PRESS_MID) {
			clock_setmode = !clock_setmode; // Toggle timer setting mode
			Press_Mode = PRESS_NOT;
		}
		sw1_released = false;
	}

	if (clock_setmode) {
		if (sw2_released == true) {
			updateTime();
			sw2_released = false;
		}

		if (sw2 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime();
					Run_Time = 0;
				}
			}
		}

		if (sw3_released == true) {
			updateTime2(item_select);
		sw3_released = false;
		}

		if (sw3 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime2();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime2();
					Run_Time = 0;
				}
			}
		}
	} else {
		if (sw4_released == true) {
			changed = !changed;
			sw4_released = false;
		}

		if (sw2_released == true && !clock_setmode) {
			buzzer = !buzzer;
			sw2_released = false;
		}

		if (sw3_released) {
			sw3_released = false;
		}

	}

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
