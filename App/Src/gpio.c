/*
 * gpio.c
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#include "gpio.h"
#include "stm32f103_regs.h"

void GPIO_Init(void) {
	// 1. Включаем тактирование портов A и C
	RCC_APB2ENR |= (1 << 2) | (1 << 4);

	// 2. Настройка PC13 (Светодиод)
    // Режим: Output Push-Pull, 2MHz (0x2)
    GPIOC_CRH &= ~(0xF << 20); // Сброс бит 23:20
    GPIOC_CRH |= (0x2 << 20);  // Установка 0010

    // 3. Настройка PA0 (Реле) - на будущее
    // Режим: Output Push-Pull, 2MHz (0x2)
    GPIOA_CRL &= ~(0xF << 0);  // Сброс бит 3:0
    GPIOA_CRL |= (0x2 << 0);   // Установка 0010
    }

void LED_Toggle(void) {
	GPIOC_ODR ^= (1 << 13);
	}

void LED_On(void) {
	GPIOC_ODR &= ~(1 << 13); // Инверсная логика на Blue Pill: 0 - горит
	}

void LED_Off(void) {
	GPIOC_ODR |= (1 << 13);
	}

void Relay_On(void) {
	GPIOA_ODR |= (1 << 0);   // Прямая логика для реле: 1 - включено
	}

void Relay_Off(void) {
	GPIOA_ODR &= ~(1 << 0);
	}

uint8_t GPIO_Relay_GetState(void) {
	// Проверяем 0-й бит регистра ODR порта A (PA0 - Relay Pin)
	if (GPIOA_ODR & (1 << 0)) {
		return 1;
		}
	return 0;
}



