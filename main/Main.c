
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

    char tests = 0;

    CMCON = 0X07;
    TRISB = 0XF8;

    slc_init();
    slcd_opt(1, 1, 1);

    slcd_clear();

    sled_on();
    sbacklight_on();

    slcd_wr_po('U', 0, 4);
    slcd_write('Z');
    slcd_write('E');
    slcd_write('A');
    slcd_write('!');

    delay_ms(1000);

    select(&PORTB, &tests);

    while (1){
        if(tests)
            number_test();
        else     
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

// --------------------------------------------------------------------------------------
// -----Number test -------
//  testa a escrita do display de um numero inteiro

void number_test(void){
    static unsigned val = 0;
    
    slcd_number(val++, 1, 6);
    delay_ms(100);

}

// --------------------------------------------------------------------------------------
// ----- -------
//  testa a escrita do display de um numero inteiro

void select(volatile unsigned char *port, char *test_port){

    if(*port&BT1){
        slcd_wr_po('K', 1, 4);
        slcd_write('B');
        slcd_write(':');
        *test_port = 0;
    }else{
        slcd_wr_po('N', 1, 4);
        slcd_write(':');
        *test_port = 1;
    }

    (*port&BT2) ? sled_off() : sled_on();
    (*port&BT3) ? sbacklight_off() : sbacklight_on();


}





































//-------------------------------End----------------------------------------