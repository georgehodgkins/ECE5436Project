#include <Clock.h>
#include <Event.h>
#include <Galaxia.h>
#include <HWI.h>
#include <Mailbox.h>
#include <Semaphore.h>
#include <SWI.h>
#include <SWItrigger.h>
#include <Task.h>
#include <Timer.h>

#include "utils.h"
#include "commands.h"

boolean stringComplete;
uint8_t len;
String str;

void setupBluetooth() {
  Serial1.begin(115200);
  stringComplete = 0;
  len = 0;
  str = "";
}


void loopBluetooth() {
  char input;
  int temp;
  //union str {
  //}
  //while (1) {
  if (stringComplete == 0 && Serial1.available()) {
    input = Serial1.read();              // read input
    if (input == '\b' && len > 0) { // if input is backspace, move string position back 1 (-2 + 1) to allow it to overwrite the last character inputted
      Serial1.print("\b \b");         // put a space in place of the last character so it actually goes away then backspace again so the cursor is in the right spot
      if(str.length() > 0) str.remove(str.length() - 1);
      len--;                      // move backward in string
    } else if (input == '\b') {   // keep user from backspacing into the prompt and keep i from going below 0 (string[-1] == death or something)
      len = 0;
    } else if (input == '\r') {
      //str += '\0';
      if(len != 0)
        stringComplete = 1;
    } else {                      // if input isn't a backspace, write character to the string
      Serial1.write(input);               // print input (uart doesn't do that by default)
      len++;
      str += input;
      if (len == 10) {
        str = str;
        stringComplete = 1;
      }
    }
  } else if (stringComplete == 1) {
    Serial1.print("\n\r");
    temp = runCommand(str);
    if (temp != -1) {
      
    } else {
      Serial1.print("Unknown Command");
      Serial1.print("\n\r");
    }
    stringComplete = 0;
    len = 0;
    str = "";
  } else {
    // do nothing
  }
  //temp = resolveCommand(in);             // resolve command string to command number
  //if (temp != -1) {                      // if it could be resolved, put it in command queue
  //i = fifo_put(commandQueue, &temp);
  //runCommands();
  //if(i == -1)
  //    Serial1.print("FIFO full");   // prints command if the fifo is full
  //} else {
  //    Serial1.print("\n\r");            // if command couldn't be resolved, go to new line
  //    Serial1.print("Unknown command"); // then output command unknown
  //}
  //}
}
