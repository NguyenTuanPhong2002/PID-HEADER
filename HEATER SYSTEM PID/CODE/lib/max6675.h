/*
 * max6675.h
 *
 *  Created on: Nov 6, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef MAX6675_H_
#define MAX6675_H_

#include "main.h"

#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stdbool.h"
#include "stdlib.h"

void MAX6675_Init();

uint32_t getTem(void);

#endif /* MAX6675_H_ */
