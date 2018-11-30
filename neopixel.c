/*
 * neopixel.c
 *
 *  Created on: Mar 17, 2017
 *      Author: Atalville
 */
#include "driverlib.h"
#include "neopixel.h"
#include "ST7735.h"
#if 0
_S_NEOPIXEL *n;

#define PIXELS 96*11 // Number of pixels in the string

// These values depend on which pin your string is connected to and what board you are using
// More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation

// These values are for digital pin 8 on an Arduino Yun or digital pin 12 on a DueMilinove
// Note that you could also include the DigitalWriteFast header file to not need to to this lookup.

#define PIXEL_PORT PORTB // Port of the pin the pixels are connected to
#define PIXEL_DDR DDRB // Port of the pin the pixels are connected to
#define PIXEL_BIT 4 // Bit of the pin the pixels are connected to

// These are the timing constraints taken mostly from the WS2812 datasheets
// These are chosen to be conservative and avoid problems rather than for maximum throughput

//#define T1H  900    // Width of a 1 bit in ns
#define T1H  10    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns


#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns

#define RES 7000    // Width of the low gap between bits to cause a frame to latch

// Here are some convenience defines for using nanoseconds specs to generate actual CPU delays

#define NS_PER_SEC (1000000000L) // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

//#define DELAY_CYCLES(n) ( ((n)&gt;0) ? __builtin_avr_delay_cycles( n ) : __builtin_avr_delay_cycles( 0 ) ) // Make sure we never have a delay less than zero
#define DELAY_CYCLES(n)   __delay_cycles(n);
// Actually send a bit to the string. We turn off optimizations to make sure the compile does
// not reorder things and make it so the delay happens in the wrong place.

void sendBit(bool) __attribute__ ((optimize(0)));

void sendBit( bool bitVal ) {

    if ( bitVal ) {      // 1-bit

      MAP_GPIO_setOutputHighOnPin(n->port,n->pin);

      DELAY_CYCLES( NS_TO_CYCLES( T1H ) - 2 ); // 1-bit width less overhead for the actual bit setting
                                                     // Note that this delay could be longer and everything would still work
      MAP_GPIO_setOutputLowOnPin(n->port,n->pin);

      DELAY_CYCLES( NS_TO_CYCLES( T1L ) - 10 ); // 1-bit gap less the overhead of the loop

    } else {             // 0-bit

      //cli();                                       // We need to protect this bit from being made wider by an interrupt
      MAP_Interrupt_disableMaster();

      MAP_GPIO_setOutputHighOnPin(n->port,n->pin );

      DELAY_CYCLES( NS_TO_CYCLES( T0H ) - 2 ); // 0-bit width less overhead
                                                    // **************************************************************************
                                                    // This line is really the only tight goldilocks timing in the whole program!
                                                    // **************************************************************************
      MAP_GPIO_setOutputLowOnPin(n->port,n->pin);

      //sei();
      MAP_Interrupt_enableMaster();

      DELAY_CYCLES( NS_TO_CYCLES( T0L ) - 10 ); // 0-bit gap less overhead of the loop

    }

    // Note that the inter-bit gap can be as long as you want as long as it doesn't exceed the 5us reset timeout (which is A long time)
    // Here I have been generous and not tried to squeeze the gap tight but instead erred on the side of lots of extra time.
    // This has thenice side effect of avoid glitches on very long strings becuase

}

void sendByte( unsigned char byte ) {

//    for( unsigned char bit = 0 ; bit &lt; 8 ; bit++ ) {
//
//      sendBit( bitRead( byte , 7 ) ); // Neopixel wants bit in highest-to-lowest order
//                                                     // so send highest bit (bit #7 in an 8-bit byte since they start at 0)
//      byte &lt;&lt;= 1; // and then shift left so bit 6 moves into 7, 5 moves into 6, etc
//
//    }
    uint8_t i =0;
    for(i=0;i<8;i++){
        sendBit(byte & 0x80);
        byte = byte << 1;
    }
}

/*

The following three functions are the public API:
  ledSetup() - set up the pin that is connected to the string. Call once at the beginning of the program.
  sendPixel( r , g , b ) - send a single pixel to the string. Call this once for each pixel in a frame.
  show() - latch the recently sent pixels on the LEDs . Call once per frame.
*/

// Set the specified pin up as digital out

void ledsetup() {

    MAP_GPIO_setOutputHighOnPin(n->port,n->pin);

}

void sendPixel( unsigned char r, unsigned char g , unsigned char b ) {

  sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
  sendByte(r);
  sendByte(b);

}

// Just wait long enough without sending any bots to cause the pixels to latch and display the last sent frame

