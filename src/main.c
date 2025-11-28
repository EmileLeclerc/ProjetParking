#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "led_strip.h"
#include "sensor.h"

#define USE_MOCK       0     // 1 to use mock, 0 to use real sensor

#if USE_MOCK
extern sensor_interface_t SENSOR_MOCK;
#define SENSOR_IMPL  SENSOR_MOCK
#else
extern sensor_interface_t SENSOR_REAL;
#define SENSOR_IMPL  SENSOR_REAL
#endif


#define TX_PIN         18      // ESP TX → sensor RX (blue wire)
#define RX_PIN         19      // ESP RX ← sensor TX (yellow wire)

#define LED_PIN        8       // uses the esp's built-in led (pin 8 for esp32-c6-devkitm-1)
#define LED_BUFFER     1000    // time that the sensor has to be stable for before the led changes state, in ms (default 10000)

#define DIST_ERROR     100      // if distance lower than this, sends error (default 100)
#define DIST_TAKEN     1000    // if distance higher than this (or 0), parking is free, if lower, parking is taken (default 1000)

static const char *TAG = "Range Sensor";
static led_strip_handle_t strip;
typedef enum {
    LED_STATE_ERROR = 0,
    LED_STATE_TAKEN,
    LED_STATE_FREE
} led_state_t;

void app_main(void) {
    sensor_interface_t sensor = SENSOR_IMPL;
    
    if(!sensor.init(TX_PIN, RX_PIN)){
        ESP_LOGI(TAG, "sensor init failed, stopping.");
        return;
    }
    else{
        ESP_LOGI(TAG, "Reader started. Waiting for frames...");
    }
    
    uint16_t dist = 0;
    //LED setup
    led_state_t current_state = LED_STATE_ERROR;
    led_state_t pending_state = LED_STATE_ERROR;
    uint32_t state_change_time = 0;
    led_strip_config_t strip_config = {
        .strip_gpio_num = LED_PIN,
        .max_leds = 1,
    };

    led_strip_rmt_config_t rmt_config = {
        .resolution_hz = 10 * 1000 * 1000,
    };

    led_strip_new_rmt_device(&strip_config, &rmt_config, &strip);

    while (1) {
        led_state_t new_state = LED_STATE_ERROR;
        if(sensor.read(&dist)){
            ESP_LOGI(TAG, "Distance: %u mm", dist);

            if (dist <= DIST_ERROR && dist != 0){
                new_state = LED_STATE_ERROR;
            }
            else if (dist == 0 || dist > DIST_TAKEN) {//sensor sends 0 when out of range
                new_state = LED_STATE_FREE;
            }
            else if (dist <= DIST_TAKEN){ 
                new_state = LED_STATE_TAKEN;
            }

            if (new_state != pending_state) {
                pending_state = new_state;
                state_change_time = esp_log_timestamp();
            }

            // If state was stable for LED_BUFFER amount of time and is different from active LED, change color and send zigbee
            if (pending_state != current_state) {
                uint32_t now = esp_log_timestamp();
                if (now - state_change_time >= LED_BUFFER) {
                    current_state = pending_state;

                    if (dist <= DIST_ERROR){
                        ESP_LOGI(TAG, "there may be something on the sensor, it can't see anything");
                        //send error zigbee here
                    }
                    else{
                        ESP_LOGI(TAG, "LED changed after being stable for %d ms", LED_BUFFER);
                        //send state change zigbee here
                    }
                }
            }
        }
        else{
            ESP_LOGI(TAG, "sensor problem");
            current_state = LED_STATE_ERROR;
            //send error zigbee here
        }

        switch (current_state) {
            case LED_STATE_ERROR: 
                led_strip_set_pixel(strip, 0, 0, 0, 10);// blue
                led_strip_refresh(strip);   
            break;
            case LED_STATE_TAKEN:                     
                led_strip_set_pixel(strip, 0, 10, 0, 0);// red
                led_strip_refresh(strip);    
            break;
            case LED_STATE_FREE:                       
                led_strip_set_pixel(strip, 0, 0, 10, 0);// green
                led_strip_refresh(strip);   
            break;
        }
    }
}
