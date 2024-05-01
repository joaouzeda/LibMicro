
/*--------------------------------------------------------------------------------------

    IHMWR V0.2 main.c
        
    Serial interface with 7hc595
    lcd 16x2 with backlight 
    auxiliarty led
    4 buttons 

    compiler: VsCode 1.88.1
     MCU: ESP32
    Author: João  G. Uzêda
    date: 2024, March

-----------------------------------------------------------------------------------------*/

// --------------------------------------------------------------------------------------
// -----Libraries-------
#include "ihm_wr.h"

// --------------------------------------------------------------------------------------
// -----Functions prototypes-------
void keypad_test(void); //imprime o valor da tecla pressionada 


//-------------------------Main Function------------------------------------

unsigned long test = 12345;

void main(){

    CMCON = 0X07;
    TRISB = 0XF8;

    slc_init();
    slcd_clear();

    sled_on();
    sbacklight_on();

    slcd_wr_po('H', 0, 0);
    slcd_write('e');
    slcd_write('l');
    slcd_write('l');
    slcd_write('o');
    slcd_write('!');

    delay_ms(2000);

    sled_off();
    sbacklight_off();

    slcd_number(test, 1, 0);    

    while (1){
        test++;
        __ms(500);
        slcd_number(test, 1, 0);

        keypad_test();
    }

}

// ----------------------------Functions prototypes ------------------------------------

// --------------------------------------------------------------------------------------
// -----keypad -------
//  imprime o valor da tecla pressionada 

void keypad_test(void){

    static char kb = 0;

    kb = keypad(&PORTB);

    switch (KB)
    {
    case 1;
        slcd_wr_po ('1', 1, 10)
        break;
    case 2;
        slcd_wr_po ('2', 1, 10)
        break;
    case 3;
        slcd_wr_po ('3', 1, 10)
        break;
     case 4;
        slcd_wr_po ('4', 1, 10)    
    }
}









































//-------------------------------End----------------------------------------