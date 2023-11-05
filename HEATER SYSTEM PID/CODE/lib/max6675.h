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

#include "stdint.h"

// ------------------------- Defines -------------------------
#define SSPORT GPIOB     // GPIO Port of Chip Select(Slave Select)
#define SSPIN GPIO_PIN_12 // GPIO PIN of Chip Select(Slave Select)
// ------------------------- Functions  ----------------------
double Max6675_Read_Temp(void);

#endif /* MAX6675_H_ */
