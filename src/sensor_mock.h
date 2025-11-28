#pragma once
#include "sensor_service.h"

class MockSensorService : public ISensorService {
public:
    bool sensorInit(int TX_PIN, int RX_PIN) override;
    bool sensorRead(uint16_t *dist) override;
};
