#include <Arduino.h>// Libraries

#include "DogMaster.h"
#define DEBUG_PRINT_ENABLE

DogMaster dog;



void setup() {float controller_x;

  Serial.begin(250000);

  delay(5000);

  

  Serial.println("Dog starting...");

  dog.begin(115); // RF channel 115

  Serial.println("Dog ready!");

}



void loop() {

  

}

