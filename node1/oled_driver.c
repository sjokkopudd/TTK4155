#define F_CPU 4915200

#include "oled_driver.h"

#include <util/delay.h>

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "fonts.h"
#include "graphic.h"

#define FONT_WIDTH 8
#define COLUMN_COUNT 128
#define PAGE_COUNT 8


static volatile char *oled_command = (char*) 0x1000;
static volatile char *oled_data = (char*) 0x1200;

static void oled_write_char(char data, FILE * _not_used);
static uint8_t page_zero = 0xb0;

static FILE oled_stream = FDEV_SETUP_STREAM (oled_write_char, NULL, _FDEV_SETUP_WRITE);

static void write_c(uint8_t command){
	*oled_command = command;
    
}

static void write_d(char data){

  *oled_data = data;

}



void oled_init(void){

  write_c(0xae);        //display  off  
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
  
  //set  normal  display  
  write_c(0xa6);        
  
  // display  on  
  write_c(0xaf);        

	//set page start address 
  write_c(0xb0);  

  //set column start address (lower)
  write_c(0x00);

  //set column start address (upper)
  write_c(0x10);

  oled_reset();


}


//clear screen and return to initial position
void oled_reset(void){

  for (int i = 0; i < PAGE_COUNT; ++i)
  {
    oled_clear_line(i);
  }

  oled_home();

}

//return to initial position 
void oled_home(void){
  oled_pos(0,0);
}


//go to specified line 
void oled_goto_line(uint8_t line){
  //check borders
  if(line >= PAGE_COUNT || line < 0){

    return;

  }
  //set cursor to corresponding line
  write_c(page_zero + line);

  
}

//go to specified column
void oled_goto_column(uint8_t column){
  if(column >= COLUMN_COUNT || column < 0){

    return; 
  }
  //set lower and upper address for column
  uint8_t address_low = column % 16;
  uint8_t address_high = column / 16;

  write_c(address_low);
  write_c(16 + address_high);

}


//clear the specified line at the screen
void oled_clear_line(uint8_t line){
  if(line >= PAGE_COUNT || line < 0){
    return;
  }

  oled_goto_line(line);

  for (int i = 0; i < COLUMN_COUNT; ++i)
  {
    //delete pixel
    oled_print(" ");
  }

  //go to begin of the line again
  oled_goto_column(0);
}

//set cursor to specified row and column
void oled_pos(uint8_t row, uint8_t column){
  oled_goto_line(row);
  oled_goto_column(column); 
}

//internal function that is called from printf
static void oled_write_char(char data, FILE * _not_used){
  (void) _not_used;

  //write one character from font8x8
  for (int i = 0; i < FONT_WIDTH; i++) {
    write_d(pgm_read_byte(&font8[data-' '][i]));
  }
}

//prints string to oled 
void oled_print(const char* data){
  
  stdout = &oled_stream;
  printf(data); 
}


void oled_write_screen(){
  //set the segment orientation: SEG0 is col 0
  
 
  for (int j = 0; j < 8; j++){
    oled_goto_line(j);
    for (int i = 0; i < 128; i++){
      write_d(pgm_read_byte(&pinball_screen[j][i]));
    }
  }
}

void oled_print_trophy(){
  //set the segment orientation: SEG0 is col 0
  oled_pos(0,47);
  for (int j = 0; j < 4; j++){
    oled_goto_line(j+3);
    for (int i = 0; i < 32; i++){
      write_d(pgm_read_byte(&trophy[j][i]));
    }
    oled_goto_column(47);
  }
}


void oled_game_over(){
  for(int i = 0; i < 2 ; i++){
    oled_goto_line(i);
    for(int j = 0; j < 128; j++){
         write_d(pgm_read_byte(&game_over[i][j]));
    }
  }
}

void oled_print_firework(int frame){
  oled_pos(3,30);
  for (int i = 0; i < 8; i++){
    write_d(pgm_read_byte(&firework[frame][i]));
  }

  oled_pos(3,90);
  for (int i = 0; i < 8; i++){
    write_d(pgm_read_byte(&firework[frame][i]));
  }

  oled_pos(4,80);
  for (int i = 0; i < 8; i++){
    write_d(pgm_read_byte(&firework[frame][i]));
  }

  oled_pos(2,60);
  for (int i = 0; i < 8; i++){
    write_d(pgm_read_byte(&firework[frame][i]));
  }
}

