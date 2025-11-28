#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

int sensor_init(int TX_PIN, int RX_PIN);
int sensor_read(uint16_t *dist);

#endif