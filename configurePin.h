enum DirectionType {IN, OUT};
enum InterruptEnableType {INTERRUPT_RISING, INTERRUPT_FALLING, NO_INTERRUPT};
enum PortLetterType {A, B, C, D, E};

void configurePin(enum PortLetterType portL, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt);

void configurePinbyNum (uint8_t portN, uint8_t pinN, uint8_t afsel, enum DirectionType dir, enum InterruptEnableType interrupt);