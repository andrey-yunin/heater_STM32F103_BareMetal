/*
 * stm32f103_regs.h
 *
 *  Created on: Mar 12, 2026
 *      Author: andrey
 */

#ifndef STM32F103_REGS_H_
#define STM32F103_REGS_H_


#include <stdint.h>

/**
 * @brief БАЗОВЫЕ АДРЕСА ПЕРИФЕРИИ (Memory Map)
 * Эти адреса фиксированы внутри кристалла STM32F103.
 */
#define RCC_BASE      0x40021000UL // Reset and Clock Control (Управление питанием и частотой)
#define GPIOA_BASE    0x40010800UL // Порт A (Линии PA0-PA15)
#define GPIOB_BASE    0x40010C00UL // Порт B (Линии PB0-PB15)
#define GPIOC_BASE    0x40011000UL // Порт C (Линии PC0-PC15)
#define USART1_BASE   0x40013800UL // Универсальный синхронно-асинхронный приемопередатчик №1
#define SYSTICK_BASE  0xE000E010UL // Системный таймер (встроен в ядро Cortex-M3)
#define NVIC_BASE     0xE000E100UL // Nested Vectored Interrupt Controller (Контроллер прерываний)
#define ADC1_BASE     0x40012400UL // Аналого-цифровой преобразователь №1
#define FLASH_BASE    0x40022000UL // Flash Memory Interface



/**
 * @brief РЕГИСТРЫ RCC (Reset and Clock Control)
*/
// RCC_CR: Включение/выключение генераторов (HSI, HSE, PLL) и проверка их готовности.
#define RCC_CR        (*(volatile uint32_t *)(RCC_BASE + 0x00))
// RCC_CFGR: Настройка множителей PLL и делителей для шин AHB, APB1, APB2.
#define RCC_CFGR      (*(volatile uint32_t *)(RCC_BASE + 0x04))
// RCC_APB2ENR: Включение тактирования модулей на шине APB2 (GPIO, USART1, ADC, и др.)
#define RCC_APB2ENR   (*(volatile uint32_t *)(RCC_BASE + 0x18))
// Включение тактирования шины APB1
#define RCC_APB1ENR   (*(volatile uint32_t *)(RCC_BASE + 0x1C))


/**
 * @brief РЕГИСТРЫ GPIO (General Purpose I/O)
 *
 * Краткая справка по работе с GPIO :
 * CRL/CRH: Чтобы настроить пин, нужно сначала "занулить" соответствующие 4 бита, а затем записать туда нужный режим (например, 0x2 для обычного выхода 2МГц или 0x4 для входа).
 * IDR: Это "глаза" контроллера. Мы читаем этот регистр, чтобы узнать, нажата ли кнопка.
 * ODR: Это "руки" контроллера. Мы пишем сюда, чтобы включить реле или светодиод.
 *
 *
 */
// --- ПОРТ A ---
// CRL (Control Register Low): Конфигурация пинов PA0 - PA7.
#define GPIOA_CRL     (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
// CRH (Control Register High): Конфигурация пинов PA8 - PA15.
#define GPIOA_CRH     (*(volatile uint32_t *)(GPIOA_BASE + 0x04))
// IDR (Input Data Register): Чтение текущего логического уровня на пинах (0 или 1).
#define GPIOA_IDR     (*(volatile uint32_t *)(GPIOA_BASE + 0x08))
// ODR (Output Data Register): Установка логического уровня на выход (запись 1 = High).
#define GPIOA_ODR     (*(volatile uint32_t *)(GPIOA_BASE + 0x0C))

// --- ПОРТ B ---
// CRL: Конфигурация пинов PB0 - PB7.
#define GPIOB_CRL     (*(volatile uint32_t *)(GPIOB_BASE + 0x00))
// CRH: Конфигурация пинов PB8 - PB15.
#define GPIOB_CRH     (*(volatile uint32_t *)(GPIOB_BASE + 0x04))
// IDR: Чтение уровня на пинах порта B.
#define GPIOB_IDR     (*(volatile uint32_t *)(GPIOB_BASE + 0x08))
// ODR: Установка уровня на пинах порта B.
#define GPIOB_ODR     (*(volatile uint32_t *)(GPIOB_BASE + 0x0C))

