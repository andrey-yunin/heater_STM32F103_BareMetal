/*
 * uart.c
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#include "uart.h"
#include "stm32f103_regs.h"

volatile char rx_data = 0;
volatile uint8_t data_ready = 0;

void UART1_Init(void) {
	// 1. Включаем тактирование GPIOA и USART1
	RCC_APB2ENR |= (1 << 2) | (1 << 14);

	// 2. Настройка PA9 (TX): Alternate Function Push-Pull, 50MHz (0xB)
    GPIOA_CRH &= ~(0xF << 4);
    GPIOA_CRH |= (0xB << 4);

    // 3. Настройка PA10 (RX): Input Floating (0x4)
    GPIOA_CRH &= ~(0xF << 8);
    GPIOA_CRH |= (0x4 << 8);

/**
 * 4. Расчет BRR для 115200 бод при частоте 72 МГц:
 * USARTDIV = 72 000 000 / (16 * 115200) = 39.0625
 * Mantissa = 39 (0x27)
 * Fraction = 0.0625 * 16 = 1 (0x1)
 * Итого BRR = 0x271
 */
USART1_BRR = 0x271;

// 5. Включаем: USART, TX, RX и прерывание по приему (RXNEIE)
USART1_CR1 |= (1 << 13) | (1 << 3) | (1 << 2) | (1 << 5);

// 6. Разрешаем прерывание USART1 в NVIC
NVIC_ISER1 |= (1 << 5);
}

void UART1_SendChar(char c) {
	while (!(USART1_SR & (1 << 7))); // Ждем TXE
	USART1_DR = c;
	}

void UART1_SendString(char* str) {
	while (*str) {
		UART1_SendChar(*str++);
		}
	}

// Обработчик прерывания (имя стандартное для стартапа)
void USART1_IRQHandler(void) {
	if (USART1_SR & (1 << 5)) {
		rx_data = (char)USART1_DR;
		data_ready = 1;
		}
	}

