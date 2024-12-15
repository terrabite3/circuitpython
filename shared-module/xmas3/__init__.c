#include "shared-bindings/xmas3/__init__.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"

#include <math.h>

const uint32_t displaySize = 8 * 6;
uint16_t displayData[8 * 6];

uint16_t lut[256];

// This is the minimum frequency that an LED should be allowed to flash
int minFreq;
int delayUs;

void __no_inline_not_in_flash_func(set_led)(uint8_t index, uint8_t level)
{
    if (index < displaySize)
    {
        displayData[index] = lut[level];
    }
}

uint8_t __no_inline_not_in_flash_func(get_led)(uint8_t index)
{
    if (index < displaySize)
    {
        // displayData stores the values from the LUT.
        // We need to invert the LUT to get the original 8-bit value.
        uint16_t lutVal = displayData[index];
        for (uint16_t i = 0; i < 256; ++i)
        {
            if (lutVal == lut[i])
            {
                return i;
            }
        }
        // We should never get here!
    }
    return 0;
}

static void __no_inline_not_in_flash_func(calculate_lut)(void)
{
    // This is the period that an LED would flash at if it were given
    // the raw value of 1.

    // Now that the explicit delay is removed from the loop, we use the measured
    // delay of the loop, which is 5.5 us.
    // Actually that value might be wrong. Might actually be 1 us. 
    // But this looks good to my eye.
    delayUs = 6;
    float fullPeriod = delayUs / 1.0E6 * 65536 * 6;
    // This is the minimum value (besides 0) that we can give an LED
    // without dropping below minFreq.
    uint16_t minValue = minFreq * fullPeriod;

    lut[0] = 0;
    for (int i = 1; i < 256; ++i)
    {
        float norm = i / 255.0;
        float adjusted = norm * norm;
        lut[i] = (int)(adjusted * (65535 - minValue) + minValue);
    }
}

static void __no_inline_not_in_flash_func(display_func)(void)
{
    calculate_lut();

    // Clear the display
    for (uint8_t i = 0; i < displaySize; ++i)
    {
        displayData[i] = 0;
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

            // There used to be an explicit sleep here, but that was causing problems.
            // If the sleep routine is running from flash with XIP, then it will break
            // when we write to flash. But it turns out the loop has an appropriate
            // speed if we let it free-run.
            // I think I might be losing 3% brightness by switching too frequently,
            // but that's pretty acceptable.
        }
    }
}


void __no_inline_not_in_flash_func(start_display)(int delayUsArg, int minFreqArg)
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

    delayUs = delayUsArg;
    minFreq = minFreqArg;

    multicore_reset_core1();
    multicore_launch_core1(display_func);
}