// --- ПОРТ C ---
// CRL: Конфигурация пинов PC0 - PC7.
#define GPIOC_CRL     (*(volatile uint32_t *)(GPIOC_BASE + 0x00))
// CRH: Конфигурация пинов PC8 - PC15 (Здесь находится светодиод PC13).
#define GPIOC_CRH     (*(volatile uint32_t *)(GPIOC_BASE + 0x04))
// IDR: Чтение уровня на пинах порта C.
#define GPIOC_IDR     (*(volatile uint32_t *)(GPIOC_BASE + 0x08))
// ODR: Установка уровня на пинах порта C.
#define GPIOC_ODR     (*(volatile uint32_t *)(GPIOC_BASE + 0x0C))


/**
 * @brief РЕГИСТРЫ NVIC (Контроллер прерываний)
 */
// NVIC_ISER: Interrupt Set-Enable Register. Разрешает прохождение сигнала от периферии к ядру.
#define NVIC_ISER0    (*(volatile uint32_t *)(NVIC_BASE + 0x00)) // Прерывания 0-31
#define NVIC_ISER1    (*(volatile uint32_t *)(NVIC_BASE + 0x04)) // Прерывания 32-63 (здесь USART1)


/**
 * @brief РЕГИСТРЫ SYSTICK (Системный таймер ядра)
 */
// STK_CTRL: Управление (включить таймер, разрешить прерывание) и статус (флаг отсчета).
#define STK_CTRL      (*(volatile uint32_t *)(SYSTICK_BASE + 0x00))
// STK_LOAD: Значение перезагрузки (от этого числа таймер считает вниз до 0).
#define STK_LOAD      (*(volatile uint32_t *)(SYSTICK_BASE + 0x04))
// STK_VAL: Текущее значение счетчика в данный момент времени.
#define STK_VAL       (*(volatile uint32_t *)(SYSTICK_BASE + 0x08))


/**
 * @brief РЕГИСТРЫ USART1 (Связь)
 */
// USART1_SR: Регистр статуса (флаги: пришел байт, буфер передачи пуст и т.д.).
#define USART1_SR     (*(volatile uint32_t *)(USART1_BASE + 0x00))
// USART1_DR: Регистр данных (чтение получает входящий байт, запись отправляет байт).
#define USART1_DR     (*(volatile uint32_t *)(USART1_BASE + 0x04))
// USART1_BRR: Baud Rate Register (настройка скорости передачи: 9600, 115200 и т.д.).
#define USART1_BRR    (*(volatile uint32_t *)(USART1_BASE + 0x08))
// USART1_CR1: Control Register 1 (Включение UART, передатчика, приемника и прерываний).
#define USART1_CR1    (*(volatile uint32_t *)(USART1_BASE + 0x0C))


/**
 * @brief РЕГИСТРЫ АЦП (ADC1)
 */
// ADC1_SR: Статус (флаг окончания преобразования EOC).
#define ADC1_SR       (*(volatile uint32_t *)(ADC1_BASE + 0x00))
// ADC1_CR1: Настройка режимов (прерывания, режим сканирования).
#define ADC1_CR1      (*(volatile uint32_t *)(ADC1_BASE + 0x04))
// ADC1_CR2: Основное управление (Включение АЦП, запуск калибровки, старт измерения).
#define ADC1_CR2      (*(volatile uint32_t *)(ADC1_BASE + 0x08))
// ADC1_SMPR2: Настройка времени выборки для каналов 0-9.
#define ADC1_SMPR2    (*(volatile uint32_t *)(ADC1_BASE + 0x10))
// Настройка времени выборки (каналы 10-17)
#define ADC1_SMPR1    (*(volatile uint32_t *)(ADC1_BASE + 0x0C))
// ADC1_SQR3: Очередность каналов (какой канал оцифровать первым).
#define ADC1_SQR3     (*(volatile uint32_t *)(ADC1_BASE + 0x34))
// ADC1_DR: Регистр с результатом оцифровки (хранит 12-битное значение).
#define ADC1_DR       (*(volatile uint32_t *)(ADC1_BASE + 0x4C))


/**
 * @brief РЕГИСТРЫ FLASH
 */
// FLASH_ACR: Настройка времени ожидания (Wait States) для работы на высоких частотах.
#define FLASH_ACR     (*(volatile uint32_t *)(FLASH_BASE + 0x00))





#endif /* STM32F103_REGS_H_ */
