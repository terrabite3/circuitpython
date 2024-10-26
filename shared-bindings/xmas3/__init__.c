#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/xmas3/__init__.h"

static mp_obj_t xmas3_set_led(mp_obj_t index_obj, mp_obj_t level_obj)
{
    uint8_t index = mp_obj_get_int(index_obj);
    uint8_t level = mp_obj_get_int(level_obj);
    set_led(index, level);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_2(xmas3_set_led_obj, xmas3_set_led);

static mp_obj_t xmas3_get_led(mp_obj_t index_obj)
{
    uint8_t index = mp_obj_get_int(index_obj);
    uint8_t level = get_led(index);
    return mp_obj_new_int(level);
}
static MP_DEFINE_CONST_FUN_OBJ_1(xmas3_get_led_obj, xmas3_get_led);

static mp_obj_t xmas3_start_display(size_t n_args, const mp_obj_t *args)
{
    int delayUs = 50;
    int minFreq = 30;
    if (n_args >= 1)
    {
        delayUs = mp_obj_get_int(args[0]);
    }
    if (n_args >= 2)
    {
        minFreq = mp_obj_get_int(args[1]);
    }

    start_display(delayUs, minFreq);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(xmas3_start_display_obj, 0, 2, xmas3_start_display);


static const mp_rom_map_elem_t xmas3_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_xmas3) },
    { MP_ROM_QSTR(MP_QSTR_set_led), MP_ROM_PTR(&xmas3_set_led_obj) },
    { MP_ROM_QSTR(MP_QSTR_get_led), MP_ROM_PTR(&xmas3_get_led_obj) },
    { MP_ROM_QSTR(MP_QSTR_start_display), MP_ROM_PTR(&xmas3_start_display_obj) },
};
static MP_DEFINE_CONST_DICT(xmas3_module_globals, xmas3_module_globals_table);

const mp_obj_module_t xmas3_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&xmas3_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_xmas3, xmas3_module);

