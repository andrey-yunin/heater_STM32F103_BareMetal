/*
 * cmd_handler.c
 *
 *  Created on: Mar 16, 2026
 *      Author: andrey
 */

#include "cmd_handler.h"
#include "frame_packer.h"
#include "uart.h"
#include "frame_sender.h"
#include "telemetry_manager.h"
#include <string.h>


// Локальный буфер для текущего фрейма
static Frame_t current_frame;

void CmdHandler_Init(void) {
	memset(&current_frame, 0, sizeof(Frame_t));
	}

/**
 * @brief Обработка конкретной команды (Внутренняя логика)
 */
static void ProcessCommand(Frame_t *frame) {
	switch (frame->cmd) {
		case 0x10: // PING
			// Пока просто шлем текст для отладки,
			// позже заменим на бинарный ACK через FrameSender
			//UART1_SendString("ACK: PING\r\n");
			FrameSender_SendACK(frame->cmd);
			break;

		case 0x13: // SET_TARGET (2 байта: int16_t)
			if (frame->len == 2) {
				// Склеиваем байты (Big-Endian согласно UART_PROTOCOL.md)
				int16_t temp_raw = (frame->data[0] << 8) | frame->data[1];

				// ПЕРЕДАЕМ уставку в Центральный Мозг (Telemetry Manager)
				Telemetry_SetTargetTemp(temp_raw);

				FrameSender_SendACK(frame->cmd);
				}
			 else {
				 FrameSender_SendNACK(0x03);
			 }
			break;

		default:
			// Неизвестная команда
			FrameSender_SendNACK(0x02); // Неизвестная команда (Invalid ID)

			break;
			}
	}

void CmdHandler_Task(void) {
	PackerStatus_t status = FramePacker_Task(&current_frame);
	if (status == PACKER_READY) {
		ProcessCommand(&current_frame);

		// ЗАЩИТА ОТ ПОВТОРОВ: Сбрасываем команду после обработки
		current_frame.cmd = 0x00;
		}
	else if (status == PACKER_ERROR) {
		// Здесь можно добавить счетчик ошибок или лог
		// UART1_SendString("ERR: PACKET\r\n");
		}
	}

