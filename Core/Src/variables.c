#include "variables.h"


/* 스위치 채터링 방지 변수 */
bool sw1 = false, sw2 = false, sw3 = false, sw4 = false;
bool sw1_released = false, sw2_released = false, sw3_released = false, sw3_pressed = false, sw4_released = false;

/* 총 실행 시간 변수 */
uint64_t Run_Time = 0;
uint8_t str[20];

/* 버튼 누른 시간 변수 */
uint16_t Press_Time = 0;
PressMode Press_Mode = PRESS_NOT;

/* 모드 변수 */
bool mode_changed = false;
ModeType Watch_Mode = MODE_CLOCK;

/* CLOCK 변수 */
uint64_t clock_time = 0;
uint8_t hour = 23, minute = 59, second = 50;
uint16_t millisecond = 0;
bool changed = true, buzzer = true;

uint16_t year = 2024;
uint8_t month = 12, day = 31;

bool clock_setmode = false;
uint8_t item_select = 0;

/* ALARM 변수 */
bool alarm_setmode = false;
bool alarm_changed[6] = { true, false, false, false, false, false };
uint8_t alarm_select = 1;
uint8_t alarm_hour[6] = { 0, };
uint8_t alarm_minute[6] = { 0, };
uint8_t item_select3 = 0;
bool alarm_ring = false;

/* STOPWATCH 변수 */
uint64_t stopwatch_time = 0;
uint64_t lap_time[10] = { 0, };
uint8_t lap_time_index = 0, lap_time_click = 0;
bool stopwatch_running = false;

/* TIMER 변수 */
uint64_t timer_time = 0;
uint64_t timer_time_tmp = 0;
bool timer_setmode = false;
uint8_t item_select2 = 0;



bool timer_start = false;
