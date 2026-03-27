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


/**
 * @brief Прецизионная задержка в микросекундах.
 * @param us Количество микросекунд (макс. ~59 секунд при 72МГц)
  */
void delay_us(uint32_t us);


/**
 * @brief Инициализация аппаратного счетчика циклов DWT.
 * Должна быть вызвана один раз при старте (System_Init).
 */
void DWT_Init(void);

uint32_t get_uptime_ms(void); // Для будущих задач




#endif /* INC_SYS_TIME_H_ */
