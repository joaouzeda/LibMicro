/*--------------------------------------------------------------------------------------

    IHMWR V0.2 file.h
        
    Serial interface with 7hc595
    lcd 16x2 with backlight 
    auxiliarty led
    4 buttons 

    compiler:   VsCode 1.88.1
    MCU:        ESP32
    Author:     João  G. Uzêda
    date:       2024, March

-----------------------------------------------------------------------------------------*/

// --------------------------------------------------------------------------------------
// -----Librarys-------
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "driver/gpio.h"
#include "esp_system.h"

#ifndef IHM_UZEDA_H

#define IHM_UZEDA_H

// --------------------------------------------------------------------------------------
// -----hardware mapping-------

#define CLK     GPIO_NUM_18
#define DAT     GPIO_NUM_19
#define LAT     GPIO_NUM_21
#define BT1     GPIO_NUM_4
#define BT2     GPIO_NUM_32
#define BT3     GPIO_NUM_5
#define BT4     GPIO_NUM_34
//#define PORTMCU PORTBs

// --------------------------------------------------------------------------------------
// -----Macros-------

//#define __ms(t) delay_ms(t)
#define __us(t) usleep(t)

// --------------------------------------------------------------------------------------
// -----Constants-------

#define LD  (1<<0)
#define BL  (1<<1)
#define RS  (1<<2)
#define EN  (1<<3)

// --------------------------------------------------------------------------------------
// -----Functions-------
void _ms(unsigned long t);
void slcd_write(unsigned char chr);                     // write characters on corrent posision
void slcd_cmd(unsigned char cmd);                       // send commands to lcd
void slc_init(void);                                    // init lcd
void slcd_clear(void);                                  // clear lcd
void shift_reg(unsigned char byte_val);                 // shift register control
void send_nibble(unsigned char nib,                     // send each nibble 
                char rsel,
                char bli,
                char led);

void slcd_opt(char disp, char cursor, 
                         char blink); // configura  on (1) off(0) para display, cursor e blink
void slcd_wr_po(unsigned char chr, char row, char col); //escreve na posição determinada
void slcd_number(unsigned long num, char row, char col);    //escreve um numero na pisição determinada
void sled_on(void); // liga o led 
void sled_off(void); //desliga o led
void sbacklight_on(void); // ligao backlight
void sbacklight_off(void); //desliga o backlight
char keypad(volatile unsigned char *port); //le o kaypad





#endif 

//-------------------------------End----------------------------------------