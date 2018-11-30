/*
 * stepper_motor.h
 *
 *  Created on: Mar 13, 2017
 *      Author: Atalville
 */

#ifndef STEPPER_MOTOR_H_
#define STEPPER_MOTOR_H_
#define WAVE_DRIVE_STEP_DELAY_MS    3
#define FULL_STEP_DELAY_MS          3
#define HALF_STEP_DELAY_MS          3
#define _delay_ms   Delay1ms
typedef struct{
    uint32_t port;
    uint8_t in[4];
}_STEPPER_MOTOR_PINS;
void stepper_init(_STEPPER_MOTOR_PINS *p);
void stepper_wave_drive_rot_angle(uint16_t angle, bool dir);
void stepper_full_step_rot_angle(uint16_t angle, bool dir);
void stepper_half_step_rot_angle(uint16_t angle, bool dir);
#endif /* STEPPER_MOTOR_H_ */
