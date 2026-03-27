/*
 * ds18b20.c
 *
 *  Created on: Mar 24, 2026
 *      Author: andrey
 */

#include "ds18b20.h"
#include "onewire.h"

// Команды DS18B20 согласно DataSheet
#define CMD_CONVERT_T        0x44  // Начать измерение температуры
#define CMD_READ_SCRATCHPAD  0xBE  // Чтение памяти датчика
#define CMD_SKIP_ROM         0xCC  // Пропустить адрес (если датчик один)

bool DS18B20_Init(void) {
	OW_Init();
	return OW_Reset(); // Если Reset прошел успешно - датчик на месте
}

void DS18B20_StartConversion(void) {
	if (OW_Reset()) {
		OW_WriteByte(CMD_SKIP_ROM);    // Общаемся со всеми на шине
		OW_WriteByte(CMD_CONVERT_T);   // Команда: Начать измерение
	}
}

int16_t DS18B20_ReadTemperature(void) {
	if (OW_Reset()) {
		OW_WriteByte(CMD_SKIP_ROM);
		OW_WriteByte(CMD_READ_SCRATCHPAD);

		/**
         * Согласно спецификации, первые 2 байта из памяти датчика:
         * Byte 0: LSB (Младший байт температуры)
         * Byte 1: MSB (Старший байт температуры)
         */
         uint8_t temp_lsb = OW_ReadByte();
         uint8_t temp_msb = OW_ReadByte();

         // ВАЖНО: Сбрасываем шину после чтения нужных данных,
         // чтобы датчик не слал оставшиеся 7 байт (CRC и прочее)
         OW_Reset();

          // Склеиваем байты в 16-битное знаковое число
          int16_t raw_temp = (temp_msb << 8) | temp_lsb;

         /**
          * Математика перевода (DS18B20 12-bit mode):
          * 1 единица raw_temp = 0.0625 градуса.
          * Чтобы получить значение в формате "Градусы * 10" (например 25.5C -> 255):
          * temp_final = (raw_temp * 0.0625) * 10 = raw_temp * 0.625
          */
          int16_t temperature = (int16_t)((float)raw_temp * 0.625f);

          return temperature;
    }

	// В случае ошибки чтения (например, датчик отключили)
	return -999;
}

