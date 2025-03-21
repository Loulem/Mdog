#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

String address_1 = "00001";
RF24 radioNRF(7, 8);
int total = 0;
String radioNRFData;
char receivedData[32];

void serial_setupConnection(int baudrate) {
  Serial.begin(baudrate);
  while (!Serial) {
    Serial.println("En attente de l'ouverture du port série...");
    delay(1000);
  }
  Serial.println("Port série activé. Baudrate: " + String(baudrate));
  delay(50);
}

void nrf24_init(uint8_t channel) {
  printf_begin();
  radioNRF.begin();
  radioNRF.setChannel(channel); // communication channel (0-125)
  radioNRF.setDataRate(RF24_250KBPS); // communication speed (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS)
  radioNRF.enableDynamicPayloads();
  radioNRF.setPALevel(RF24_PA_MIN);// amplification (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH et RF24_PA_MAX)
  radioNRF.printDetails();
}


void setup() {
  serial_setupConnection(9600);
  nrf24_init(115);
  radioNRF.openReadingPipe(1, (uint8_t *)address_1.c_str());
}

void loop() {
  radioNRF.startListening();
  if (radioNRF.available()) {
    total = total + 1;
    Serial.println(total);
    radioNRF.read(&receivedData, sizeof(receivedData));
    radioNRFData = receivedData;
    Serial.println(radioNRFData);
  }
}


