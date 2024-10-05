#pragma once

#include <stdint.h>

#include "py/obj.h"

int32_t get_answer(void);

void set_led(uint8_t level);

void start_display(void);
