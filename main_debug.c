#include "oled_menu.h"
#include "pinball_statemachine.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

int main(){

	printf("pinball\n");

	InitPinballGame();
	
	while(1){
		PinballGameProcess();
		sleep(3);

		
	}







	return 0;
}