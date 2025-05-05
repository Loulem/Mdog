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


void get_command(){
  radioNRF.startListening();
  if (radioNRF.available())
  {
    radioNRF.read(&receivedData, sizeof(receivedData));
    radioNRFData = receivedData;
    int index_of_comma = radioNRFData.indexOf(',');
    command = radioNRFData.substring(0, index_of_comma).toInt();
    speed = radioNRFData.substring(index_of_comma + 1).toInt();
  }
}