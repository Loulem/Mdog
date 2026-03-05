#include <Arduino.h>

#include "DogMaster.h"

#define LED_BUILTIN 2 // builtin LED pin for ESP32 DevKit
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
  dog.setStepInterval(100); 
}
long int led_timer = 0;
byte run_led_state = 0;
void loop() {
  dog.update();
  //delay(10); // Small delay
  if (millis() - led_timer > 500) {
    run_led_state = !run_led_state;
    digitalWrite(LED_BUILTIN, run_led_state);
    led_timer = millis();
  }
  
}

