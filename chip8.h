#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>

#define BUFFER_SIZE = 4096
namespace Chip8
{
class CPU
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

    int getFileSize(FILE *program)
    {
        int size = 0;
        fseek(program, 0, SEEK_END); // seek to end of file
        size = ftell(program);       // get current file pointer
        std::cout << "Loaded program file size is " << size << " bytes long";
        fseek(program, 0, SEEK_SET); // seek back to beginning of file

        return size;
    }

    void print(std::string s){
        std::cout << s << "\n";
    }

public:
    void initializeMemory()
    {
        unsigned int sizeV = sizeof(V);
        unsigned int sizeGfx = sizeof(gfx);
        unsigned int sizeStack = sizeof(stack);
        pc = 0x200;
        opcode = 0;
        I = 0;
        sp = 0;
        delay_timer = 60;
        sound_timer = 60;

        for (int i = 0; i < sizeV; i++)
        {
            V[i] = 0;
        }

        for (int i = 0; i < sizeGfx; i++)
        {
            gfx[i] = 0;
        }

        for (int i = 0; i < 0x80; i++)
        {
            //memory[i] = chip8_fontset[i];
        }
    }

    void loadGame()
    {
        unsigned int memCount = 0;
        unsigned char c;
        FILE *program = fopen("test/pong.ch8", "rb+");
        if (program)
        {
            while ((c = getc(program)) != EOF)
            {
                memory[0x200 + memCount] = c;
                memCount++;
            }
            std::cout << "Successfully loaded program";
        }
        else
        {
            std::cout << "ERROR: could not load program";
        }
    }

    void emulateCycle()
    {
        unsigned char opCode = 0x0;

        opCode = memory[pc] << 8 | memory[pc + 1];

        switch (opCode & 0xF000)
        {
            case 0x0000:
                print("0x0000");
                break;
            case 0x1000:
                print("0x1000");
                break;
            case 0x2000:
                print("0x2000");
                break;
            case 0x3000:
                print("0x3000");
                break;
            case 0x4000:
                print("0x4000");
                break;
            case 0x5000:
                print("0x5000");
                break;
            case 0x6000:
                print("0x6000");
                break;
            case 0x7000:
                print("0x6000");
                break;
            case 0x8000:
                print("0x6000");
                break;
            case 0x9000:
                print("0x6000");
                break;
            case 0xA000:
                print("0xA000");
                break;
            case 0xB000:
                print("0xB000");
                break;
            case 0xC000:
                print("0xC000");
                break;
            case 0xD000:
                print("0xD000");
                break;
            case 0xE000:
                print("0xE000");
                break;
            case 0xF000:
                print("0xF000");
                break;

            default:
                printf("Unknown opcode: 0x%X\n", opCode);
        }

        if (delay_timer > 0)
            --delay_timer;

        if (sound_timer > 0)
        {
            if (sound_timer == 1)
                printf("BEEP!\n");
            --sound_timer;
        }
    }
};
} // namespace Chip8
