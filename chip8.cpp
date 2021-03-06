#include "chip8.h"

//CONSTANTS
const unsigned char font_set[80] =
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8CPU cpu();

//Get the size of the file
int Chip8CPU::getFileSize(FILE *program)
{
    int size = 0;
    fseek(program, 0, SEEK_END); // seek to end of file
    size = ftell(program);       // get current file pointer
    fseek(program, 0, SEEK_SET); // seek back to beginning of file
    return size;
}

//Initialize Memory
void Chip8CPU::initializeMemory()
{
    unsigned int sizeV = sizeof(V);
    unsigned int sizeGfx = sizeof(gfx);
    unsigned int sizeStack = sizeof(stack);
    pc = 0x200;
    I = 0;
    sp = 0;
    delay_timer = 60;
    sound_timer = 60;
    clockSpeed = 600;
    fps = 60;
    instructionCounter = 0;

    for (unsigned int i = 0; i < sizeV; i++)
    {
        V[i] = 0;
    }

    for (unsigned int i = 0; i < sizeGfx; i++)
    {
        gfx[i] = 0;
    }

    for (unsigned int i = 0; i < sizeStack; i++)
    {
        stack[i] = 0;
    }

    for (unsigned int i = 0; i < 0x80; i++)
    {
        memory[i] = font_set[i];
    }
}

//Load memory into buffer
void Chip8CPU::loadGame(const char * filePath)
{
    unsigned int memCount = 0;
    unsigned char c;
    unsigned char *buffer;
    unsigned int fileSize = 0;
    size_t result;
    FILE *program = fopen(filePath, "rb+");

    if (program == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }

    fileSize = getFileSize(program);
    buffer = (unsigned char *)malloc(getFileSize(program));
    result = fread(buffer, sizeof(unsigned char), fileSize, program);

    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        exit(2);
    }
    if (result != fileSize / sizeof(unsigned char))
    {
        fputs("Reading error", stderr);
        exit(3);
    }

    for (unsigned int i = 0; i < fileSize; i++)
    {
        memory[0x200 + i] = buffer[i];
    }
}

