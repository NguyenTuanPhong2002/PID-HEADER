/*
 * application.h
 *
 *  Created on: Nov 5, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "main.h"
#include <stdio.h>

#include "CLCD_I2C.h"
#include "pid.h"
#include "max6675.h"

void initApp(void);
void mainMenu(void);

void updateSetpoint(uint16_t temp);
void updateRealValue(uint16_t temp);
void updateStatus(uint8_t status);
void updateTime(void);

#endif /* APPLICATION_H_ */
