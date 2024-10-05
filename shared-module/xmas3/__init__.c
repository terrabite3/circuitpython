#include "shared-bindings/xmas3/__init__.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"

int32_t get_answer(void)
{
    return 42;
}


static uint8_t led_level = 0;

void set_led(uint8_t level)
{
    led_level = level;
}

void display_func(void);

void display_func(void)
{
    // SIO (Single-cycle IO) is at offset 0xd0000000
    // GPIO_OUT_SET is at 0x014
    uint32_t* GPIO_OUT_SET = (uint32_t*)0xd0000014;
    uint32_t* GPIO_OUT_CLR = (uint32_t*)0xd0000018;
    
    uint8_t counter = 0;
    while (true)
    {
        uint16_t val = counter + led_level;
        counter = val;
        if (val & 0x100)
        {
            *GPIO_OUT_SET = 1;
        }
        else
        {
            *GPIO_OUT_CLR = 1;
        }

        sleep_ms(1);
    }
}


void start_display(void)
{
    multicore_reset_core1();
    multicore_launch_core1(display_func);
}
