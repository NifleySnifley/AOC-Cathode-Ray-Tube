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
const int8_t INPUT[240] = { 1, 15, 1, -11, 1, 6, 1, -3, 1, 5, 1, -1, 1, -8, 1, 13, 1, 4, 0, 1, -1, 1, 5, 1, -1, 1, 5, 1, -1, 1, 5, 1, -1, 1, 5, 1, -1, 1, -35, 1, 1, 1, 24, 1, -19, 1, 1, 1, 16, 1, -11, 0, 0, 1, 21, 1, -15, 0, 0, 1, -3, 1, 9, 1, 1, 1, -3, 1, 8, 1, 1, 1, 5, 0, 0, 0, 0, 0, 1, -36, 0, 1, 1, 1, 7, 0, 0, 0, 1, 2, 1, 6, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 7, 1, 1, 0, 1, -13, 1, 13, 1, 7, 0, 1, 1, 1, -33, 0, 0, 0, 1, 2, 0, 0, 0, 1, 8, 0, 1, -1, 1, 2, 1, 1, 0, 1, 17, 1, -9, 1, 1, 1, 1, 1, -3, 1, 11, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, -13, 1, -19, 1, 1, 1, 3, 1, 26, 1, -30, 1, 12, 1, -1, 1, 3, 1, 1, 0, 0, 0, 1, -9, 1, 18, 1, 1, 1, 2, 0, 0, 1, 9, 0, 0, 0, 1, -1, 1, 2, 1, -37, 1, 1, 1, 3, 0, 1, 15, 1, -21, 1, 22, 1, -6, 1, 1, 0, 1, 2, 1, 1, 0, 1, -10, 0, 0, 1, 20, 1, 1, 1, 2, 1, 2, 1, -6, 1, -11, 0, 0, 0 };

#define SCREEN_W 40
#define SCREEN_H 6

int main() {
    AnalogOut beamX(A3);
    AnalogOut beamY(A4);

    uint32_t i = 0;
    uint32_t clk = 0;
    int32_t X = 1;

    int32_t p1sum = 0;

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

        if (p1sum != INT_MAX && (clk == 20-1 || clk == 60-1 || clk == 100-1 || clk == 140-1 || clk == 180-1 || clk == 220-1)) {
            p1sum += X * (clk+1);
        }

        // printf("Clk: %d, x: %d\n", clk, X);
        int x = clk % SCREEN_W;
        int y = clk / SCREEN_W;

        // Move beam to pizel if inside sprite
        if (abs(x - X) <= 1) {
            beamX = (float(x) / float(SCREEN_W));
            beamY = 1-(float(y) / (float(SCREEN_H * 2)));
        }

        // Reset
        if (clk == 60*4) { 
            if (p1sum != INT_MAX) {
                printf("Part One: %d\n", p1sum);
            }
            clk = 0; i = 0; adding = false; X = 1; p1sum = INT_MAX; 
        }
    }
}
