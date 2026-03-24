/*
 * telemetry_manager.h
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#ifndef INC_TELEMETRY_MANAGER_H_
#define INC_TELEMETRY_MANAGER_H_


/**
 * @brief Инициализация менеджера телеметрии.
 */
void Telemetry_Init(void);

/**
 * @brief Задача телеметрии. Вызывается в главном цикле.
 * Управляет периодической и событийной отправкой USER_DATA и HEALTH_DATA.
 */
void Telemetry_Task(void);

#endif /* INC_TELEMETRY_MANAGER_H_ */
