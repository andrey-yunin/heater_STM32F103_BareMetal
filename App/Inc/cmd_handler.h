/*
 * cmd_handler.h
 *
 *  Created on: Mar 16, 2026
 *      Author: andrey
 */

#ifndef INC_CMD_HANDLER_H_
#define INC_CMD_HANDLER_H_

#include "frame_packer.h"

/**
 * @brief Инициализация обработчика команд
 */
void CmdHandler_Init(void);


/**
 * @brief Основной цикл обработки команд.
 * Вызывает FramePacker_Task и, если пакет готов, исполняет его.
 */
void CmdHandler_Task(void);




#endif /* INC_CMD_HANDLER_H_ */
