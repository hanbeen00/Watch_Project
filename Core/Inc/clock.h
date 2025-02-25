#ifndef __CLOCK_H
#define __CLOCK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

void Clock_basic_operation();
void Clock_display_operation();
void Clock_button_operation();
bool isLeapYear(uint16_t year);
void updateTime();
void updateTime2();

#ifdef __cplusplus
}
#endif

#endif
