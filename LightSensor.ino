#include <Galaxia.h>
#include <HWI.h>
#include <SWI.h>
#include <Clock.h>

int counter;
int last;//tracks the last result of the line detection (0=line, 1=no line)
Clock lightSensorClock;

void setupLightSensor () {
  last = 1;
  pinMode(78, OUTPUT);
  lightSensorClock.begin(blinky, 0, 500);
}

void blinky () {
  pinToggle(78);
}

/*
void LightSensor() {
    // 1.Configure GPIO pin as output
    // 2.Set output to HIGH
    // 3.Allow at least 10[µs] for the sensor output voltage to rise
    // 4.Reconfigure GPIO pin as input
    // 5.Measure time elapsed until input is LOW
    // 6.Use the measured time to determine whether on black or white surface
    int countPin6 = 0;
    int threshold = 700;
    while(1){
        pinMode(23, OUTPUT);    // configure port 3 pin 6 as an output pin
        digitalWrite(23, 1);//set the port 3 pin 6 to high
        //GPIO_setAsOutputPin(GPIO_PORT_P3, GPIO_PIN7);    // configure port 3 pin 7 as an output pin
        //GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN7);//set the port 3 pin 7 to GPIO_INPUT_PIN_HIGH
        //start a timer
        for(int i = 0; i < 1000; i++) {}
        pinMode(23, INPUT);     // reconfigure this to an input pin
        //GPIO_setAsInputPin(GPIO_PORT_P3, GPIO_PIN7);     // reconfigure this to an input pin
        countPin6 = 0;
        //countPin7 = 0;
        while(digitalRead(23) == HIGH) {
            countPin6 +=  1;
            //countPin7 +=  GPIO_getInputPinValue(GPIO_PORT_P3, GPIO_PIN7);
        }                      // read from port 3 pin 6 and 7
        //sprintf(a, "countPin6 %d \n\r", countPin6); // then output command unknown
        if(countPin6 > threshold && last) {
            //putString("Black");
            pinToggle(23);
            last = 0;
            counter++;
        } else if(countPin6 < threshold && last == 0) {
            if(counter > 3) {
                stop();
                queueNum = (queueNum + 1) % 2;
                Semaphore_post(printSema);
                Task_sleep(25);
                printTime();
            } else {
                print = (print + 1) % 2;
            }
            counter = 0;
            last = 1;
        } else if(countPin6 > threshold) counter++;

        // pass the time to a function that determines if the thing we just saw was black or white
        Task_sleep(25);
    }
}*/
