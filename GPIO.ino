static boolean pins[80];

void GPIO_Init() {
  for(int i; i < 80; i++) {
    pins[i] = 0;
  }
  pinMode(75, OUTPUT);
  pinMode(76, OUTPUT);
  pinMode(77, OUTPUT);
}

void pinOn(uint8_t pin) {
  pins[pin] = 1;
  digitalWrite(pin, pins[pin]);
}

void pinToggle(uint8_t pin) {
  pins[pin] = 1 - pins[pin];
  digitalWrite(pin, pins[pin]);
}

void pinOff(uint8_t pin) {
  pins[pin] = 0;
  digitalWrite(pin, pins[pin]);
}
