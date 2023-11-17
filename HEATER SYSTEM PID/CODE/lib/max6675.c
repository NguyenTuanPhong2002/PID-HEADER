/*
 * max6675.c
 *
 *  Created on: Nov 6, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "max6675.h"

extern SPI_HandleTypeDef hspi2;

#include "stm32f1xx.h" // Device header
#include "stdlib.h"
#include "stdio.h"

void MAX6675_Init() {
    // Cài đặt SPI
    hspi2.Instance = SPI2;
    hspi2.Init.Mode = SPI_MODE_MASTER;
    hspi2.Init.Direction = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize = SPI_DATASIZE_16BIT;
    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi2.Init.NSS = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    HAL_SPI_Init(&hspi2);
}

uint32_t getTem(void){
	uint32_t temperature=0;
    uint16_t value;
    uint8_t txData = 0;

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET); // Kích hoạt MAX6675

    HAL_SPI_Transmit(&hspi2, &txData, 1, HAL_MAX_DELAY);          // Gửi 1 byte để khởi đầu giao tiếp.
    HAL_SPI_Receive(&hspi2, (uint8_t *)&value, 2, HAL_MAX_DELAY); // Nhận 16 bit dữ liệu.

    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET); // Vô hiệu hóa MAX6675

    // Xử lý giá trị đọc để tính nhiệt độ Celsius
    if (value & 0x8000) {
      // Bit dấu là 1, nghĩa là lỗi (thermocouple open)
      // Trong trường hợp này, bạn có thể xử lý lỗi hoặc ghi nhận một giá trị nhiệt độ lỗi.
      // Ví dụ:
      temperature = -1.0; // Giá trị nhiệt độ lỗi (điều này phụ thuộc vào ứng dụng của bạn)
    } else {
      // Xóa bit dấu và chia cho 4 để tính nhiệt độ Celsius
      temperature = (float)((value >> 3) * 0.25);
    }

    return temperature;
}