void Chip8CPU::emulateCycle()
{
    unsigned short int opCode = 0x0;
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int addition = 0;
    bool flag = false;

    //Fetch opCode
    opCode = (int)(memory[pc] << 8 | (memory[pc + 1]));
    x = (int)((opCode & 0x0F00) >> 8);
    y = (int)((opCode & 0x00F0) >> 4);
    addition = 0;
    flag = false;

    //Decode and Execute opCode
    switch (opCode & 0xF000)
    {
    case 0x0000:
        switch (opCode & 0x0FFF)
        {
        case 0x00E0:
            for (unsigned int i = 0; i < 64 * 32; i++)
            {
                gfx[i] = 0;
            }
            pc += 2;
            break;
        case 0x00EE:
            pc = stack[sp];
            stack[sp] = 0;
            sp--;
            pc += 2;
            break;
        default:
            pc += 2;
            break;
        }
        break;
    case 0x1000:
        pc = opCode & 0x0FFF;
        break;
    case 0x2000:
        sp++;
        stack[sp] = pc;
        pc = opCode & 0x0FFF;
        break;
    case 0x3000:
        if (V[x] == (opCode & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x4000:
        if (V[x] != (opCode & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x5000:
        if (V[x] == V[y])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x6000:
        V[x] = opCode & 0x00FF;
        pc += 2;
        break;
    case 0x7000:
        V[x] += opCode & 0x00FF;
        pc += 2;
        break;
    case 0x8000:
        switch (opCode & 0x000F)
        {
        case 0x0000:
            V[x] = V[y];
            break;
        case 0x0001:
            V[x] = V[x] | V[y];
            break;
        case 0x0002:
            V[x] = V[x] & V[y];
            break;
        case 0x0003:
            V[x] = V[x] ^ V[y];
            break;
        case 0x0004:
            addition = V[x] + V[y];
            if (addition > 0xFF)
            {
                V[0xF] = 1;
            }
            V[x] = addition & 0x00FF;
            break;
        case 0x0005:
            if (V[x] > V[y])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[x] - V[y];
            break;
        case 0x0006:
            if (V[x] & 0x1 == 0x1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] /= 2;
            break;
        case 0x0007:
            if (V[y] > V[x])
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] = V[y] - V[x];
            break;
        case 0x000E:
            if (V[x] >> 7 == 0x1)
            {
                V[0xF] = 1;
            }
            else
            {
                V[0xF] = 0;
            }
            V[x] *= 2;
        }
        pc += 2;
        break;
    case 0x9000:
        if (V[x] != V[y])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0xA000:
        I = opCode & 0x0FFF;
        pc += 2;
        break;
    case 0xB000:
        pc += V[0] + (opCode & 0x0FFF);
        break;
    case 0xC000:
        V[x] = (rand() % 256) & (opCode && 0x00FF);
        pc += 2;
        break;
    case 0xD000:
    {
        unsigned short height = opCode & 0x000F;
        unsigned short pixel;
        V[0xF] = 0;
        for (unsigned int yline = 0; yline < height; yline++)
        {
            pixel = memory[I + yline];
            for (unsigned int xline = 0; xline < 8; xline++)
            {
                if ((pixel & (0x80 >> xline)) != 0)
                {
                    if (gfx[(V[x] + xline + ((V[y] + yline) * 64))] == 1)
                        V[0xF] = 1;
                    gfx[V[x] + xline + ((V[y] + yline) * 64)] ^= 1;
                }
            }
        }
        drawFlag = true;
        pc += 2;
        break;
    }
    case 0xE000:
        switch (opCode & 0x00FF)
        {
        case 0x9E:
            if (inputKeys[V[x]])
            {
                pc += 4;
            }
            else
            {
                pc += 2;
            }
            break;
        case 0xA1:
            if (!inputKeys[V[x]])
            {
                pc += 4;
            }
            else
            {
                pc += 2;
            }
            break;
        }
        break;
    case 0xF000:
        switch (opCode & 0x00FF)
        {
        case 0x07:
            V[x] = delay_timer;
            pc += 2;
            break;
        case 0x0A:
        {
            bool flag = false;
            for (unsigned int i = 0; i < sizeof(inputKeys); i++)
            {
                if (inputKeys[i])
                {
                    flag = true;
                    V[x] = i;
                }
            }
            if (flag)
            {
                pc += 2;
            }
            break;
        }
        case 0x15:
            delay_timer = V[x];
            pc += 2;
            break;
        case 0x18:
            sound_timer = V[x];
            pc += 2;
            break;
        case 0x1E:
            if (I += V[x] >= 0x1000) {
                V[0xF] = 0x1;
            } else {
                V[0xF] = 0x0;
            }
            I += V[x];
            pc += 2;
            break;
        case 0x29:
            I = V[x] * 5;
            pc += 2;
            break;
        case 0x33:
            memory[I] = V[x] / 100;
            memory[I + 1] = V[x] / 10 % 10;
            memory[I + 2] = V[x] % 10;
            pc += 2;
            break;
        case 0x55:
            for (unsigned int i = 0; i <= x; i++)
            {
                memory[I + i] = V[i];
            }
            pc += 2;
            break;
        case 0x65:
            for (unsigned int i = 0; i <= x; i++)
            {
                V[i] = memory[I + i];
            }
            pc += 2;
            break;
        default:
            pc += 2;
            break;
        }
        break;

    default:
        exit(EXIT_FAILURE);
    }
    if (delay_timer > 0 && instructionCounter >= clockSpeed / fps) {
        --delay_timer;
        instructionCounter = 0;
    }
    if (sound_timer > 0 && instructionCounter >= clockSpeed / fps)
    {
        if (sound_timer == 1)
            Beep(600, 100);
        --sound_timer;
    }
    instructionCounter++;
}