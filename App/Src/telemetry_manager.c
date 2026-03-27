/*
 * telemetry_manager.c
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#include "telemetry_manager.h"
#include "sys_time.h"
#include "adc.h"
#include "frame_sender.h"
#include "gpio.h"
#include "ds18b20.h"

// Тайминги согласно промышленному стандарту
#define HEARTBEAT_INTERVAL_MS  30000  // 30 секунд (Основной пульс USER_DATA)
#define DIAG_SYNC_INTERVAL_MS  300000 // 5 минут (Контрольная диагностика HEALTH_DATA)
#define HYSTERESIS_X10         5      // 0.5 градуса

// Внутреннее состояние "Мозга"
static int16_t  target_temp = 220;      // Уставка (по умолчанию 22.0C)
static int16_t  current_ds_temp = -999;  // Последнее измеренное значение
static bool     relay_request = false;   // Логическое решение для реле

// Таймеры последнего обмена
static uint32_t last_heartbeat_time = 0;
static uint32_t last_diag_time = 0;
static uint8_t  last_relay_state = 0; // ПЕРЕМЕННАЯ ДЛЯ ДЕТЕКТОРА СОБЫТИЙ

// --- Интерфейс управления (Геттеры/Сеттеры) ---
void Telemetry_SetTargetTemp(int16_t target_x10) {
	if (target_x10 >= 50 && target_x10 <= 350) {
		target_temp = target_x10;
	}
}

int16_t Telemetry_GetTargetTemp(void) {
	return target_temp;
}

bool Telemetry_GetRelayRequest(void) {
	return relay_request;
}

/**
 * @brief Вспомогательная функция отправки USER_DATA (0x03)
 */
static void SendUserData(void) {
	uint8_t data[4];

    // Упаковка в Big-Endian (MSB first)
    data[0] = (uint8_t)(current_ds_temp >> 8);
    data[1] = (uint8_t)(current_ds_temp & 0xFF);
    data[2] = (uint8_t)(target_temp >> 8);
    data[3] = (uint8_t)(target_temp & 0xFF);

    FrameSender_SendFrame(0x03, data, 4);

    // После отправки отчета всегда запускаем новый замер температуры
    DS18B20_StartConversion();
    last_heartbeat_time = get_uptime_ms();
}

/**
 * @brief Вспомогательная функция отправки HEALTH_DATA (0x04)
 */
static void SendHealthData(void) {
	uint8_t health[7];
	uint8_t current_phys_state = GPIO_Relay_GetState();

	// КРИТИЧЕСКАЯ ПРАВКА: Сбрасываем триггер события немедленно
	last_relay_state = current_phys_state;
	last_diag_time = get_uptime_ms();

	// Используем КЭШ, не трогаем шину OneWire заново!
	health[0] = (current_ds_temp == -999) ? 0x01 : 0x00;
	health[1] = current_phys_state; 
	health[2] = (uint8_t)ADC_GetFilteredMCUCoreTemp();

	uint32_t uptime = get_uptime_ms() / 1000;
	health[3] = (uint8_t)(uptime >> 24);
	health[4] = (uint8_t)(uptime >> 16);
	health[5] = (uint8_t)(uptime >> 8);
	health[6] = (uint8_t)(uptime & 0xFF);

	FrameSender_SendFrame(0x04, health, 7);
}

// --- Жизненный цикл задачи ---

void Telemetry_Init(void) {
	DS18B20_StartConversion();
	last_heartbeat_time = get_uptime_ms();
	last_diag_time = get_uptime_ms();

	// Принудительная отправка при старте для синхронизации с ESP32
    last_relay_state = GPIO_Relay_GetState();
	SendUserData();
	SendHealthData();
}

void Telemetry_Task(void) {
	uint32_t now = get_uptime_ms();

	// 1. РЕАКТИВНОЕ ПРИНЯТИЕ РЕШЕНИЯ (Мозг думает постоянно)
	if (current_ds_temp == -999) {
		relay_request = false;
	} else {
		if (current_ds_temp < (target_temp - HYSTERESIS_X10)) {
			relay_request = true; 
		} else if (current_ds_temp > (target_temp + HYSTERESIS_X10)) {
			relay_request = false;
		}
	}

	// 2. МГНОВЕННАЯ РЕАКЦИЯ НА СОБЫТИЕ (Relay Event)
	if (GPIO_Relay_GetState() != last_relay_state) {
		SendHealthData();
	}

	// 3. ОСНОВНОЙ ЦИКЛ ОПРОСА (30 секунд)
	if ((now - last_heartbeat_time) >= HEARTBEAT_INTERVAL_MS) {
		current_ds_temp = DS18B20_ReadTemperature();
	    SendUserData();
    }

	// 4. ДИАГНОСТИЧЕСКИЙ ЦИКЛ (5 минут)
	if ((now - last_diag_time) >= DIAG_SYNC_INTERVAL_MS) {
		SendHealthData();
	}
}
