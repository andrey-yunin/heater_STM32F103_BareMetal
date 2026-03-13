/*
 * sys_time.h
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#ifndef INC_SYS_TIME_H_
#define INC_SYS_TIME_H_

#include <stdint.h>

void SysTick_Init(void);
void delay_ms(uint32_t ms);
uint32_t get_uptime_ms(void); // Для будущих задач

#endif /* INC_SYS_TIME_H_ */
