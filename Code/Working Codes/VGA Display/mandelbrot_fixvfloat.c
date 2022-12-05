/**
 * Hunter Adams (vha3@cornell.edu)
 * 
 * Mandelbrot set calculation and visualization
 * Uses PIO-assembly VGA driver.
 * 
 * Core 1 draws the bottom half of the set using floating point.
 * Core 0 draws the top half of the set using fixed point.
 * This illustrates the speed improvement of fixed point over floating point.
 * 
 * https://vanhunteradams.com/FixedPoint/FixedPoint.html
 * https://vanhunteradams.com/Pico/VGA/VGA.html
 *
 * HARDWARE CONNECTIONS
 *  - GPIO 16 ---> VGA Hsync
 *  - GPIO 17 ---> VGA Vsync
 *  - GPIO 18 ---> 330 ohm resistor ---> VGA Red
 *  - GPIO 19 ---> 330 ohm resistor ---> VGA Green
 *  - GPIO 20 ---> 330 ohm resistor ---> VGA Blue
 *  - RP2040 GND ---> VGA GND
 *
 * RESOURCES USED
 *  - PIO state machines 0, 1, and 2 on PIO instance 0
 *  - DMA channels 0 and 1
 *  - 153.6 kBytes of RAM (for pixel color data)
 *
 */
#include "vga_graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/adc.h"
#include "registers.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// Stuff for Mandelbrot ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// Fixed point data type
typedef signed int fix28 ;
#define multfix28(a,b) ((fix28)(((( signed long long)(a))*(( signed long long)(b)))>>28)) 
#define float2fix28(a) ((fix28)((a)*268435456.0f)) // 2^28
#define fix2float28(a) ((float)(a)/268435456.0f) 
#define int2fix28(a) ((a)<<28)
// the fixed point value 4
#define FOURfix28 0x40000000 
#define SIXTEENTHfix28 0x01000000
#define ONEfix28 0x10000000

// Maximum number of iterations
#define max_count 1000

#define LEFT_VERT 150
#define MID_VERT 290
#define RIGHT_VERT 430

#define LEFT_VERT_TILES 160
#define MID_VERT_TILES 300
#define RIGHT_VERT_TILES 440

#define RESTART_PIN 4
#define RESTART_PIN_REG ((volatile uint32_t *)(IO_BANK0_BASE + 0x010))

uint act_adc() {
    adc_select_input(0);
    uint adc_x_raw = adc_read();
    uint adc_x = 0;

    if (adc_x_raw > 2000 && adc_x_raw < 2060) {
        adc_x = 2048;
    } else {
        adc_x = adc_x_raw;
    }

    adc_x = (adc_x * 100) / 4095;

    /* printf("%d, %d\n", adc_x, adc_x_raw); */
    
    if (adc_x == 50){
        fillRect(MID_VERT,460,60,20,WHITE);
        fillRect(LEFT_VERT,460,60,20,0);
        fillRect(RIGHT_VERT,460,60,20,0);
    }
    else if (adc_x > 50) {
        fillRect(RIGHT_VERT,460,60,20,WHITE);
        fillRect(LEFT_VERT,460,60,20,0);
        fillRect(MID_VERT,460,60,20,0);
    }else if (adc_x < 50) {
        fillRect(LEFT_VERT,460,60,20,WHITE);
        fillRect(MID_VERT,460,60,20,0);
        fillRect(RIGHT_VERT,460,60,20,0);
    }
    sleep_ms(10);
    return adc_x;
}

void draw_fill_rect(short x, short y, short w, short h, char color, short inc_dec){
    fillRect(x,y,w,h,color);
    fillRect(x,y,w,inc_dec,0);
    fillRect(x,y+h,w,inc_dec,color);
    sleep_ms(10);
}

void update_score(uint score){
    fillRect(30,60,240,20,0);
    /* setCursor(30, 30); */
    /* setTextSize(3); */
    char str_score[3] = {'0', '0', '0'};
    str_score[2] = (score % 10) + '0';
    str_score[1] = ((score/10) % 10) + '0';
    str_score[0] = (((score/10)/10) % 10) + '0';
    drawChar(30, 60, str_score[0], WHITE, 0, 2);
    drawChar(45, 60, str_score[1], WHITE, 0, 2);
    drawChar(60, 60, str_score[2], WHITE, 0, 2);
}

int main() {

    gpio_init(RESTART_PIN);
    gpio_set_dir(RESTART_PIN, GPIO_IN);

    // Initialize stdio
    stdio_init_all();

    // Initialize VGA
    initVGA() ;

    /* int pattern_array[6] = {20, 80, 20, 120, 60, 20} */
    
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);

    uint blue_indx = 20, green_indx = 40, cyan_indx = 60, joystick_pos = 0;
    uint curr_score = 0, buttons_status = 0;

    drawChar(30, 30, 'S', WHITE, 0, 2);
    drawChar(45, 30, 'c', WHITE, 0, 2);
    drawChar(60, 30, 'o', WHITE, 0, 2);
    drawChar(75, 30, 'r', WHITE, 0, 2);
    drawChar(90, 30, 'e', WHITE, 0, 2);
    drawChar(105, 30, ':', WHITE, 0, 2);
    update_score(curr_score);

    sleep_ms(5000);

    while(true) {
        int x0=570;
        int y0=420;

        for (int16_t x=x0; x > 100; x-=50) {
           for (int16_t y=y0; y > 100; y-=50) {

        fillRect(x,y, 40,20, RED);
        fillCircle(x, y+10, 10, RED);
        
        
        
           }

        sleep_ms(1000);

           for (int16_t y=y0; y >100; y-=50) {

        fillRect(x,y, 40,20, BLACK);
        fillCircle(x, y+10, 10, BLACK);
        
           }
        sleep_ms(500);
        }

}

}
// x0=100;
//     y0=100;
//     sleep_ms(500);
//     for (int16_t x=x0; x < 640; x+=50) {
//         for (int16_t y=y0; y < 480; y+=50) {
//             for (int k=x;k<x+40;k++){
//           for (int j=y;j<y+20;j++){
//             drawPixel(k,j,RED); 
//           }
//         }
        
//         }
//         printf("Printed RED\n");
//        sleep_ms(500);

//         for (int16_t y=y0; y < 480; y+=50) {
//         for (int k=x;k<x+40;k++){
//           for (int j=y;j<y+20;j++){
//             drawPixel(k,j,BLACK); 
//           }
//         }

//         }
        
//         printf("Printed BLUE\n");
//         sleep_ms(500);
//     }
