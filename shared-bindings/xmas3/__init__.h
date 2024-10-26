#pragma once

#include <stdint.h>

#include "py/obj.h"

void set_led(uint8_t index, uint8_t level);
uint8_t get_led(uint8_t index);

void start_display(int delayUs, int minFreq);
