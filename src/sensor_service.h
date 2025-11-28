#pragma once
#include <stdint.h>
class ISensorService {
    public:
    virtual ~ISensorService() = default;
    virtual bool sensorInit(int TX_PIN, int RX_PIN) = 0;
    virtual bool sensorRead(uint16_t *dist) = 0;
};