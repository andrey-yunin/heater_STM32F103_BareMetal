/*
 * fifo_handler.h
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */

#ifndef INC_FIFO_HANDLER_H_
#define INC_FIFO_HANDLER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Размер буфера (обязательно степень двойки для быстрой маскировки)
 */
 #define FIFO_SIZE 64
 #define FIFO_MASK (FIFO_SIZE - 1)

/**
 * @brief Добавление байта в очередь (вызывается из прерывания UART)
 * @param byte Принятый символ
 * @return true если байт добавлен, false если буфер переполнен
 */

bool FIFO_Push(uint8_t byte);

/**
 * @brief Извлечение байта из очереди (вызывается из логики протокола)
 * @param byte Указатель, куда записать результат
 * @return true если байт извлечен, false если очередь пуста
 */
bool FIFO_Pop(uint8_t *byte);

/**
 * @brief Проверка наличия данных в очереди
 */
bool FIFO_IsEmpty(void);

/**
 * @brief Получение количества байт, доступных в очереди
 */
uint32_t FIFO_GetCount(void);



#endif /* INC_FIFO_HANDLER_H_ */
