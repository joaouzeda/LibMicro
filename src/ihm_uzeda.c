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

// -----Libraries-------
#include "ihm_uzeda.h"


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
                        (gpio_set_level(DAT, 1));
                        (gpio_set_level(DAT, 0));
                __us(100);
                gpio_set_level(CLK, 1);
                __ms(1);
                gpio_set_level(CLK, 0);
                __us(100);
        }

        gpio_set_level(DAT, 1);
        gpio_set_level(LAT, 1);
        __ms(1);
        gpio_set_level(LAT, 0);
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

// --------------------------------------------------------------------------------------
// -----Write position-------
// escreve caractere na linha e coluna 
void slcd_wr_po(unsigned char chr, char row, char col){
        if(!row){
                slcd_cmd(0x80|col); //envia comando para posicionar o cursor na posição correta
                slcd_write(chr);
        }else{
                slcd_cmd(0xc0|col); //envia comando para posicionar o cursor na posição correta
                slcd_write(chr);    
        }
}


// --------------------------------------------------------------------------------------
// -----Write number-------
// escreve um numero na posição determinada
void slcd_number(unsigned long num, char row, char col){
        
        short no_zero = 0;
        char mil, dez, dem, cen, uni;  
        
        
        dem = (char) (num/10000);
        mil = (char) (num%10000/1000);
        cen = (char) (num%1000/100);
        dez = (char) (num%100/10);
        uni = (char) (num%10);

        if(!dem && !no_zero){
                slcd_wr_po(' ', row, col);
        }else{
                slcd_write(dem+0x30, row, col);
                no_zero = 1;
        }
        if(!mil && !no_zero){
                slcd_write(' ');
        }else{
                slcd_write(mil+0x30);
                no_zero = 1;
        }
        if(!cen && !no_zero){
                slcd_write(' ');
        }else{
                slcd_write(ced+0x30);
                no_zero = 1;

        }
        if(!dez && !no_zero){
                slcd_write(' ');
        }else{
                slcd_write(dez+0x30);
                no_zero = 1;

        }       
        slcd_write(uni+0x30);
} 

// --------------------------------------------------------------------------------------
// -----LED ON-------
// liga o led

void sled_on(void){
        sled = 1;
        slcd_cmd(0x06);

}

// --------------------------------------------------------------------------------------
// -----LED OFF-------
// desliga o led

void sled_off(void){
        sled = 0;
        slcd_cmd(0x06);
}

// --------------------------------------------------------------------------------------
// -----backlight ON-------
// liga o backlight

void sbacklight_on(void){
        sbli = 1;
        slcd_cmd(0x06);
}

// --------------------------------------------------------------------------------------
// -----backlight OFF-------
// desliga o backlight

void sbacklight_off(void){
        sbli = 0;
        slcd_cmd(0x06);
}

// --------------------------------------------------------------------------------------
// -----Keypad-------
// le o kaypad

char keypad(volatile unsigned char *port){

        static unsigned char flags= 0x00; 

        if(!(gpio_get_level(BT1)))       // botao 1 pressionado? 
                flags |= (1<<1); // se sim seta flag bit 1
        

        if ((gpio_get_level(BT1)) && (flags&(1<<1))){  // botao 1 solto e flag 1 setada? sim
                flags  &= ~(1<<1);           // limpa flag 1 
                __ms(50);               //anti bouncing
                return 1;               //  retorna 1 indicando botao 1 presionado 
        }
        
        if(!(gpio_get_level(BT2)))       // botao 2 pressionado? 
                flags |= (1<<2); // se sim seta flag bit 2
        

        if ((gpio_get_level(BT2)) && (flags&(1<<2))){  // botao 2 solto e flag 2 setada? sim
                flags  &= ~(1<<2);           // limpa flag 2
                __ms(50);               //anti bouncing
                return 2;               //  retorna 2 indicando botao 2 presionado 
        }
        if(!(gpio_get_level(BT3)))       // botao 3 pressionado? 
                flags |= (1<<3); // se sim seta flag bit 3


        if ((gpio_get_level(BT3)) && (flags&(1<<3))){  // botao 3 solto e flag 3 setada? sim
                flags  &= ~(1<<3);           // limpa flag 3 
                __ms(50);               //anti bouncing
                return 3;               //  retorna 3 indicando botao 3 presionado 
        }
        if(!(gpio_get_level(BT4)))       // botao 4 pressionado? 
                flags |= (1<<4); // se sim seta flag bit 4
        

        if ((gpio_get_level(BT4)) && (flags&(1<<4))){  // botao 4 solto e flag 4 setada? sim
                flags  &= ~(1<<4);           // limpa flag 4
                __ms(50);               //anti bouncing
                return 4;               //  retorna 4 indicando botao 4 presionado 
        }
        return 0;
}


// --------------------------------------------------------------------------------------
// -----ms  -------
// escreve o chr na posicão atual do lcd

void __ms(unsigned long t){
        register unsigned long i;

        for(i = 0; i < t; i++)
                usleep(1000);
}

// --------------------------------------------------------------------------



//-------------------------------End----------------------------------------