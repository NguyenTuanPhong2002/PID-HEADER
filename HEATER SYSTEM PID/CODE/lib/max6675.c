/*
 * max6675.c
 *
 *  Created on: Nov 6, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#include "max6675.h"

#include "stm32f1xx.h" // Device header
#include "stdlib.h"
#include "stdio.h"

extern SPI_HandleTypeDef hspi2;

// ------------------- Variables ----------------
_Bool TCF = 0;     // Thermocouple Connection acknowledge Flag
uint8_t DATARX[2]; // Raw Data from MAX6675

// ------------------- Functions ----------------
double Max6675_Read_Temp(void)
{
	double Temp = 0;                                         // Temperature Variable
    HAL_GPIO_WritePin(SSPORT, SSPIN, GPIO_PIN_RESET);       // Low State for SPI Communication
    HAL_SPI_Receive(&hspi2, DATARX, 1, 50);                 // DATA Transfer
    HAL_GPIO_WritePin(SSPORT, SSPIN, GPIO_PIN_SET);         // High State for SPI Communication
    TCF = (((DATARX[0] | (DATARX[1] << 8)) >> 2) & 0x0001); // State of Connecting
    Temp = ((((DATARX[0] | DATARX[1] << 8))) >> 3);         // Temperature Data Extraction
    Temp *= 0.25;                                           // Data to Centigrade Conversation
    HAL_Delay(250);                                         // Waits for Chip Ready(according to Datasheet, the max time for conversion is 220ms)
    return Temp;
}
