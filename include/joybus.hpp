#ifndef JOYBUS_H
#define JOYBUS_H

#include "joybus.pio.h"

#include "hardware/pio.h"
#include "pico/stdlib.h"

typedef struct {
    uint pin;
    PIO pio;
    uint sm;
    uint offset;
} joybus_port_t;

// TODO: Merge these 3 into one function and use pure C for internal code, C++ for API
int joybus_port_init(joybus_port_t *port, uint pin, PIO pio);
int joybus_port_init(joybus_port_t *port, uint pin, PIO pio, uint sm);
int joybus_port_init(joybus_port_t *port, uint pin, PIO pio, uint sm, uint offset);
void joybus_port_terminate(joybus_port_t *port);

uint joybus_send_receive(
    joybus_port_t *port,
    uint8_t *message,
    uint message_len,
    uint8_t *response_buf,
    uint response_len,
    uint read_timeout_us
);

void joybus_send_bytes(joybus_port_t *port, uint8_t *bytes, uint len);
void joybus_send_byte(joybus_port_t *port, uint8_t byte, bool stop);

void joybus_reset_receive(joybus_port_t *port);

uint joybus_receive_bytes(joybus_port_t *port, uint8_t *buf, uint len, uint64_t timeout_us);
uint8_t joybus_receive_byte(joybus_port_t *port);
bool joybus_receive_byte_timeout(joybus_port_t *port, uint8_t *byte, uint64_t timeout_us);

#endif