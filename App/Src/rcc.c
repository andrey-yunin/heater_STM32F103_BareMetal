/*
 * rcc.c
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#include "rcc.h"
#include "stm32f103_regs.h"

void RCC_Init(void) {
	// 1. Включаем HSE (внешний кварц)
	RCC_CR |= (1 << 16);
	while (!(RCC_CR & (1 << 17))); // Ждем готовности HSE

	// 2. Настройка Flash Wait States (2 такта задержки для 72МГц)
	FLASH_ACR |= (0x2 << 0);

	// 3. Настройка делителей шин:
	// AHB = SYSCLK (72MHz)
	// APB1 = SYSCLK/2 (36MHz - макс. для этой шины)
	// APB2 = SYSCLK (72MHz)
	RCC_CFGR |= (0x4 << 8); // PPRE1 = 100 (делитель /2)

	// 4. Настройка PLL: Источник HSE, Множитель x9 (8MHz * 9 = 72MHz)
	RCC_CFGR |= (1 << 16) | (0x7 << 18);

	// 5. Включаем PLL и ждем
	RCC_CR |= (1 << 24);

	while (!(RCC_CR & (1 << 25)));

	// 6. Переключаем систему на PLL
	RCC_CFGR |= (0x2 << 0);
	while (((RCC_CFGR >> 2) & 0x3) != 0x2);
	}



