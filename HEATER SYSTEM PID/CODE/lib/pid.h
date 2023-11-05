/*
 * pid.h
 *
 *  Created on: Nov 6, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */

#ifndef PID_H_
#define PID_H_

#include <stdio.h>

// Cấu trúc lưu trữ thông tin cho bộ điều khiển PID
typedef struct
{
    double Kp;         // Hệ số P
    double Ki;         // Hệ số I
    double Kd;         // Hệ số D
    double setpoint;   // Giá trị mục tiêu
    double integral;   // Số tích phân
    double prev_error; // Sai số trước đó
} PIDController;

void PID_Init(PIDController *pid, double kp, double ki, double kd, double setpoint);
double PID_Update(PIDController *pid, double process_variable);

#endif /* PID_H_ */
