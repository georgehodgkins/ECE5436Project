#ifndef CONFPIN_H_
#define CONFPIN_H_

enum DirectionType {_IN, _OUT};
enum InterruptEnableType {INTERRUPT_RISING, INTERRUPT_FALLING, NO_INTERRUPT};
enum PortLetterType {A, B, C, D, E};

void configurePinL(enum PortLetterType portL, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt);

void configurePin (uint8_t portN, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt);

void pinOffL(enum PortLetterType portL, uint8_t pinN);

void pinOff(int portN, uint8_t pinN);

void pinOnL(enum PortLetterType portL, uint8_t pinN);

void pinOn(int portN, uint8_t pinN);

void pinToggleL(enum PortLetterType portL, uint8_t pinN);

void pinToggle(int portN, uint8_t pinN);

uint16_t* portAddr(enum PortLetterType portL);
	
enum PortLetterType NumToLetterPort(uint8_t portN, uint8_t* pinN);
#endif
