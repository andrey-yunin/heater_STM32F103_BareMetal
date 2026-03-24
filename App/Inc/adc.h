/*
 * adc.h
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

/**
 * @brief Инициализация ADC1 и DMA1 для автономного мониторинга температуры MCU.
 * Настраивает канал 16 (Internal Temp Sensor) в режиме Continuous Conversion.
 */
void ADC_Init(void);

/**
 * @brief Возвращает текущую отфильтрованную температуру кристалла.
 * Использует EMA-фильтр (Exponential Moving Average).
 * @return Температура в целых градусах Цельсия.
 */
int8_t ADC_GetFilteredMCUCoreTemp(void);


#endif /* INC_ADC_H_ */
