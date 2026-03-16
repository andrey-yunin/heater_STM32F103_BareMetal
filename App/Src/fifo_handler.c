/*
 * fifo_handler.c
 *
 *  Created on: Mar 13, 2026
 *      Author: andrey
 */


#include "fifo_handler.h"

// Память буфера
static volatile uint8_t fifo_buf[FIFO_SIZE];

// Индексы. volatile обязателен, так как head меняется в прерывании
static volatile uint32_t head = 0; // Индекс для записи (Producer)
static volatile uint32_t tail = 0; // Индекс для чтения (Consumer)

bool FIFO_Push(uint8_t byte) {
// Проверка на переполнение: разница между индексами не должна превышать размер
if ((head - tail) < FIFO_SIZE) {
	fifo_buf[head & FIFO_MASK] = byte;
	head++; // Атомарное инкрементирование
	return true;
	}
return false; // Буфер переполнен, байт потерян
}

bool FIFO_Pop(uint8_t *byte) {
	if (head == tail) {
		return false; // Очередь пуста
		}
	*byte = fifo_buf[tail & FIFO_MASK];
	tail++;
	return true;
	}

bool FIFO_IsEmpty(void) {
	return (head == tail);
	}

uint32_t FIFO_GetCount(void) {
	return (head - tail);
	}

