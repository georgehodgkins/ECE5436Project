static boolean pins[80];

/**
 * Set up GPIO pins needed for the robot
 * Also initialize the array that allows pin toggling
 */
void GPIO_Init() {
  for(int i; i < 80; i++) {
    pins[i] = 0;
  }
  pinMode(75, OUTPUT);
  pinMode(76, OUTPUT);
  pinMode(77, OUTPUT);
  pinMode(34, OUTPUT); // right motor direction, LOW = forward HIGH = backwards
  pinMode(35, OUTPUT); // right motor PWM
  pinMode(36, OUTPUT); // left motor direction, LOW = forward HIGH = backwards
  pinMode(37, OUTPUT); // left motor PWM
  analogReadResolution(14); // ADC resolution
}

/**
 * Function to simplify turning on pins, we used an array to store the current value of the pin to allow toggling on certain pins.
 *
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinOn(uint8_t pin) {
  pins[pin] = 1;
  digitalWrite(pin, pins[pin]);
}

/**
 * Function to simplify Toggling pins, we used an array to store the current value of the pin to allow toggling on certain pins.
 *
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinToggle(uint8_t pin) {
  pins[pin] = 1 - pins[pin];
  digitalWrite(pin, pins[pin]);
}

/**
 * Function to simplify turning off pins, we used an array to store the current value of the pin to allow toggling on certain pins.
 *
 * @param pin - the pin number of the pin you want to change
 * @return void
 */
void pinOff(uint8_t pin) {
  pins[pin] = 0;
  digitalWrite(pin, pins[pin]);
}
