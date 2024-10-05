#include <stdint.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/xmas3/__init__.h"


//| """`xmas3` module.
//|
//| Provides NOCOMMIT."""
//|
//| def get_answer() -> int:
//|     """Gets 42
//|     """
//|     ...
//|
static mp_obj_t xmas3_get_answer(void) {
    int32_t result = get_answer();
    return MP_OBJ_NEW_SMALL_INT(result);
}
static MP_DEFINE_CONST_FUN_OBJ_0(xmas3_get_answer_obj, xmas3_get_answer);

static mp_obj_t xmas3_set_led(mp_obj_t level_obj)
{
    uint8_t level = mp_obj_get_int(level_obj);
    set_led(level);
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_1(xmas3_set_led_obj, xmas3_set_led);

static mp_obj_t xmas3_start_display(void)
{
    start_display();
    return mp_const_none;
}
static MP_DEFINE_CONST_FUN_OBJ_0(xmas3_start_display_obj, xmas3_start_display);



static const mp_rom_map_elem_t xmas3_module_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_xmas3) },
    { MP_ROM_QSTR(MP_QSTR_get_answer), MP_ROM_PTR(&xmas3_get_answer_obj) },
    { MP_ROM_QSTR(MP_QSTR_set_led), MP_ROM_PTR(&xmas3_set_led_obj) },
    { MP_ROM_QSTR(MP_QSTR_start_display), MP_ROM_PTR(&xmas3_start_display_obj) },
};
static MP_DEFINE_CONST_DICT(xmas3_module_globals, xmas3_module_globals_table);

const mp_obj_module_t xmas3_module = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&xmas3_module_globals,
};
MP_REGISTER_MODULE(MP_QSTR_xmas3, xmas3_module);

