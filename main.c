//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************
#include <string.h>
#include <stdlib.h>
#include "driverlib.h"
#include "clk.h"
#include "neopixel.h"
#include "BackchannelUartIO.h"
#include "stepper_motor.h"

_STEPPER_MOTOR_PINS motor;

#define CODE 0

void init_rgb_array(_S_NEOPIXEL_RGB_ARRAY * l){
   memset(l,0,sizeof(_S_NEOPIXEL_RGB_ARRAY));
//   l->r[0] = 0xff;
//   l->g[1] = 0xff;
//   l->b[2] = 0xff;
//   l->w[3] = 0xff;
//   l->r[4] = 255; ; l->g[4] = 255; l->b[4] = 0;
//   l->r[5] = 200; ; l->g[5] = 20; l->b[5] = 120;
//   l->r[6] = 200; ; l->g[6] = 80; l->b[6] = 10;
   l->r[0] = 0xff;
   l->r[1] = 0xff;
   l->r[2] = 0xff;
   l->r[3] = 0xff;
   l->r[4] = 0xff;
   l->r[5] = 0xff;
   l->r[6] = 0xff;
   l->r[7] = 0xff;
}
void main(void)
{

     uint8_t state = 0;
     uint8_t intensity[5],r[5],g[5],b[5],w[5],motor_dir[5],motor_angle[5];
     uint8_t pixel[5];
     bool dir = false;
     long pixel_value,r_value,g_value,b_value,w_value,motor_dir_value, motor_angle_value;
     uint8_t *ptr=0;
     long intensity_value = 0;
     _S_NEOPIXEL_RGB_ARRAY rgb_array;
    motor.port =GPIO_PORT_P4;
    motor.in[0] = GPIO_PIN0;
    motor.in[1] = GPIO_PIN1;
    motor.in[2] = GPIO_PIN2;
    motor.in[3] = GPIO_PIN3;
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    clk_init();
    stepper_init(&motor);
    MAP_Interrupt_enableMaster();
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN0);
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2,GPIO_PIN7);
    initBackchannelUART();
    init_rgb_array(&rgb_array);
    my_puts("Neopixel Program Started!\r\n");
    all_off();
    Delay1ms(500);
    while(1){
#if CODE == 0
        uint8_t i;
        intensity_value=255;
        for ( i=0; i<1;i++)
        {
            red_one_at_a_time(255);
            green_one_at_a_time(255);
            blue_one_at_a_time(255);
        }
        //fade_red_all();
        //for ( i=0; i<10;i++)
        //green_one_at_a_time(127);
        //fade_green_all();
        //for ( i=0; i<10;i++)
        //blue_one_at_a_time(127);
        //fade_blue_all();
        for ( i=0; i<10;i++)
        {
          blue_on_all_intensity(255);
          Delay1ms(50);
          all_off();
          Delay1ms(50);
        }

        for ( i=0; i<1;i++)
                {
                red_one_at_a_time(255);
                green_one_at_a_time(255);
                blue_one_at_a_time(255);
                }


        for ( i=0; i<10;i++)
                {
                  green_on_all_intensity(255);
                  Delay1ms(50);
                  all_off();
                  Delay1ms(50);
                }
        for ( i=0; i<1;i++)
                        {
                        red_one_at_a_time(255);
                        green_one_at_a_time(255);
                        blue_one_at_a_time(255);
                        }


                for ( i=0; i<10;i++)
                        {
                          red_on_all_intensity(255);
                          Delay1ms(50);
                          all_off();
                          Delay1ms(50);
                        }
        //white_one_at_a_time(127);
        //fade_white_all();
        //green_on_all_intensity((uint8_t)intensity_value);
        //fade_green_all();
        //blue_on_all_intensity((uint8_t)intensity_value);
        //fade_blue_all();

#elif CODE == 1
        red_one_at_a_time(127);
        green_one_at_a_time(127);
        blue_one_at_a_time(127);
       // white_one_at_a_time(127);

#elif CODE == 2
        if(state == 0){
            my_puts("\r\nEnter Intensity(0-255) for RED\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            red_on_all_intensity((uint8_t)intensity_value);
            memset(intensity,0,5);
            state = 1;
        }else if (state == 1){
            my_puts("\r\nEnter Intensity(0-255) for GREEN\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            green_on_all_intensity((uint8_t)intensity_value);
            memset(intensity,0,5);
            state = 2;
        }else if(state == 2){
            my_puts("\r\nEnter Intensity(0-255) for BLUE\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            blue_on_all_intensity((uint8_t)intensity_value);
            memset(intensity,0,5);
            state = 3;
        }else if(state == 3){
            my_puts("\r\nEnter Intensity(0-255) for WHITE\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            white_on_all_intensity((uint8_t)intensity_value);
            memset(intensity,0,5);
            state = 0;
        }
#elif CODE == 3
        if(state == 0){
            my_puts("\r\nEnter Pixel Number(0-6) for RED\r\n");
            my_gets(pixel,5);
            pixel_value = strtoul(pixel,&ptr,10);
            my_puts("\r\nEnter Intensity(0-255) for RED\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            red_one_pixel_on_intensity(pixel_value,intensity_value);
            memset(pixel,0,5);
            memset(intensity,0,5);
            state = 1;
        }else if (state == 1){
            my_puts("\r\nEnter Pixel Number(0-6) for GREEN\r\n");
            my_gets(pixel,5);
            pixel_value = strtoul(pixel,&ptr,10);
            my_puts("\r\nEnter Intensity(0-255) for GREEN\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            green_one_pixel_on_intensity(pixel_value,intensity_value);
            memset(pixel,0,5);
            memset(intensity,0,5);
            state = 2;
        }else if(state == 2){
            my_puts("\r\nEnter Pixel Number(0-6) for BLUE\r\n");
            my_gets(pixel,5);
            pixel_value = strtoul(pixel,&ptr,10);
            my_puts("\r\nEnter Intensity(0-255) for BLUE\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            blue_one_pixel_on_intensity(pixel_value,intensity_value);
            memset(pixel,0,5);
            memset(intensity,0,5);
            state = 3;
        }else if(state == 3){
            my_puts("\r\nEnter Pixel Number(0-6) for WHITE\r\n");
            my_gets(pixel,5);
            pixel_value = strtoul(pixel,&ptr,10);
            my_puts("\r\nEnter Intensity(0-255) for WHITE\r\n");
            my_gets(intensity,5);
            intensity_value = strtoul(intensity,&ptr,10);
            white_one_pixel_on_intensity(pixel_value,intensity_value);
            memset(pixel,0,5);
            memset(intensity,0,5);
            state = 0;
        }
#elif CODE == 4
       if(state == 0){
            my_puts("\r\n*********************************************************\r\n");
            my_puts("\r\nEnter Intensity(0-255) for RED\r\n");
            my_gets(r,5);
            r_value = strtoul(r,&ptr,10);

            my_puts("\r\nEnter Intensity(0-255) for GREEN\r\n");
            my_gets(g,5);
            g_value = strtoul(g,&ptr,10);

            my_puts("\r\nEnter Intensity(0-255) for BLUE\r\n");
            my_gets(b,5);
            b_value = strtoul(b,&ptr,10);

            my_puts("\r\nEnter Intensity(0-255) for WHITE\r\n");
            my_gets(w,5);
            w_value = strtoul(w,&ptr,10);
            control_all_neopixel((uint8_t)r_value,(uint8_t)g_value,(uint8_t)b_value,(uint8_t)w_value);
            memset(r,0,5);
            memset(g,0,5);
            memset(b,0,5);
            memset(w,0,5);
            state = 1;
       }else if(state == 1){
           my_puts("\r\nEnter 1 for Clockwise and 0 for Counter-Clockwise rotation of motor\r\n");
           my_gets(motor_dir,5);
           motor_dir_value = strtoul(motor_dir,&ptr,10);

           my_puts("\r\nEnter angle of rotation for motor\r\n");
           my_gets(motor_angle,5);
           motor_angle_value = strtoul(motor_angle,&ptr,10);
           dir = (motor_dir_value)? true:false;
           stepper_full_step_rot_angle(motor_angle_value,dir);
//           stepper_full_step_rot_angle(motor_angle_value,dir);
//
//           stepper_full_step_rot_angle(motor_angle_value,dir);
//
//           stepper_full_step_rot_angle(motor_angle_value,dir);

           memset(motor_dir,0,5);
           memset(motor_angle,0,5);
           state = 0;
       }
#else
    #error "Wrong Code Number. It should be between from 0 to 4!!"
#endif

    }
}
