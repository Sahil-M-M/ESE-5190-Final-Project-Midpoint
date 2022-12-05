#include <stdint.h>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "adc.h"
#include "hardware/pwm.h"
#include "st7735_80x160/my_lcd.h"

static void error_blink(uint led, int count)
{
    while (true) {
        for (int i = 0; i < count; i++) {
            gpio_put(led, 1);
            sleep_ms(250);
            gpio_put(led, 0);
            sleep_ms(250);
        }
        gpio_put(led, 0);
        sleep_ms(500);
        break;
    }
}

void testtexts()
{
    LCD_Clear(BLACK);
    LCD_ShowString(24,  0*16, (u8 *) "Hello World", WHITE);
    LCD_ShowString(24,  1*16, (u8 *) "Hello World", GRAY);
    LCD_ShowString(24,  2*16, (u8 *) "Hello World", RED);
    LCD_ShowString(24,  3*16, (u8 *) "Hello World", GREEN);
    LCD_ShowString(24,  4*16, (u8 *) "Hello World", BLUE);
}

void testoverlaytexts()
{
    LCD_ShowStringLn(0, 0*16, 0*8, 17*8, "Raspberry Pi Pico", 1, BLACK);
    LCD_ShowStringLn(0, 2*16, 0*8, 17*8, "Raspberry Pi Pico", 1, LIGHTBLUE);
    LCD_ShowStringLn(0, 4*16, 0*8, 17*8, "Raspberry Pi Pico", 1, GRAYBLUE);
}

void testscrolltexts(int i, int bl_val, u8 rotation)
{
    char str[16];
    static uint16_t sft0 = 0;
    static uint16_t sft1 = 0;

    LCD_Scroll_ShowString(0, 0*16, 0*16, LCD_W()-1, "Raspberry Pi Pico is a tiny, fast, and versatile board built using RP2040.", 0, GRED, &sft0, i);
    LCD_Scroll_ShowString(0, 2*16, 0*16, LCD_W()-1, "RP2040 features a dual-core Arm Cortex-M0+ processor with 264KB internal RAM and support for up to 16MB of off-chip Flash.", 0, GBLUE, &sft1, i);
    sprintf(str, "Rot: %d", (int) rotation);
    LCD_ShowString(0,  3*16, (u8 *) str, WHITE);
    sprintf(str, "BL: %3d", bl_val);
    LCD_ShowString(0,  4*16, (u8 *) str, WHITE);

}

void testpoints(uint16_t color1, uint16_t color2)
{
    LCD_Clear(BLACK);
    for (int16_t y=4; y < LCD_H(); y+=8) {
        for (int16_t x=4; x < LCD_W(); x+=8) {
            if (x == 4 || y == 4 || x >= LCD_W()-8 || y >= LCD_H()-8) {
                LCD_DrawPoint_big(x, y, color2);
            } else {
                LCD_DrawPoint(x, y, color1);
            }
        }
    }
}

void testlines(uint16_t color)
{
    LCD_Clear(BLACK);
    for (int16_t x=0; x < LCD_W(); x+=6) {
        LCD_DrawLine(0, 0, x, LCD_H()-1, color);
        sleep_ms(0);
    }
    for (int16_t y=0; y < LCD_H(); y+=6) {
        LCD_DrawLine(0, 0, LCD_W()-1, y, color);
        sleep_ms(0);
    }

    LCD_Clear(BLACK);
    for (int16_t x=0; x < LCD_W(); x+=6) {
        LCD_DrawLine(LCD_W()-1, 0, x, LCD_H()-1, color);
        sleep_ms(0);
    }
    for (int16_t y=0; y < LCD_H(); y+=6) {
        LCD_DrawLine(LCD_W()-1, 0, 0, y, color);
        sleep_ms(0);
    }

    LCD_Clear(BLACK);
    for (int16_t x=0; x < LCD_W(); x+=6) {
        LCD_DrawLine(0, LCD_H()-1, x, 0, color);
        sleep_ms(0);
    }
    for (int16_t y=0; y < LCD_H(); y+=6) {
        LCD_DrawLine(0, LCD_H()-1, LCD_W()-1, y, color);
        sleep_ms(0);
    }

    LCD_Clear(BLACK);
    for (int16_t x=0; x < LCD_W(); x+=6) {
        LCD_DrawLine(LCD_W()-1, LCD_H()-1, x, 0, color);
        sleep_ms(0);
    }
    for (int16_t y=0; y < LCD_H(); y+=6) {
        LCD_DrawLine(LCD_W()-1, LCD_H()-1, 0, y, color);
        sleep_ms(0);
    }
}

void testdrawrects(uint16_t color)
{
    LCD_Clear(BLACK);
    for (int16_t x=0; x < LCD_W(); x+=6) {
        LCD_DrawRectangle(LCD_W()/2 -x/2, LCD_H()/2 -x*LCD_H()/2/LCD_W(), LCD_W()/2 -x/2 + x, LCD_H()/2 -x*LCD_H()/2/LCD_W() + x*LCD_H()/LCD_W(), color);
    }
}

void testfillrects(uint16_t color1, uint16_t color2)
{
    LCD_Clear(BLACK);
    for (int16_t x=LCD_W()-1; x > 8; x-=8) {
        LCD_Fill(LCD_W()/2 -x/2, LCD_H()/2 -x*LCD_H()/2/LCD_W(), LCD_W()/2 -x/2 + x, LCD_H()/2 -x*LCD_H()/2/LCD_W() + x*LCD_H()/LCD_W(), color1);
        LCD_DrawRectangle(LCD_W()/2 -x/2, LCD_H()/2 -x*LCD_H()/2/LCD_W(), LCD_W()/2 -x/2 + x, LCD_H()/2 -x*LCD_H()/2/LCD_W() + x*LCD_H()/LCD_W(), color2);
    }
}

