/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdint>

// Baud-controlled serial debugging.
BufferedSerial ser(USBTX, USBRX, 115200);
FileHandle *mbed::mbed_override_console(int fd) {
    return &ser;
}


// Input (noop = 0, addx = 1 followed by argument)
const int8_t INPUT[240] = { 1, 2, 1, 3, 0, 0, 1, 1, 1, 5, 1, -1, 1, 5, 1, 1, 0, 0, 1, 4, 0, 0, 1, 5, 1, -5, 1, 6, 1, 3, 1, 1, 1, 5, 1, 1, 0, 1, -38, 1, 41, 1, -22, 1, -14, 1, 7, 0, 0, 1, 3, 1, -2, 1, 2, 0, 1, 17, 1, -12, 1, 5, 1, 2, 1, -16, 1, 17, 1, 2, 1, 5, 1, 2, 1, -30, 0, 1, -6, 1, 1, 0, 1, 5, 0, 0, 0, 1, 5, 1, -12, 1, 17, 0, 0, 0, 0, 1, 5, 1, 10, 1, -9, 1, 2, 1, 5, 1, 2, 1, -5, 1, 6, 1, 4, 0, 0, 1, -37, 0, 0, 1, 17, 1, -12, 1, 30, 1, -23, 1, 2, 0, 1, 3, 1, -17, 1, 22, 0, 0, 0, 1, 5, 0, 1, -10, 1, 11, 1, 4, 0, 1, 5, 1, -2, 0, 1, -6, 1, -29, 1, 37, 1, -30, 1, 27, 1, -2, 1, -22, 0, 1, 3, 1, 2, 0, 1, 7, 1, -2, 1, 2, 1, 5, 1, -5, 1, 6, 1, 2, 1, 2, 1, 5, 1, -25, 0, 1, -10, 0, 1, 1, 0, 1, 2, 0, 0, 0, 0, 1, 7, 1, 1, 1, 4, 1, 1, 0, 1, 2, 0, 1, 3, 1, 5, 1, -1, 0, 1, 3, 1, 5, 1, 2, 1, 1, 0, 0, 0, 0 };

#define SCREEN_W 40
#define SCREEN_H 6

int main() {
    AnalogOut beamX(A3);
    AnalogOut beamY(A4);

    uint32_t i = 0;
    uint32_t clk = 0;
    int32_t X = 1;

    // Pipelined addition because it's two clock cycles
    bool adding = false;

    while (true) {
        if (adding) {
            adding = false;
            X += int32_t(INPUT[i]);
        } else if (INPUT[i] == 1) {
            // Addx instruction received
            adding = true;
        } else if (INPUT[i] == 0) {
            // Noop
        }

        ++i;
        ++clk;

        // printf("Clk: %d, x: %d\n", clk, X);
        int x = clk % SCREEN_W;
        int y = clk / SCREEN_W;

        // Move beam to pizel if inside sprite
        if (abs(x - X) <= 1) {
            beamX = (float(x) / float(SCREEN_W));
            beamY = 1-(float(y) / (float(SCREEN_H * 2)));
        }

        // Reset
        if (clk == 60*4) { clk = 0; i = 0; adding = false; X = 1; }
    }
}
