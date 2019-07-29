# Chip8_Emulator :computer:
An Chip-8 Interpreter to run Chip-8 programs, using SDL2 for graphics.

![Rush Hour on the interpreter](https://raw.githubusercontent.com/Zhuse/Chip8_Emulator/master/images/rushhour.png "Rush Hour on the interpreter")

## Specifications
Chip-8 is an interpreted programming language developed by Joseph Weisbecker. The instruction set contains a total of 35 2-byte operations.

The specifications of the emulated interpreter are as follows:
- Memory: 4KB
- Registers: 16, each with a size of 1-byte
- Stack: 16 levels, each with a size of 2-bytes
- Display: 64x32

The interpreter runs at 60 FPS and operates with a clock speed of 600Hz by default.

## Usage
`Chip_8_Interpreter.exe <path to .ch8 program> <red RGB value> <blue RGB value> <green RGB value>`

## Build
1. Download SDL2 from https://www.libsdl.org/download-2.0.php
2. Change the lines in the makefile to point to SDL2's include and library folders.
3. Run `make`
  
## To-do
1. Non-blocking sound
2. Bug fix with Rush Hour where menu does not process keyboard input.
