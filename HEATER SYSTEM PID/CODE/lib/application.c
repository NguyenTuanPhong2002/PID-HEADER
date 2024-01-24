/*
 * application.c
 *
 *  Created on: Nov 5, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */
#include "application.h"

extern PIDController pid;
extern RTC_TimeTypeDef hrtc;
extern TIM_HandleTypeDef htim2;

RTC_TimeTypeDef HTime = { 0 };
RTC_DateTypeDef HDate = { 0 };

extern CLCD_I2C_Name LCD1;
extern SPI_HandleTypeDef hspi2;
extern I2C_HandleTypeDef hi2c1;

void initApp(void) {
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);

	CLCD_I2C_Init(&LCD1, &hi2c1, 0x7E, 20, 4);

	HAL_GPIO_WritePin(LED_LCD_GPIO_Port, LED_LCD_Pin, GPIO_PIN_SET);
}

void mainMenu(void) {
	//CLCD_I2C_Clear(&LCD1);
	CLCD_I2C_SetCursor(&LCD1, 0, 0);
	CLCD_I2C_WriteString(&LCD1, "SET TEM POINT:");
	CLCD_I2C_SetCursor(&LCD1, 0, 1);
	CLCD_I2C_WriteString(&LCD1, "REAL VALUE:");
	CLCD_I2C_SetCursor(&LCD1, 0, 2);
	CLCD_I2C_WriteString(&LCD1, "TIME RUNNER:");
	CLCD_I2C_SetCursor(&LCD1, 0, 3);
	CLCD_I2C_WriteString(&LCD1, "STATUS:");
}

void mainPower(void) {
	//CLCD_I2C_Clear(&LCD1);
	CLCD_I2C_SetCursor(&LCD1, 0, 0);
	CLCD_I2C_WriteString(&LCD1, "SET POWER POINT:");
	CLCD_I2C_SetCursor(&LCD1, 0, 1);
	CLCD_I2C_WriteString(&LCD1, "REAL VALUE:");
	CLCD_I2C_SetCursor(&LCD1, 0, 2);
	CLCD_I2C_WriteString(&LCD1, "TIME RUNNER:");
	CLCD_I2C_SetCursor(&LCD1, 0, 3);
	CLCD_I2C_WriteString(&LCD1, "STATUS:   RUN");
}

void updateSetpoint(uint16_t temp) {
	CLCD_I2C_SetCursor(&LCD1, 15, 0);
	char buffer[5];
	sprintf(buffer, "%04i", temp);
	CLCD_I2C_WriteString(&LCD1, buffer);
}

void updateRealValue(uint16_t temp) {
	CLCD_I2C_SetCursor(&LCD1, 15, 1);
	char buffer[5];
	sprintf(buffer, "%04i", temp);
	CLCD_I2C_WriteString(&LCD1, buffer);
}

void updatePowerpoint(float temp) {
	CLCD_I2C_SetCursor(&LCD1, 16, 0);
	char buffer[5];
	sprintf(buffer, "%02.2f", temp);
	CLCD_I2C_WriteString(&LCD1, buffer);
}

void updateStatus(uint8_t status) {
	CLCD_I2C_SetCursor(&LCD1, 13, 3);
	if (status == 0)
		CLCD_I2C_WriteString(&LCD1, "STOP");
	if (status == 1)
		CLCD_I2C_WriteString(&LCD1, "RUN ");
}

void initTime(void) {
	HAL_RTC_Init(&hrtc);
	HTime.Hours = 0;
	HTime.Minutes = 0;
	HTime.Seconds = 0;
	HAL_RTC_SetTime(&hrtc, &HTime, RTC_FORMAT_BIN);
}

void updateTime(void) {
	char buffer[10];

	HAL_RTC_GetTime(&hrtc, &HTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &HDate, RTC_FORMAT_BIN);

	CLCD_I2C_SetCursor(&LCD1, 12, 2);
	sprintf(buffer, "%02d:", HTime.Hours);
	CLCD_I2C_WriteString(&LCD1, buffer);
	CLCD_I2C_SetCursor(&LCD1, 15, 2);
	sprintf(buffer, "%02d:", HTime.Minutes);
	CLCD_I2C_WriteString(&LCD1, buffer);
	CLCD_I2C_SetCursor(&LCD1, 18, 2);
	sprintf(buffer, "%02d", HTime.Seconds);
	CLCD_I2C_WriteString(&LCD1, buffer);
}
