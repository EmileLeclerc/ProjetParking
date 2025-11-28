#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"

int mock_sensor_init(int TX, int RX){
    return 1;
}

int mock_sensor_read(uint16_t *dist){
    vTaskDelay(pdMS_TO_TICKS(500));
    static int test_state = 0;
    test_state = (test_state + 1) % 16;

    if (test_state >= 0 && test_state <= 3) *dist = 5;
    else if (test_state >= 4 && test_state <= 7) *dist = 1500;
    else if (test_state >= 8 && test_state <= 11) return 0;
    else if (test_state >= 12 && test_state <= 15) *dist = 2500;
    return 1;
}

sensor_interface_t SENSOR_MOCK = {
    .init = mock_sensor_init,
    .read = mock_sensor_read
};