/*
 * pid.c
 *
 *  Created on: Nov 6, 2023
 *      Author: NTPhong
 * 		Email: ntphong011102@gmail.com
 */
#include "pid.h"

// Khởi tạo bộ điều khiển PID
void PID_Init(PIDController *pid, double kp, double ki, double kd, double setpoint)
{
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
    pid->setpoint = setpoint;
    pid->integral = 0.0;
    pid->prev_error = 0.0;
}

// Cập nhật bộ điều khiển PID
double PID_Update(PIDController *pid, double process_variable)
{
    // Tính sai số
    double error = pid->setpoint - process_variable;

    // Tính số tích phân
    pid->integral += error;

    // Tính đạo hàm của sai số
    double derivative = error - pid->prev_error;

    // Tính tín hiệu điều khiển
    double control_signal = pid->Kp * error + pid->Ki * pid->integral + pid->Kd * derivative;

    // Lưu trữ sai số hiện tại để sử dụng cho lần cập nhật sau
    pid->prev_error = error;

    return control_signal;
}
