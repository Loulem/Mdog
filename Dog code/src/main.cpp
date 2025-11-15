#include <Arduino.h>

#include "DogMaster.h"

DogMaster dog(115);



void setup() {

  Serial.begin(250000);

  delay(5000);

  

  Serial.println("Dog starting...");

  if (!dog.begin()) {
      Serial.println("Dog failed to start!");
      return;
  }

  Serial.println("Dog ready!");

}



void loop() {
  dog.update();
  delay(1000);
}

