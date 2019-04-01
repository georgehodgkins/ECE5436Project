#include <stdint.h>
#include "configurePin.h"

inline uint16_t* portAddr(enum PortLetterType portL) {
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
	return addr;
}
	
inline enum PortLetterType NumToLetterPort(uint8_t portN, uint8_t* pinN) {
	enum PortLetterType portL;
	switch (portN) {
		case 2:
		*pinN += 8;
		case 1:
		portL = A;
		break;
		case 4:
		*pinN += 8;
		case 3:
		portL = B;
		break;
		case 6:
		*pinN += 8;
		case 5:
		portL = C;
		break;
		case 8:
		*pinN += 8;
		case 7:
		portL = D;
		break;
		case 10:
		*pinN += 8;
		case 9:
		portL = E;
		break;
	}
	return portL;
}

//This function configures a pin
//inputs: enum inputs (defined in header) are self-explanatory, pinN is the pin number (0-16 for letter ports)
//afsel selects the alternate function with bits 0 and 1, corresponding to function select regs 0 & 1
void configurePinL(enum PortLetterType portL, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt) {
	uint16_t* addr = portAddr(portL);
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
void configurePin (uint8_t portN, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt) {
	enum PortLetterType portL = NumToLetterPort(portN, &pinN);
	configurePin(portL, pinN, afsel, dir, interrupt);
}

void pinOffL(enum PortLetterType portL, uint8_t pinN) {
	uint16_t* addr = portAddr(portL);
	//output reg is +0x02
	*(addr + 0x02) &= ~(0x1 << pinN);
}

void pinOff(int portN, uint8_t pinN) {
	enum PortLetterType portL = NumToLetterPort(portN, &pinN);
	pinOff(portN, pinN);
}

void pinOnL(enum PortLetterType portL, uint8_t pinN) {
	uint16_t* addr = portAddr(portL);
	//output reg is +0x02
	*(addr + 0x02) |= 0x1 << pinN;
}

void pinOn(int portN, uint8_t pinN) {
	enum PortLetterType portL = NumToLetterPort(portN, &pinN);
	pinOffL(portL, pinN);
}

void pinToggleL(enum PortLetterType portL, uint8_t pinN) {
	uint16_t* addr = portAddr(portL);
	//output reg is +0x02
	*(addr + 0x02) ^= 0x1 << pinN;
}

void pinToggle(int portN, uint8_t pinN) {
	enum PortLetterType portL = NumToLetterPort(portN, &pinN);
	pinToggleL(portL, pinN);
}