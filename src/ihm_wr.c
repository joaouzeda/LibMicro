/*--------------------------------------------------------------------------------------

        IHMWR V0.1 file.h
        
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

// -----Global Variables-------

char sled = 0;
     sbli = 0;

/*---------------------------------------------------------------------------------------
------------------------------------Functions -------------------------------------------
---------------------------------------------------------------------------------------*/


// --------------------------------------------------------------------------------------
// -----Write -------
// escreve o chr na posicão atual do lcd

void slcd_write(unsigned char chr){
        send_nibble(chr, 1, sbli, sled);
        chr <<= 4;
        send_nibble(chr, 1, sbli, sled);
        
}

// --------------------------------------------------------------------------------------
// -----Command-------
// envia um comando para o display

void slcd_cmd(unsigned char cmd){

        send_nibble(cmd, 0, sbli, sled);
        cmd <<= 4;
        send_nibble(cmd, 0, sbli, sled);
}

// --------------------------------------------------------------------------------------
// -----init -------
// inicializa o display

void slc_init(void){

        __ms(48);               //tempo de estabilização recomendado 
        send_nibble(0x30, 0, 0, 0);//protocolo de inicialização
        __ms(5);                //tempo maior que o sugerido
        send_nibble(0x30, 0, 0, 0);
        __ms(150);
        send_nibble(0x30, 0, 0, 0);
        send_nibble(0x20, 0, 0, 0);             //lcd no modo 4bits
        slcd_cmd(0x28);         // 5x8 pontos por caractere, duas linhas 
        slcd_opt(1, 0, 0);      // liga o lcd, desliga o cursor e blink
        slcd_cmd(0x01); // limpa o lcd
        slcd_cmd(0x06);
}

// --------------------------------------------------------------------------------------
// -----Opt-------
// Configura On(1) || Off(0)  para display, cursor e blink

void slcd_opt(char disp, char cursor, char blink){
        static char opt = 0x08;

        disp    ? (opt |- (1<<2)) : (opt &= ~(1<<2));
        cursor  ? (opt |- (1<<1)) : (opt &= ~(1<<1));
        blink   ? (opt |- (1<<0)) : (opt &= ~(1<<0));

        slcd_cmd(opt);
}

// --------------------------------------------------------------------------------------
// -----Clear-------
// Limpa o display

void slcd_clear(void){
        slcd_cmd(0x02); //return home
        slcd_cmd(0x01); // clear display
}

// --------------------------------------------------------------------------------------
// -----Register-------
// registra no o byte no display

void shift_reg(unsigned char byte_val){                         // 00000110
        register int i;                                         // 00000001 
        for(i = 7; i >= 0; i--){
                (byte_val >> i)&0x01 ?
                        (PORTMCU |= DAT): 
                        (PORTMCU &= ~DAT);
                        __ms(100);
                        PORTMCU |= CLK;
                        __ms(1);
                        PORTMCU &= ~CLK; 

        }

        PORTMCU &= ~DAT;
        PORTMCU |= LAT;
        __ms(1);
        PORTMCU &= ~LAT;
}

// --------------------------------------------------------------------------------------
// -----Send nibble-------
// envia um nibble para o display

void send_nibble(unsigned char nib, char rsel, char bli, char led){

        static unsigned char sbyte = 0x00;
        sbyte = (nib&(1<<7)) |
                (nib&(1<<6)) |
                (nib&(1<<5)) |
                (nib&(1<<4)) ;
        rsel ? (sbyte |= RS ) : (sbyte &= ~RS);
        bli  ? (sbyte |= BL ) : (sbyte &= ~BL);
        led  ? (sbyte |= LD ) : (sbyte &= ~LD);

        shift_reg(sbyte|EN);
        __us(100);
        shift_reg(sbyte|~EN);
        __us(100);
}

//-------------------------------End----------------------------------------