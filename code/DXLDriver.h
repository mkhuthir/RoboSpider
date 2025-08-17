#ifdef DXL_DRIVER_H
#define DXL_DRIVER_H

#include <Arduino.h>
#include <DynamixelSDK.h>

class DXLDriver {
public:
    DXLDriver();
    ~DXLDriver();

    bool begin(const char* device_name, uint32_t baudrate);
    void setLogLevel(uint8_t level);
    void setPacketHandler(float protocol_version);
    float getProtocolVersion(void);
    uint32_t getBaudrate(void);

private:
    DynamixelSDK dxl;
    uint8_t log_level;
};

#endif