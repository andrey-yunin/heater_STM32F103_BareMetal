/*
 * thermostat.h
 *
 *  Created on: Mar 25, 2026
 *      Author: andrey
 */

#ifndef INC_THERMOSTAT_H_
#define INC_THERMOSTAT_H_

#include <stdint.h>

void    Thermostat_Init(void);
void    Thermostat_Task(void);
void    Thermostat_SetTarget(int16_t target_x10);
int16_t Thermostat_GetTarget(void);


#endif /* INC_THERMOSTAT_H_ */
