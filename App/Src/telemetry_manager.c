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

// Тайминги согласно промышленному стандарту
#define HEARTBEAT_INTERVAL_MS  30000  // 30 секунд (Основной пульс USER_DATA)
#define DIAG_SYNC_INTERVAL_MS  300000 // 5 минут (Контрольная диагностика HEALTH_DATA)

// Состояние для отслеживания изменений (Events)
static int8_t  last_sent_temp = 0;
static uint8_t last_relay_state = 0;

// Таймеры последнего обмена
static uint32_t last_heartbeat_time = 0;
static uint32_t last_diag_time = 0;

/**
 * @brief Вспомогательная функция отправки USER_DATA (0x03)
 */
static void SendUserData(void) {
	uint8_t data[4];
    // Получаем температуру и приводим к протоколу (Значение * 10)
    int16_t t_curr = (int16_t)ADC_GetFilteredMCUCoreTemp() * 10;
    int16_t t_target = 225; // Заглушка: 22.5 градуса (до реализации thermostat.h)

    // Упаковка в Big-Endian (MSB first)
    data[0] = (uint8_t)(t_curr >> 8);
    data[1] = (uint8_t)(t_curr & 0xFF);
    data[2] = (uint8_t)(t_target >> 8);
    data[3] = (uint8_t)(t_target & 0xFF);

    FrameSender_SendFrame(0x03, data, 4);

    // Обновляем состояние для следующей проверки дельты
    last_sent_temp = (int8_t)(t_curr / 10);
    last_heartbeat_time = get_uptime_ms();
}

/**
 * @brief Вспомогательная функция отправки HEALTH_DATA (0x04)
 */
static void SendHealthData(void) {
	uint8_t health[7];
    uint8_t sens_stat = 0; // Состояние датчика (0-OK)
    uint8_t relay = GPIO_Relay_GetState(); // Чтение реального пина реле

    health[0] = sens_stat;
    health[1] = relay;
    health[2] = ADC_GetFilteredMCUCoreTemp();

    uint32_t uptime = get_uptime_ms() / 1000; // Аптайм в секундах
    health[3] = (uint8_t)(uptime >> 24);
    health[4] = (uint8_t)(uptime >> 16);
    health[5] = (uint8_t)(uptime >> 8);
    health[6] = (uint8_t)(uptime & 0xFF);

    FrameSender_SendFrame(0x04, health, 7);

    // Обновляем состояние для следующей проверки событий
    last_relay_state = relay;
    last_diag_time = get_uptime_ms();
}

void Telemetry_Init(void) {
	// Принудительная отправка при старте для синхронизации с ESP32
	SendUserData();
	SendHealthData();
}

void Telemetry_Task(void) {
	uint32_t now = get_uptime_ms();
    int8_t current_temp = ADC_GetFilteredMCUCoreTemp();
    uint8_t current_relay = GPIO_Relay_GetState();

    // 1. ПРОВЕРКА СОБЫТИЙ (Event-driven - Мгновенная реакция)

    // Событие: Переключение реле
    if (current_relay != last_relay_state) {
    	SendHealthData();
    }

    // Событие: Изменение температуры на 1 градус (минимум для чипа)
    // Используем модуль разности
    int8_t diff = (current_temp > last_sent_temp) ? (current_temp - last_sent_temp) : (last_sent_temp - current_temp);
    if (diff >= 1) {
    	SendUserData();
    }

    // 2. ПРОВЕРКА ТАЙМЕРОВ (Periodic - Контрольный пульс)

    // Таймер: USER_DATA (30с)
    if ((now - last_heartbeat_time) >= HEARTBEAT_INTERVAL_MS) {
    	SendUserData();
    }

    // Таймер: HEALTH_DATA (5 мин)
    if ((now - last_diag_time) >= DIAG_SYNC_INTERVAL_MS) {
    	SendHealthData();
    }
}

