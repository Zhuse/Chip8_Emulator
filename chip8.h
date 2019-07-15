#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <Windows.h>
#include <SDL2/SDL.h>
#include <fstream>

class Chip8CPU
{
private:
    unsigned short opcode;
    unsigned char memory[4096];
    unsigned char V[16];
    unsigned short I;
    unsigned short pc;
    unsigned char delay_timer;
    unsigned char sound_timer;
    unsigned short stack[16];
    unsigned short sp;
    unsigned char key[16];

    int getFileSize(FILE *program);

    void print(std::string s);
    void printRegisters();
    void reDrawScreen();

public:
    bool drawFlag;
    unsigned char inputKey;
    unsigned char gfx[64 * 32];

    void initializeMemory();
    void loadGame();
    void emulateCycle();
};

