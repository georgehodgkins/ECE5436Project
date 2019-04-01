#include <stdint.h>
#include "configurePin.h"

//This function configures a pin
//inputs: enum inputs (defined in header) are self-explanatory, pinN is the pin number (0-16 for letter ports)
//afsel selects the alternate function with bits 0 and 1, corresponding to function select regs 0 & 1
void configurePin(enum PortLetterType portL, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt) {
	uint16_t offset;
	switch (portL) {
		case(A):
		offset =  0x0;
		break;
		case (B):
		offset =  0x20;
		break;
		case (C):
		offset = 0x40;
		break;
		case (D):
		offset = 0x60;
		break;
		case (E):
		offset = 0x80;
		break;
	}
	//addr is the base address of the port
	uint16_t* addr = (uint16_t*) 0x40004C00;
	addr += offset;
	//direction reg is at +0x04
	if (dir == IN) {
		*(addr+ 0x04) &= ~(0x1 << pinN);
		//set PUR or PDR?
	} else { //dir == OUT
		*(addr + 0x04) |= 0x1 << pinN;
	}
	//function select regs are at + 0x0A, 0x0C
	*(addr + 0x0A) |= (afsel & 0x1) << pinN;
	*(addr + 0x0C) |= (afsel & 0x2) << pinN;
	//interrupt enable reg is at +0x1A
	//interrupt edge select reg is at +0x18
	if (interrupt == NO_INTERRUPT) {
		*(addr + 0x1A) &= ~(0x1 << pinN);
	} else {
		if (interrupt == INTERRUPT_RISING) {
			*(addr + 0x18) &= ~(0x1 << pinN);
		} else { //INTERRUPT_FALLING
			*(addr + 0x18) |= 0x1 << pinN;
		}
		*(addr + 0x1A) |= 0x1 << pinN;
	}
}

//inputs to this function are the same, except first argument is a port number instead of a letter
//and obv pinN can only be 0-8
void configurePinbyNum (uint8_t portN, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt) {
	switch (portN) {
		//odd-numbered ports are low side of the corresponding letter, even is high side
		case 1:
		configurePin(A, pinN, afsel, dir, interrupt);
		break;
		case 2:
		configurePin(A, pinN+8, afsel, dir, interrupt);
		break;
		case 3:
		configurePin(B, pinN, afsel, dir, interrupt);
		break;
		case 4:
		configurePin(B, pinN+8, afsel, dir, interrupt);
		break;
		case 5:
		configurePin(C, pinN, afsel, dir, interrupt);
		break;
		case 6:
		configurePin(C, pinN+8, afsel, dir, interrupt);
		break;
		case 7:
		configurePin(D, pinN, afsel, dir, interrupt);
		break;
		case 8:
		configurePin(D, pinN+8, afsel, dir, interrupt);
		break;
		case 9:
		configurePin(E, pinN, afsel, dir, interrupt);
		break;
		case 10:
		configurePin(E, pinN+8, afsel, dir, interrupt);
		break;
	}
}
		