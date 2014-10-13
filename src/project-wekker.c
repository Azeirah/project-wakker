/*
===============================================================================
 Name        : project-wekker.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <cr_section_macros.h>
#include "modules/I2c.h"


int main(void) {
	// 12/10/2014 14:50:xx, 24-hours mode, no AM/PM, clock HALT
	unsigned char initRTCCommands[8]  = {0x00, 0x40, 0x39, 0x21, 0x07, 0x12, 0x10, 0x14};
	// xx/xx/xxxx xx:xx:30 and disable clock halt to start the time!
	unsigned char initRTCCommands2[2] = {0x00, 0x30};
	unsigned char readCommand[1] = {0x00};
	unsigned char data[6];
	initI2C();
//	beginTransmission(8, initRTCCommands);
//	block(3500);
//	beginTransmission(2, initRTCCommands2);
//	block(2000);
	beginTransmission(1, readCommand);
	block(7000);
	receive(6);

	volatile static int i = 0 ;
    while(1) {
        i++;
    }
    return 0;
}
