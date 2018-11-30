/*
 * stepper_motor.c
 *
 *  Created on: Mar 13, 2017
 *      Author: Atalville
 */
#include "driverlib.h"
#include "stepper_motor.h"
#include "ST7735.h"



_STEPPER_MOTOR_PINS *p;
static void  wave_drive_step1(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[1]|p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[0]);
    _delay_ms(WAVE_DRIVE_STEP_DELAY_MS);
}
static void wave_drive_step2(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[1]);
    _delay_ms(WAVE_DRIVE_STEP_DELAY_MS);
}
static void wave_drive_step3(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[2]);
    _delay_ms(WAVE_DRIVE_STEP_DELAY_MS);
}
static void wave_drive_step4(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]|p->in[2]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[3]);
    _delay_ms(WAVE_DRIVE_STEP_DELAY_MS);
}
static void full_step1(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[0]|p->in[1]);
    _delay_ms(FULL_STEP_DELAY_MS);
}
static void full_step2(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[1]|p->in[2]);
    _delay_ms(FULL_STEP_DELAY_MS);
}
static void full_step3(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[2]|p->in[3]);
    _delay_ms(FULL_STEP_DELAY_MS);
}
static void full_step4(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[1]|p->in[2]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[3]|p->in[0]);
    _delay_ms(FULL_STEP_DELAY_MS);
}
static void half_step1(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[1]|p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[0]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step2(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[0]|p->in[1]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step3(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[2]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[1]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step4(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[1]|p->in[2]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step5(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]|p->in[3]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[2]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step6(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[2]|p->in[3]);
    _delay_ms(HALF_STEP_DELAY_MS);

}
static void half_step7(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[0]|p->in[1]|p->in[2]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[3]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void half_step8(){
    MAP_GPIO_setOutputLowOnPin(p->port,p->in[1]|p->in[2]);
    MAP_GPIO_setOutputHighOnPin(p->port,p->in[0]|p->in[3]);
    _delay_ms(HALF_STEP_DELAY_MS);
}
static void wave_drive_cc_sequence(uint16_t step){
    if(step == 0){
        wave_drive_step1();
    }else if(step == 1){
        wave_drive_step2();
    }else if(step == 2){
        wave_drive_step3();
    }else if(step == 3){
        wave_drive_step4();
    }
}
static void wave_drive_acc_sequence(uint16_t step){
    if(step == 0){
        wave_drive_step4();
    }else if(step == 1){
        wave_drive_step3();
    }else if(step == 2){
        wave_drive_step2();
    }else if(step == 3){
        wave_drive_step1();
    }
}
static void full_step_cc_sequence(uint16_t step){
    if(step == 0){
        full_step1();
    }else if(step == 1){
        full_step2();
    }else if(step == 2){
        full_step3();
    }else if(step == 3){
        full_step4();
    }
}
static void full_step_acc_sequence(uint16_t step){
    if(step == 0){
        full_step4();
    }else if(step == 1){
        full_step3();
    }else if(step == 2){
        full_step2();
    }else if(step == 3){
        full_step1();
    }
}
static void half_step_cc_sequence(uint16_t step){
    if(step == 0){
        half_step1();
    }else if(step == 1){
        half_step2();
    }else if(step == 2){
        half_step3();
    }else if(step == 3){
        half_step4();
    }else if(step == 4){
        half_step5();
    }else if(step == 5){
        half_step6();
    }else if(step == 6){
        half_step7();
    }else if(step == 7){
        half_step8();
    }
}
static void half_step_acc_sequence(uint16_t step){
    if(step == 0){
        half_step8();
    }else if(step == 1){
        half_step7();
    }else if(step == 2){
        half_step6();
    }else if(step == 3){
        half_step5();
    }else if(step == 4){
        half_step4();
    }else if(step == 5){
        half_step3();
    }else if(step == 6){
        half_step2();
    }else if(step == 7){
        half_step1();
    }
}
void stepper_init(_STEPPER_MOTOR_PINS *motor_pins){
    p = motor_pins;
    MAP_GPIO_setAsOutputPin(p->port, p->in[0]|p->in[1]|p->in[2]|p->in[3]);
}
/* Angle in degrees, dir is 1 for clockwise rotation and 0 for counter-clockwise rotation */
void stepper_wave_drive_rot_angle(uint16_t angle, bool dir){
    uint16_t steps = angle * 5.68;
    uint16_t i = 0;//, j = steps/4;
    for(i=0;i<steps;i++){
        if(dir==true){
            wave_drive_cc_sequence(i%4);
        }else{
            wave_drive_acc_sequence(i%4);
        }
    }
}
/* Angle in degrees, dir is 1 for clockwise rotation and 0 for counter-clockwise rotation */
void stepper_full_step_rot_angle(uint16_t angle, bool dir){
    uint16_t steps = angle * 5.68;
    uint16_t i = 0;//, j = steps/4;
    for(i=0;i<steps;i++){
        if(dir==true){
            full_step_cc_sequence(i%4);
        }else{
            full_step_acc_sequence(i%4);
        }
    }
}
/* Angle in degrees, dir is 1 for clockwise rotation and 0 for counter-clockwise rotation */
void stepper_half_step_rot_angle(uint16_t angle, bool dir){
    uint16_t steps = angle * 11.38;
    uint16_t i = 0;//, j = steps/4;
    for(i=0;i<steps;i++){
        if(dir==true){
            half_step_cc_sequence(i%8);
        }else{
            half_step_acc_sequence(i%8);
        }
    }
}
