/*
 * neopixel.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Atalville
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_
#define F_CPU   12000000
#define NUM_LEDS 8

typedef struct{
    uint32_t port;
    uint8_t  pin;
    uint8_t portout;
}_S_NEOPIXEL;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
}_S_NEOPIXEL_LED;

typedef struct{
    uint8_t r[NUM_LEDS];
    uint8_t g[NUM_LEDS];
    uint8_t b[NUM_LEDS];
    uint8_t w[NUM_LEDS];
}_S_NEOPIXEL_RGB_ARRAY;


void neopixel_init(_S_NEOPIXEL * neopixel);
void showStrip() ;
void sendPixel( unsigned char r, unsigned char g , unsigned char b , unsigned char w);
void sendStrip(_S_NEOPIXEL_LED *led, uint32_t delay_cycles);
void led_test();
void sendPixelOnce( unsigned char r, unsigned char g , unsigned char b , unsigned char w);
void all_off();
void fade_red_all();
void fade_green_all();
void fade_blue_all();
void red_one_at_a_time(uint8_t intensity);
void green_one_at_a_time(uint8_t intensity);
void blue_one_at_a_time(uint8_t intensity);
void white_one_at_a_time(uint8_t intensity);
void red_gradual_on(uint8_t intensity);
void green_gradual_on(uint8_t intensity);
void blue_gradual_on(uint8_t intensity);
void white_gradual_on(uint8_t intensity);
void red_on_all_intensity(uint8_t intensity);
void green_on_all_intensity(uint8_t intensity);
void blue_on_all_intensity(uint8_t intensity);
void white_on_all_intensity(uint8_t intensity);
void red_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity);
void green_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity);
void blue_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity);
void white_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity);
void one_pixel_on_intensity(uint8_t pixel_no, uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void control_all_neopixel(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void control_each_neopixel(_S_NEOPIXEL_RGB_ARRAY *n);
void neopixel_one_on_at_a_time(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
void all_on();
#endif /* NEOPIXEL_H_ */
