// This file is part of the CircuitPython project: https://circuitpython.org
//
// SPDX-FileCopyrightText: Copyright (c) 2021 Scott Shawcroft for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#pragma once

#define MICROPY_HW_BOARD_NAME "Xmas3 Board"
#define MICROPY_HW_MCU_NAME "rp2040"

#define CIRCUITPY_BOARD_I2C         (1)
#define CIRCUITPY_BOARD_I2C_PIN     {{.scl = &pin_GPIO25, .sda = &pin_GPIO24}}
