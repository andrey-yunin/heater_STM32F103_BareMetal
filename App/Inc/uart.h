/*
 * uart.h
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include <stdint.h>


/**
 * @brief Инициализация USART1 на скорости 115200 бод (при частоте шины 72 МГц).
 */
void UART1_Init(void);

/**
 * @brief Отправка одного символа.
 */
void UART1_SendChar(char c);

/**
 * @brief Отправка строки.
 */
void UART1_SendString(char* str);

// Флаги и переменные для работы с прерываниями (пока оставляем так)
extern volatile char rx_data;
extern volatile uint8_t data_ready;


#endif /* INC_UART_H_ */
