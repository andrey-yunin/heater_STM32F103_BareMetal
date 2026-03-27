/*
 * telemetry_manager.h
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#ifndef INC_TELEMETRY_MANAGER_H_
#define INC_TELEMETRY_MANAGER_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Инициализация менеджера
 */
void Telemetry_Init(void);

/**
 * @brief Основная задача: опрос датчиков, принятие решений и отправка отчетов
 */
void Telemetry_Task(void);

/**
 * @brief Установка целевой температуры (вызывается из CmdHandler)
 * @param target_x10 Температура в формате "Градусы * 10" (225 = 22.5C)
 */
void Telemetry_SetTargetTemp(int16_t target_x10);

/**
 * @brief Получение текущей уставки
 */
int16_t Telemetry_GetTargetTemp(void);

/**
 * @brief Получение логического решения для реле
 * @return true - требуется нагрев, false - нагрев не требуется
 */
bool Telemetry_GetRelayRequest(void);


#endif /* INC_TELEMETRY_MANAGER_H_ */
