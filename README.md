# Advent of Code 2022 day 10 on an oscilloscope!

Designed to be ran on a ST Nucleo L432KC development board.

### Pinout

- A3: scope X
- A4: scope Y

### Using a different input

Currently the code is running with my input for day 10, but it can use yours too. 
The script `inputconverter.py` takes one argument (your input file) and prints out the array equivalent to your input.
Replace line 18 of `main.cpp` with what is printed out and you should be good to go!