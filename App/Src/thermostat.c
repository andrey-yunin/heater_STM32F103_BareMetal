/*
 * thermostat.c
 *
 *  Created on: Mar 25, 2026
 *      Author: andrey
 */

#include "thermostat.h"
#include "telemetry_manager.h"
#include "gpio.h"
#include "sys_time.h"

#define MIN_SWITCH_INTERVAL_MS  5000 // Защита реле (5 секунд)

static uint32_t last_switch_time = 0;

void Thermostat_Init(void) {
	GPIO_Relay_SetState(0);
	last_switch_time = get_uptime_ms();
}

/**
 * @brief Задача-Исполнитель.
 * Безопасно применяет решение Мозга к физическому реле.
 */
void Thermostat_Task(void) {
	uint32_t now = get_uptime_ms();

	// 1. Получаем текущее решение от центрального "Мозга"
	bool request = Telemetry_GetRelayRequest();
	bool current_phys_state = GPIO_Relay_GetState();

	// 2. Если физика уже соответствует желаемому - ничего не делаем
	if (request == current_phys_state) {
		return;
	}

	// 3. БЕЗОПАСНОЕ ИСПОЛНЕНИЕ
	// Переключаем только если прошло более 5 секунд с прошлого раза
	if ((now - last_switch_time) >= MIN_SWITCH_INTERVAL_MS) {
		GPIO_Relay_SetState(request);
		last_switch_time = now;
	}
}
