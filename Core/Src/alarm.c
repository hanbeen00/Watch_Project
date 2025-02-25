#include "alarm.h"
#include <stdint.h>
#include <stdio.h>
#include "variables.h"
#include "7SEG.h"
#include "CLCD.h"

void Alarm_basic_operation() {
	if (alarm_hour[alarm_select] >= 24) {
		alarm_hour[alarm_select] = 0;
	}

	if (alarm_minute[alarm_select] >= 60) {
		alarm_minute[alarm_select] = 0;
		alarm_hour[alarm_select]++;
	}

}

void Alarm_display_operation() {

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

void Alarm_button_operation() {
	if (sw1_released) {
		if (Press_Mode >= PRESS_MID) {
			alarm_setmode = !alarm_setmode; // Toggle timer setting mode
			Press_Mode = PRESS_NOT;
		}
		sw1_released = false;
	}

	if (alarm_setmode) {
		if (sw2_released) {
			updateTime5();
			sw2_released = false;
		}
		if (sw2 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime5();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime5();
					Run_Time = 0;
				}
			}
		}

		if (sw3_released) {
			updateTime6();
			sw3_released = false;
		}
		if (sw3 == true) {
			if (Press_Mode == PRESS_MID) {
				if (Run_Time >= 500) {
					updateTime6();
					Run_Time = 0;
				}
			} else if (Press_Mode == PRESS_LONG) {
				if (Run_Time >= 200) {
					updateTime6();
					Run_Time = 0;
				}
			}
		}
	}

	else {
		if (sw2_released) {
			alarm_select++;
			if (alarm_select >= 6) {
				alarm_select = 1;
			}
			sw2_released = false;
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


void Alarm_Check(){
	for (int i = 1; i <= 5; i++) {
		if (alarm_changed[i] && !alarm_setmode) {
			if (alarm_hour[i] == hour && alarm_minute[i] == minute) {
				alarm_changed[i] = !alarm_changed[i];
			    alarm_ring = true;
			}
		}
	}
}
