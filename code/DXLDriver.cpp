#include "DXLDriver.h"

DXLDriver::DXLDriver() : log_level(0) {}

DXLDriver::~DXLDriver() {}

bool DXLDriver::begin(const char* device_name, uint32_t baudrate) {
    if (!dxl.begin(device_name, baudrate)) {
        return false;
    }
    return true;
}

void DXLDriver::setLogLevel(uint8_t level) {
    log_level = level;
}

void DXLDriver::setPacketHandler(float protocol_version) {
    dxl.setPacketHandler(protocol_version);
}

float DXLDriver::getProtocolVersion(void) {
    return dxl.getProtocolVersion();
}

uint32_t DXLDriver::getBaudrate(void) {
    return dxl.getBaudrate();
}
