#ifndef VARIABLES_H
#define VARIABLES_H

#include <stdbool.h>
#include <stdint.h>

/* 스위치 채터링 방지 변수 */
extern bool sw1, sw2, sw3, sw4;
extern bool sw1_released, sw2_released, sw3_released,sw3_pressed, sw4_released;

/* 총 실행 시간 변수 */
extern uint64_t Run_Time;
extern uint8_t str[20];

/* 버튼 누른 시간 변수 */
extern uint16_t Press_Time;
typedef enum {
    PRESS_NOT = 0,
    PRESS_SHORT,
    PRESS_MID,
    PRESS_LONG
} PressMode;
extern PressMode Press_Mode;

/* 모드 변수 */
extern bool mode_changed;

typedef enum {
    MODE_CLOCK = 1,
    MODE_ALARM,
    MODE_STOPWATCH,
    MODE_TIMER
} ModeType;
extern ModeType Watch_Mode;

/* CLOCK 변수 */
extern uint64_t clock_time;
extern uint8_t hour, minute, second;
extern uint16_t millisecond;
extern bool changed, buzzer;

extern uint16_t year;
extern uint8_t month, day;

extern bool clock_setmode;
extern uint8_t item_select;

/* ALARM 변수 */
extern bool alarm_setmode;
extern bool alarm_changed[6];
extern uint8_t alarm_select;
extern uint8_t alarm_hour[6];
extern uint8_t alarm_minute[6];
extern uint8_t item_select3;
extern bool alarm_ring;

/* STOPWATCH 변수 */
extern uint64_t stopwatch_time;
extern uint64_t lap_time[10];
extern uint8_t lap_time_index, lap_time_click;
extern bool stopwatch_running;

/* TIMER 변수 */
extern uint64_t timer_time;
extern uint64_t timer_time_tmp;
extern bool timer_setmode;
extern uint8_t item_select2;


extern bool timer_start;

#endif /* VARIABLES_H */