void testdrawcircles(uint8_t radius, uint16_t color1, uint16_t color2)
{
    LCD_Clear(BLACK);
    for (int16_t x=5; x < LCD_W(); x+=15) {
        for (int16_t y=5; y < LCD_H(); y+=20) {
            Draw_Circle(x, y, radius, color1);
        }
        sleep_ms(500);
        LCD_Clear(BLACK);
    }
}

void testdrawcircles1(uint8_t x, uint16_t y, uint16_t color)
{
   int radius=0;
            LCD_Clear(BLACK);

            Draw_Circle(x, y, radius, color);

            sleep_ms(100);
            LCD_Clear(BLACK);
            
        

}

void testdrawcircles3(uint8_t *x_pos, uint16_t *y_pos, uint16_t color1)
{
    
    const uint adc_max = (1 << 12) - 1;
    const uint bar_width = 40;
   int radius=0;
           adc_select_input(0);
        uint adc_x_raw = adc_read();
        adc_select_input(1);
        uint adc_y_raw = adc_read();

        uint bar_x_pos = adc_x_raw * bar_width / adc_max;
        uint bar_y_pos = adc_y_raw * bar_width / adc_max;
        printf("X: %d\n",bar_x_pos);
        printf("Y: %d\n",bar_y_pos);
        if (bar_x_pos<15){
            printf("X+\n");
            *x_pos-=5;}
        if (bar_x_pos>25){
            *x_pos+=5;
            printf("X-\n");}
        if (bar_y_pos<15){
            *y_pos+=5;
            printf("Y+\n");}
        if (bar_y_pos>25){
            *y_pos-=5;
            printf("Y-\n");}

 
         Draw_Circle1(*x_pos, *y_pos, radius, color1);
         sleep_ms(25);
        LCD_Clear_jet(*x_pos, *y_pos, BLACK)    ;     
        

        
        

}

void testdrawcircles4(uint8_t radius, uint16_t color1, uint16_t color2)
{
    uint x_pos=140;
    uint y_pos=50;

    for (int16_t x=5; x < LCD_W(); x+=15) {
        for (int16_t y=5; y < LCD_H(); y+=20) {
            Draw_Circle(x, y, radius, color1);
            testdrawcircles3(&x_pos, &y_pos , YELLOW);
        }
        sleep_ms(500);
        LCD_Clear(BLACK);
    }
}

int main()
{
    const uint32_t TimeStay = 1000;
    u8 rotation = 2;

    stdio_init_all();

    // Initialise UART 0
    uart_init(uart0, 115200);
    // Set the GPIO pin mux to the UART - 0 is TX, 1 is RX
    gpio_set_function(0, GPIO_FUNC_UART);
    gpio_set_function(1, GPIO_FUNC_UART);

    // LED
    const uint32_t LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // BackLight PWM (125MHz / 65536 / 4 = 476.84 Hz)
    gpio_set_function(PIN_LCD_BLK, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_init(slice_num, &config, true);
    int bl_val = 196;
    // Square bl_val to make brightness appear more linear
    pwm_set_gpio_level(PIN_LCD_BLK, bl_val * bl_val);

    // Discard any input from Serial
    while (uart_is_readable(uart0)) {
        uart_getc(uart0);
    }
    printf("\n");
    //printf("Type any character to start\n");
    //while (!uart_is_readable_within_us(uart0, 1000));

    printf("========================\n");
    printf("== pico_st7735_80x160 ==\n");
    printf("========================\n");
    printf("-: ncrease back light\n");
    printf("=: decrease back light\n");
    printf("r: repeat demo\n");

    LCD_Init();
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(26);
    adc_gpio_init(27);
    uint x_pos=0;
    uint y_pos=0;
    const uint adc_max = (1 << 12) - 1;
    const uint bar_width = 40;
    while (1) {
        // LCD_SetRotation(rotation);

        LCD_Clear(BLACK);
        BACK_COLOR=BLACK;

        // testtexts();
        // sleep_ms(TimeStay);
        
        // testpoints(GREEN, RED);
        // sleep_ms(TimeStay);

        // testlines(YELLOW);
        // sleep_ms(TimeStay);

        // testdrawrects(GREEN);
        // sleep_ms(TimeStay);
        
        // testfillrects(YELLOW, MAGENTA);
        // sleep_ms(TimeStay);


        // testdrawcircles1(x_pos, y_pos , BLUE);
        // sleep_ms(10);

        // testoverlaytexts();
        // sleep_ms(TimeStay);
        testdrawcircles4(4, BLUE, GREEN);
        LCD_Clear(BLACK);
        // while(!stdio_usb_connected());
        // printf("Starting PIO I2C ADPS9960 Interface\n");
        // Demo end (Text Scroll & Serial command)
        // for (int i = 0; i < 512; i++) {

        //     int c = getchar_timeout_us(0);
        //     switch (c) {
        //         case '-':
        //             if (bl_val) bl_val--;
        //             pwm_set_gpio_level(PIN_LCD_BLK, bl_val * bl_val);
        //             printf("-");
        //             break;
        //         case '=':
        //         case '+':
        //             if (bl_val < 255) bl_val++;
        //             pwm_set_gpio_level(PIN_LCD_BLK, bl_val * bl_val);
        //             printf("+");
        //             break;
        //         default:
        //             break;
        //     }
        //     //if (c == 'r') break; // repeat demo
        //     testscrolltexts(i, bl_val, rotation);
        //     sleep_ms(2);
        // }
        // rotation = (rotation + 1) & 0x3;
    }
    printf("\nDone\n");

    return 0;
}
