// RF communication utilities: parse incoming controller frames
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>                    //https://www.arduino.cc/en/reference/wire
#include <printf.h>



String address_1 = "00001";
RF24 radioNRF(7, 8);
int total = 0;
String radioNRFData;
char receivedData[32];
void nrf24_init(uint8_t channel);

// Struct to return parsed command values to callers
struct ControllerCommand {
  float angle; // degrees
  float power; // transmitter-defined units
  bool ok;     // true if a valid frame was parsed
};


void nrf24_init(uint8_t channel)
{
  
  printf_begin();
  radioNRF.begin();
  radioNRF.setChannel(channel);       // communication channel (0-125)
  radioNRF.setDataRate(RF24_250KBPS); // communication speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
  radioNRF.enableDynamicPayloads();
  radioNRF.setPALevel(RF24_PA_MIN); // amplification (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH et RF24_PA_MAX)
  radioNRF.printDetails();
  radioNRF.openReadingPipe(1, (uint8_t *)address_1.c_str());
}

// Return the latest controller command as a struct. If no payload is ready or parsing fails, ok=false.
ControllerCommand get_command() {
  ControllerCommand out{0.0f, 0.0f, false};
  radioNRF.startListening();
  if (radioNRF.available()) {
    uint8_t len = radioNRF.getDynamicPayloadSize();
    if (len == 0) return out;
    if (len > sizeof(receivedData) - 1) len = sizeof(receivedData) - 1;
    radioNRF.read(&receivedData, len);
    receivedData[len] = '\0';

    String s = String(receivedData);
    s.trim();
    int l = s.indexOf('(');
    int r = s.indexOf(')', l + 1);
    String inner = (l != -1 && r != -1 && r > l) ? s.substring(l + 1, r) : s;
    int comma = inner.indexOf(',');
    if (comma != -1) {
      out.angle = inner.substring(0, comma).toFloat();
      out.power = inner.substring(comma + 1).toFloat();
      out.ok = true;
    }
  }
  return out;
}

#endif // COMMUNICATION_H