void show() {
    DELAY_CYCLES( NS_TO_CYCLES(RES) );
}
void neopixel_init(_S_NEOPIXEL * neopixel){
    n = neopixel;
    MAP_GPIO_setAsOutputPin(n->port, n->pin);

}
#endif

#define T0H 5 // number of cycles at 48 MHz for 0 bit time high
#define T1H 10 // number of cycles at 48 MHz for 0 bit time high
#define TLD 9 // number of cycles at 48 MHz for bit time low
#define TLL 150 // number of cycles at 48 MHz for bit latch time low
#define SETBIT_LEDS (P2OUT |= BIT7) // set P3.6 to logic 1
#define CLEARBIT_LEDS (P2OUT &= ~BIT7) // set P3.6 to logic 0

typedef struct {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
    uint8_t whites;
}_S_LEDS;
_S_LEDS leds[NUM_LEDS];
void showStrip() { // Send colors to the strip and show them
    int i, j;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for (i = 0; i < 2; i++){ // send RGBW color intensity for every LED
        uint8_t rgb[4] = {128,255,0,0x00};
        for (j = 0; j < 4; j++){ // send green, then red, then blue, then white
            uint8_t mask = 0x80; // b1000000
            while(mask != 0) {
                if (rgb[j] & mask) { // most significant bit first
                    SETBIT_LEDS; // send timing pattern for a 1
                    _delay_cycles(T1H);
                }
                else {
                    SETBIT_LEDS; // send timing pattern for a 0
                    _delay_cycles(T0H);
                }
                CLEARBIT_LEDS;
                _delay_cycles(TLD);
                mask >>= 1; // check next bit
            }
        }
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
inline static void sendBit( bool bitVal ) {

    if ( bitVal ) {      // 1-bit

        SETBIT_LEDS;//MAP_GPIO_setOutputHighOnPin(n->port,n->pin);
        _delay_cycles(T1H);
        CLEARBIT_LEDS;
        _delay_cycles(TLD);
    } else {             // 0-bit

        SETBIT_LEDS; // send timing pattern for a 0
        _delay_cycles(T0H);
        CLEARBIT_LEDS;
        _delay_cycles(TLD);
    }
}
inline void sendByte( unsigned char byte ) {
        uint8_t i =0;
        for(i=0;i<8;i++){
            sendBit(byte & 0x80);
            byte = byte << 1;
        }
    }
void sendPixelOnce( unsigned char r, unsigned char g , unsigned char b , unsigned char w) {
  MAP_Interrupt_disableMaster(); // Disables interrupts while processing
  sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
  sendByte(r);
  sendByte(b);
  sendByte(w);
  _delay_cycles(TLL); // send latching low time (> 6000 ns)
   MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void sendPixel( unsigned char r, unsigned char g , unsigned char b , unsigned char w) {
  sendByte(g); // Neopixel wants colors in green-then-red-then-blue order
  sendByte(r);
  sendByte(b);
  //sendByte(w);
}
void sendStrip(_S_NEOPIXEL_LED *led, uint32_t delay_cycles){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0xff,0x00,0x00,0x00);
       // _delay_cycles(0); // send latching low time (> 6000 ns)
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
//#define XD   0



#define FADE_DELAY  19
#define SUCCESSIVE_DELAY    50




void led_test(){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0xff,0x00,0x00,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)

    _delay_cycles(16000000);
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0xff,0x00,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    _delay_cycles(16000000);

    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0x00,0xff,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    _delay_cycles(16000000);

    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0x00,0x00,0xff);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    _delay_cycles(16000000);

    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void fade_red_all(){
    uint8_t i = 0,j  =0;
    const uint16_t step = 2;
    uint8_t red  = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<254;i++){
        if(i<127){
           red += step;
        }else{
            red -= step;
        }
        for(j=0;j<NUM_LEDS;j++){
            sendPixel(red,0x00,0x00,0x00);
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(FADE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void fade_green_all(){
    uint8_t i = 0,j=0;
    const uint16_t step = 2;
    uint8_t green  = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<254;i++){
        if(i<127){
           green += step;
        }else{
            green -= step;
        }
        for(j=0;j<NUM_LEDS;j++){
            sendPixel(0x00,green,0x00,0x00);
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(FADE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}

void fade_blue_all(){
    uint8_t i = 0,j=0;
    const uint16_t step = 2;
    uint8_t blue  = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<254;i++){
        if(i<127){
           blue += step;
        }else{
            blue -= step;
        }
        for(j=0;j<NUM_LEDS;j++){
            sendPixel(0x00,0x00,blue,0x00);
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(FADE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
//void fade_white_all(){
//    uint8_t i = 0,j=0;
//    const uint16_t step = 2;
//    uint8_t white  = 0;
//    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
//    for(i=0;i<254;i++){
//        if(i<127){
//           white += step;
//        }else{
//            white -= step;
//        }
//        for(j=0;j<NUM_LEDS;j++){
//            sendPixel(0x00,0x00,0x00,white);
//        }
//        _delay_cycles(TLL); // send latching low time (> 6000 ns)
//        Delay1ms(FADE_DELAY);
//    }
//    MAP_Interrupt_enableMaster(); // re-enable interrupts
//}
void red_one_at_a_time(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i==j){
                sendPixel(intensity,0x00,0x00,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void green_one_at_a_time(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i==j){
                sendPixel(0x00,intensity,0x00,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void blue_one_at_a_time(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i==j){
                sendPixel(0x00,0x00,intensity,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
//void white_one_at_a_time(uint8_t intensity){
//    uint8_t i = 0,j=0;
//    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
//    for(j=0;j<NUM_LEDS;j++){
//        for(i=0;i<NUM_LEDS;i++){
//            if(i==j){
//                sendPixel(0x00,0x00,0x00,intensity);
//            }else{
//                sendPixel(0x00,0x00,0x00,0x00);
//            }
//        }
//        _delay_cycles(TLL); // send latching low time (> 6000 ns)
//        Delay1ms(SUCCESSIVE_DELAY);
//    }
//    MAP_Interrupt_enableMaster(); // re-enable interrupts
//}
void red_gradual_on(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i<=j){
                sendPixel(intensity,0x00,0x00,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void green_gradual_on(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i<=j){
                sendPixel(0x00,intensity,0x00,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void blue_gradual_on(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i<=j){
                sendPixel(0x00,0x00,intensity,0x00);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void white_gradual_on(uint8_t intensity){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i<=j){
                sendPixel(0x00,0x00,0x00,intensity);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void all_off(){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0x00,0x00,0x00);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    Delay1ms(50);
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void all_on(){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0xff,0x00,0x00,0x00);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    Delay1ms(50);
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void red_on_all_intensity(uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(intensity,0x00,0x00,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void green_on_all_intensity(uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,intensity,0x00,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void blue_on_all_intensity(uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0x00,intensity,0x00);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void white_on_all_intensity(uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(0x00,0x00,0x00,intensity);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void red_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        if(i == pixel_no){
            sendPixel(intensity,0x00,0x00,0x00);
        }else{
            sendPixel(0x00,0x00,0x00,0x00);
        }
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void green_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        if(i == pixel_no){
            sendPixel(0x00,intensity,0x00,0x00);
        }else{
            sendPixel(0x00,0x00,0x00,0x00);
        }
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void blue_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        if(i == pixel_no){
            sendPixel(0x00,0x00,intensity,0x00);
        }else{
            sendPixel(0x00,0x00,0x00,0x00);
        }
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void white_one_pixel_on_intensity(uint8_t pixel_no, uint8_t intensity){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        if(i == pixel_no){
            sendPixel(0x00,0x00,0x00,intensity);
        }else{
            sendPixel(0x00,0x00,0x00,0x00);
        }
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void one_pixel_on_intensity(uint8_t pixel_no, uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        if(i == pixel_no){
            sendPixel(r,g,b,w);
        }else{
            sendPixel(0x00,0x00,0x00,0x00);
        }
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts

}
void control_all_neopixel(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(r,g,b,w);
        //_delay_cycles(XD);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void control_each_neopixel(_S_NEOPIXEL_RGB_ARRAY *n){
    uint16_t i = 0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(i=0;i<NUM_LEDS;i++){
        sendPixel(n->r[i],n->g[i],n->b[i],n->w[i]);
    }
    _delay_cycles(TLL); // send latching low time (> 6000 ns)
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
void neopixel_one_on_at_a_time(uint8_t r, uint8_t g, uint8_t b, uint8_t w){
    uint8_t i = 0,j=0;
    MAP_Interrupt_disableMaster(); // Disables interrupts while processing
    for(j=0;j<NUM_LEDS;j++){
        for(i=0;i<NUM_LEDS;i++){
            if(i==j){
                sendPixel(r,g,b,w);
            }else{
                sendPixel(0x00,0x00,0x00,0x00);
            }
        }
        _delay_cycles(TLL); // send latching low time (> 6000 ns)
        Delay1ms(SUCCESSIVE_DELAY);
    }
    MAP_Interrupt_enableMaster(); // re-enable interrupts
}
