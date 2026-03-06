//TODO: Implement error handling for radio begin failure.
//TODO: Implement parameter files for data rate, power level, etc.

#include "RFController.h"
#include <printf.h>

RFController::RFController(uint8_t ce_pin, uint8_t csn_pin) :
    rfDevices(ce_pin, csn_pin), channel(0) {}

bool RFController::begin(uint8_t _channel, const char* address) {
    channel = _channel;
    printf_begin();
    if (!rfDevices.begin()) return false; // TODO: throw error
    rfDevices.setChannel(channel);
    rfDevices.setDataRate(RF24_250KBPS);
    rfDevices.enableDynamicPayloads();
    rfDevices.setPALevel(RF24_PA_MIN);
    rfDevices.openReadingPipe(1, (const uint8_t*)address);
    rfDevices.startListening();
    Serial.println("=== NRF24 Details ===");
    rfDevices.printDetails();
    Serial.print("isChipConnected: ");
    Serial.println(rfDevices.isChipConnected());
    return true;
}

ControllerCommand RFController::retrieveRadioCommand() {
    ControllerCommand cmd{0.0f, 0.0f, false};
    if (!rfDevices.available()) return cmd;
    uint8_t len = rfDevices.getDynamicPayloadSize();
    if (len == 0 || len > sizeof(data_receive_buffer) - 1) return cmd;
    rfDevices.read(data_receive_buffer, len);
    data_receive_buffer[len] = '\0';
    return extractCommandFromData(data_receive_buffer, len);
}

ControllerCommand RFController::extractCommandFromData(const char* data, uint8_t len) { // data is supposed to be in this format: "state,power"
    ControllerCommand cmd{0.0f, 0.0f, false};
    String dataString = String(data);
    dataString.trim(); // remove extra characters
    int comma = dataString.indexOf(',');
    if (comma == -1) return cmd;
    cmd.angle = dataString.substring(0, comma).toFloat();
    cmd.power = dataString.substring(comma + 1).toFloat();
    cmd.valid = true;
    return cmd;
}
