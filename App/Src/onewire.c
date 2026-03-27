/*
 * onewire.c
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */


#include "onewire.h"
#include "stm32f103_regs.h"
#include "sys_time.h"

/**
 * Менеджмент пина PA1 (OneWire):
 * Мы будем использовать PA1 для данных.
 */

// Переключение PA1 на выход (Push-Pull, 2MHz)
static void OW_SetOutput(void) {
	GPIOA_CRL &= ~(0xF << 4); // Сброс бит 7:4
    GPIOA_CRL |= (0x2 << 4);  // 0010: Output PP 2MHz
}

// Переключение PA1 на вход (Input Floating)
static void OW_SetInput(void) {
	GPIOA_CRL &= ~(0xF << 4); // Сброс бит 7:4
    GPIOA_CRL |= (0x4 << 4);  // 0100: Input Floating
}

void OW_Init(void) {
	// Включаем тактирование GPIOA
    RCC_APB2ENR |= (1 << 2);
    OW_SetInput(); // По умолчанию линия "отпущена"
}

bool OW_Reset(void) {
	bool presence = false;
	OW_SetOutput();
    GPIOA_ODR &= ~(1 << 1); // Притягиваем к земле
    delay_us(480);          // Минимум 480 мкс

    OW_SetInput();          // Отпускаем линию
    delay_us(70);           // Ждем ответа (датчик притянет линию через 15-60 мкс)

    if (!(GPIOA_IDR & (1 << 1))) {
    	presence = true;    // Линия притянута - датчик на месте
    }

    delay_us(410);          // Дочитываем тайм-слот до конца (480 - 70)
    return presence;
}

static void OW_WriteBit(uint8_t bit) {
	OW_SetOutput();
	GPIOA_ODR &= ~(1 << 1); // Начало тайм-слота (всегда 0)

	if (bit) {
		delay_us(10);       // Для записи "1" отпускаем линию быстро
		OW_SetInput();
		delay_us(55);
	}
	else {
		delay_us(65);       // Для записи "0" держим линию 60-120 мкс
		OW_SetInput();
		delay_us(5);
	}
}

static uint8_t OW_ReadBit(void) {
	uint8_t bit = 0;
	OW_SetOutput();
	GPIOA_ODR &= ~(1 << 1); // Начало тайм-слота (на 1-5 мкс)
	delay_us(3);

	OW_SetInput();          // Отпускаем
	delay_us(10);           // Датчик должен выставить данные

	if (GPIOA_IDR & (1 << 1)) {
		bit = 1;
	}

	delay_us(50);           // Завершаем тайм-слот
	return bit;
}

void OW_WriteByte(uint8_t data) {
	for (uint8_t i = 0; i < 8; i++) {
		OW_WriteBit(data & (1 << i));
		}
}

uint8_t OW_ReadByte(void) {
	uint8_t data = 0;
	for (uint8_t i = 0; i < 8; i++) {
		if (OW_ReadBit()) {
			data |= (1 << i); // Устанавливаем i-й бит, только если на линии "1"
			}
	}
	return data;
}
