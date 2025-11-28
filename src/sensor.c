#include "sensor.h"
#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define UART_PORT      UART_NUM_1

int sensor_init(int TX, int RX){
    esp_err_t error;

    uart_config_t uart_config = {
        .baud_rate = 921600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    error = uart_param_config(UART_PORT, &uart_config);
    if (error != ESP_OK) {
        return 0;
    }
    error = uart_set_pin(UART_PORT, TX, RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (error != ESP_OK) {
        return 0;
    }
    error = uart_driver_install(UART_PORT, 1024 * 2, 0, 0, NULL, 0);
    if (error != ESP_OK) {
        return 0;
    }

    return 1;
}

int sensor_read(uint16_t *dist){
    uint8_t data[1024];
    uint8_t frame[16];
    int frame_pos = 0;
    
    int len = uart_read_bytes(UART_PORT, data, 1024, 20 / portTICK_PERIOD_MS);
    if (len <= 0){
        return 0;
    }
    for (int i = 0; i < len; i++) {
        uint8_t b = data[i];

        if (frame_pos == 0 && b != 0x57) continue;
        if (frame_pos == 1 && b != 0x00) { frame_pos = 0; continue; }

        frame[frame_pos++] = b;

        if (frame_pos == 16) {
            *dist = frame[8] | (frame[9] << 8);
            frame_pos = 0;
            return 1;
        }
    }
    return 0;
}