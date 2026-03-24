/*
 * adc.c
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#include "adc.h"
#include "stm32f103_regs.h"

// Константы из DataSheet STM32F103x8
#define V25             1430    // Напряжение датчика при 25°C (мВ)
#define AVG_SLOPE       43      // Наклон характеристики (4.3 мВ/°C * 10 для точности)
#define VDD_CALIB       3300    // Опорное напряжение питания (мВ)
#define ADC_MAX         4095    // 12-битное разрешение (2^12 - 1)

// Буфер, куда DMA будет автоматически складывать данные АЦП
static volatile uint16_t adc_raw_data = 0;

// Состояние фильтра (начальное значение 25.0)
static float current_filtered_temp = 25.0f;

void ADC_Init(void) {
	// 1. Тактирование: ADC1 (APB2) и DMA1 (AHB)
	RCC_APB2ENR |= (1 << 9);
    RCC_AHBENR  |= (1 << 0);

    // 2. Настройка DMA1 Канал 1
    DMA1_CPAR1 = (uint32_t)&ADC1_DR;        // Источник: регистр данных АЦП
    DMA1_CMAR1 = (uint32_t)&adc_raw_data;  // Назначение: наша переменная в RAM
    DMA1_CNDTR1 = 1;                        // Количество передач: 1

    // Конфигурация: Circular (5), Memory Size 16-bit (10), Peripheral Size 16-bit (8), Enable (0)
    DMA1_CCR1 = (1 << 5) | (1 << 10) | (1 << 8);
    DMA1_CCR1 |= (1 << 0); // Включить DMA

    // 3. Настройка АЦП
    // Включаем внутренний датчик температуры и Vrefint (бит 23 TSVREFE)
    ADC1_CR2 |= (1 << 23);

    // Время выборки для Канала 16 (Internal Temp)
    // Должно быть > 17.1 мкс. При 12МГц АЦП ставим 239.5 циклов (0x7)
    ADC1_SMPR1 |= (0x7 << 18);

    // Очередность: только один канал 16 (записываем 16 в 5 бит SQR3)
    ADC1_SQR3 = 16;

    // Режимы: Continuous (1), DMA Enable (8), ADON (0)
    ADC1_CR2 |= (1 << 1) | (1 << 8) | (1 << 0);

    // 4. Самокалибровка (обязательно для точности в пром. решениях)
    for(volatile int i=0; i<1000; i++); // Небольшая задержка перед калибровкой
    ADC1_CR2 |= (1 << 2);               // Запуск CAL
    while(ADC1_CR2 & (1 << 2));         // Ждем сброса бита CAL железом

    // 5. Старт преобразования (второй ADON запускает цикл Continuous)
    ADC1_CR2 |= (1 << 0);
}

int8_t ADC_GetFilteredMCUCoreTemp(void) {
	// 1. Пересчет RAW в милливольты
    uint32_t v_sense = (adc_raw_data * VDD_CALIB) / ADC_MAX;

    // 2. Расчет температуры по формуле RM0008:
    // T = ((V25 - Vsense) / Avg_Slope) + 25
    float instant_temp = (float)(V25 - (int32_t)v_sense) / 4.3f + 25.0f;

    // 3. EMA фильтр: Alpha = 0.1 (10% нового значения, 90% истории)
    current_filtered_temp = (0.1f * instant_temp) + (0.9f * current_filtered_temp);

    return (int8_t)current_filtered_temp;
}

