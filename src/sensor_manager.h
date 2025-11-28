#pragma once
#include "sensor_service.h"

class SensorManager {public:
    SensorManager(ISensorService* sensorService) : sensorService(sensorService) {}

    bool startSensor(int RX, int TX) {
        return sensorService->sensorInit(RX, TX);
    }
    bool getDist(uint16_t *dist){
        return sensorService->sensorRead(dist);
    }
private:
    ISensorService* sensorService; // abstraction → peut être mock ou réel
};