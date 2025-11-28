#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include <stdint.h>

typedef struct {
    int  (*init)(int TX_PIN, int RX_PIN);
    int  (*read)(uint16_t *dist);
} sensor_interface_t;


#ifdef __cplusplus
}
#endif