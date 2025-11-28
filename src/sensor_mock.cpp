#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "sensor_mock.h"

bool MockSensorService::sensorInit(int TX, int RX) {
    return 1;
}
bool MockSensorService::sensorRead(uint16_t *dist) { //this cycles between states to make the led change colors
    vTaskDelay(pdMS_TO_TICKS(500));
    static int test_state = 0;
    test_state = (test_state + 1) % 16;

    if (test_state >= 0 && test_state <= 3) *dist = 5;
    else if (test_state >= 4 && test_state <= 7) *dist = 500;
    else if (test_state >= 8 && test_state <= 11) return 0;
    else if (test_state >= 12 && test_state <= 15) *dist = 2500;
    return 1;
}