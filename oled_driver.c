#define F_CPU 4915200

#include "oled_driver.h"

#include <util/delay.h>

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define FONT_WIDTH 8

volatile char *oled_command = (char*) 0x1000;
volatile char *oled_data = (char*) 0x1200;

static void write_c(uint8_t command){
	*oled_command = command;
    
}

static void write_d(char data){

      volatile char *oled_data = (char*) 0x1200;

      oled_data[0x00] = data;

}



void oled_init(void){
	
write_c(0xae);        //  display  off  
      write_c(0xa1);        //segment  remap  
      write_c(0xda);        //common  pads  hardware:  alternative  
      write_c(0x12);  
      write_c(0xc8);        //common output scan direction:com63~com0 
      write_c(0xa8);        //multiplex  ration  mode:63  
      write_c(0x3f);  
      write_c(0xd5);        //display  divide ratio/osc. freq. mode 
      write_c(0x80);          
      write_c(0x81);        //contrast  control    
      write_c(0x50);          
      write_c(0xd9);        //set  pre-charge  period  
      write_c(0x21);          
      write_c(0x20);        //Set  Memory  Addressing  Mode  
      write_c(0x02);          
      write_c(0xdb);        //VCOM  deselect  level  mode  
      write_c(0x30);          
      write_c(0xad);        //master  configuration    
      write_c(0x00);          
      write_c(0xa4);        //out  follows  RAM  content  
      write_c(0xa6);        //set  normal  display  
      write_c(0xaf);        //  display  on  
      write_c(0xa5);

	//set start address.....

}



void oled_reset(void){


}

void oled_home(void){

}

void oled_goto_line(uint8_t line){

}

void OLED_clear_line(uint8_t line){

}

void oled_pos(uint8_t row, uint8_t column){

}

void oled_write_data(char data){
     //write one character from font8x8
     for (int i = 0; i < FONT_WIDTH; i++) {
		*oled_data = pgm_read_byte(&font[data-' '][i]);
     }
}

void oled_print(char* data){

}



void oled_set_brightness(uint8_t lvl){

}
