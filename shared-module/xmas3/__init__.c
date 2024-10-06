#include "shared-bindings/xmas3/__init__.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <math.h>

const uint32_t displaySize = 8 * 6;
uint16_t displayData[8 * 6];

uint16_t lut[256];

void set_led(uint8_t index, uint8_t level)
{
    if (index < displaySize)
    {
        displayData[index] = lut[level];
    }
}

void display_func(void);

void display_func(void)
{
    for (int i = 0; i < 256; ++i)
    {
        double norm = i / 255.0;
        double adjusted = norm * norm;
        lut[i] = (int)(adjusted * 65535);
    }


    uint16_t displayCounters[displaySize];

    while (true)
    {
        for (uint8_t arm = 0; arm < 6; ++arm)
        {
            uint8_t gpio0_7 = 0;
            for (uint8_t i = 0; i < 8; ++i)
            {
                uint8_t index = arm * 8 + i;
                uint32_t temp = displayCounters[index] + displayData[index];
                if (temp & 0x10000)
                {
                    gpio0_7 |= 1 << i;
                }
                displayCounters[index] = temp;
            }

            // Drive GPIO8-13 high (off)
            gpio_set_mask(0x3F00);//(0b11'1111'0000'0000);
            // Drive GPIO0-7 low (off)
            gpio_clr_mask(0xFF);

            // Drive the required bits of GPIO0-7 high (on)
            gpio_set_mask(gpio0_7);
            // Drive the correct bit of GPIO8-13 low (on)
            gpio_clr_mask(1 << (arm + 8));

            // Assuming the above takes no time (which probably isn't far off),
            // a 1ms delay here will produce a worst-case PDM frequency of 4 Hz
            // (produced by a duty cycle of 1/256).
            sleep_us(50);
        }
    }
}


void start_display(void)
{
    for (int i = 0; i < 14; ++i)
    {
        gpio_init(i);
        gpio_set_dir(i, GPIO_OUT);
        gpio_set_drive_strength(i, GPIO_DRIVE_STRENGTH_12MA);
    }

    for (int i = 14; i < 16; ++i)
    {
        gpio_init(i);
        gpio_set_input_enabled(i, true);
        gpio_pull_up(i);
    }

    multicore_reset_core1();
    multicore_launch_core1(display_func);
}
