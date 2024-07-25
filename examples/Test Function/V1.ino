//IRFAN R

/*
  NOTE :
  you cant create a custom character after clear display, you need to delay them first otherwise it cant work
*/


//Include LIB
#include <string.h>
#include "Rams_LCD.h"

Rams_LCD LCD{2, 3, 7, 6, 5, 4}; // RS E D7 D6 D5 D4 D3 D2 D1 D0 


uint8_t heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};


void setup(){
  LCD.initialize(16, 2, 0);
  LCD.clearDisplay();
  delay(100);
  LCD.printMode("By :", 1, 0);
  LCD.printMode("Irfan R", 1, 1);
  delay(1000);
  LCD.clearDisplay();
  delay(100);               // YOU NEED THIS DELAY OTHERWISE IT CANT CREATE A CUSTOM CHAR
  LCD.createChar(0, heart); 
  LCD.setCursor(0, 0); 
  LCD.displayCustomChar(0); 
  LCD.print("IRFAN");
  delay(500);
  LCD.clearDisplay();
  delay(100);
  LCD.loopShiftRight("BY :", 0, 50, 1, 0);
  LCD.loopShiftLeft("Irfan R", 1, 50, 1, 0);

}

void loop() {
  
}
