#define _XOPEN_SOURCE 600
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "c_gpio.h"

int gpio_pin;

void
irpause()
{
    struct timespec ts = { .tv_sec = 0, .tv_nsec = 450000 };
    while(nanosleep(&ts, &ts) < 0 && errno == EINTR) ;
}

void
write_byte(uint8_t byte)
{
    for(int i = 0; i < 8; i++) {
        if(byte & 1) {
            output_gpio(gpio_pin, HIGH);
        }

        irpause();

        if(byte & 1) {
            output_gpio(gpio_pin, LOW);
        }

        irpause();

        byte >>= 1;
    }
}

int
main(int argc, const char** argv)
{
    if(argc < 2 || !(gpio_pin = atoi(argv[1]))) {
        fprintf(stderr, "usage: irtx <gpio pin>\n");
    }

    if(setup() != SETUP_OK) {
        fprintf(stderr, "could not set up GPIO\n");
    }

    setup_gpio(gpio_pin, OUTPUT, PUD_OFF);

    write_byte('X');

    cleanup();
}
