#ifndef CHIP8_CPU
#define CHIP8_CPU
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>


class Chip8CPU
{
private:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char gfx[64 * 32];
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];

    int getFileSize(FILE *program);

    void print(std::string s);

public:
    void initializeMemory();
    void loadGame();
    void emulateCycle();
};

#endif
