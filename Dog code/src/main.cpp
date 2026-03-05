#include <Arduino.h>

#include "DogMaster.h"

#define LED_BUILTIN 2 // builtin LED pin for ESP32 DevKit
DogMaster dog(115);



void setup() {

  Serial.begin(250000);
  pinMode(LED_BUILTIN, OUTPUT);
  
  delay(5000);

  

  Serial.println("Dog starting...");

  if (!dog.begin()) {
      Serial.println("Dog failed to start!");
      return;
  }

  Serial.println("Dog ready!");
  dog.setStepInterval(25); 
}


long int led_timer = 0;
byte run_led_state = 0;
void loop() {
  dog.update();
  if (millis() - led_timer > 500) {
    run_led_state = !run_led_state;
    digitalWrite(LED_BUILTIN, run_led_state);
    led_timer = millis();
  }
  
}

