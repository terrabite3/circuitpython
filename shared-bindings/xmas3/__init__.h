#pragma once

#include <stdint.h>

#include "py/obj.h"

void set_led(uint8_t index, uint8_t level);

void start_display(void);
