/*
 * gpio.h
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

#include <stdint.h>

/**
 * @brief Инициализация всех необходимых GPIO пинов:
 * PC13 - Встроенный светодиод (Индикация)
 * PA0  - Выход на реле (Управление нагревателем) - заложим на будущее
 */
void GPIO_Init(void);

/**
 * @brief Управление светодиодом.
 */
void LED_Toggle(void);
void LED_On(void);
void LED_Off(void);

/**
 * @brief Управление реле.
 */
void Relay_On(void);
void Relay_Off(void);


#endif /* INC_GPIO_H_ */
