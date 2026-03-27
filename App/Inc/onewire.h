/*
 * onewire.h
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Инициализация шины OneWire на пине PA1
 */
void OW_Init(void);

/**
 * @brief Сигнал сброса и проверка наличия датчика (Presence Pulse)
 * @return true если датчик ответил, false если линия пуста
 */
bool OW_Reset(void);

/**
 * @brief Запись одного байта на шину
 */
void OW_WriteByte(uint8_t data);

/**
 * @brief Чтение одного байта с шины
 */
uint8_t OW_ReadByte(void);


#endif /* INC_ONEWIRE_H_ */
