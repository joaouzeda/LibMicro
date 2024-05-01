/*--------------------------------------------------------------------------------------

    IHMWR V0.2 file.h
        
    Serial interface with 7hc595
    lcd 16x2 with backlight 
    auxiliarty led
    4 buttons 

    compiler: VsCode 1.88.1
    MCU: ESP32
    Author: João  G. Uzêda
    date: 2024, March

-----------------------------------------------------------------------------------------*/

#ifndef IHM_WR_H

#define IHM_WR_H

// --------------------------------------------------------------------------------------

// -----hardware mapping-------

#define CLK     (1<<0)
#define DAT     (1<<1)
#define LAT     (1<<2)
#define BT1     (1<<4)
#define BT2     (1<<5)
#define BT3     (1<<6)
#define BT4     (1<<7)
#define PORTMCU PORTBs

// --------------------------------------------------------------------------------------

// -----Macros-------

#define __ms(t) delay_ms(t)
#define __us(t) delay_us(t)

// --------------------------------------------------------------------------------------

// -----Constants-------

#define LD  (1<<0)
#define BL  (1<<1)
#define RS  (1<<2)
#define EN  (1<<3)

// --------------------------------------------------------------------------------------

// -----Functions-------

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