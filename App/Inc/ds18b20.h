/*
 * ds18b20.h
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Инициализация датчика DS18B20.
 * @return true если датчик обнаружен на шине.
 */
bool DS18B20_Init(void);

/**
 * @brief Запуск процесса преобразования температуры.
 * Занимает до 750мс (внутри датчика). Функция неблокирующая.
 */
void DS18B20_StartConversion(void);

/**
 * @brief Чтение результата последнего преобразования.
 * @return Температура в градусах Цельсия (с точностью до 0.1C, умноженная на 10).
 * Пример: 25.5C вернет 255.
 */
int16_t DS18B20_ReadTemperature(void);




#endif /* INC_DS18B20_H_ */
