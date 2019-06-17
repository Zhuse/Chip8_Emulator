#include "chip8.h"

Chip8CPU cpu();
int Chip8CPU::getFileSize(FILE *program)
{
    int size = 0;
    fseek(program, 0, SEEK_END); // seek to end of file
    size = ftell(program);       // get current file pointer
    fseek(program, 0, SEEK_SET); // seek back to beginning of file

    return size;
}

void Chip8CPU::print(std::string s)
{
    std::cout << s << "\n";
}

void Chip8CPU::initializeMemory()
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

void Chip8CPU::loadGame()
{
    unsigned int memCount = 0;
    unsigned char c;
    unsigned char *buffer;
    unsigned int fileSize = 0;
    size_t result;
    FILE *program = fopen("test/pong.ch8", "rb+");

    if (program == NULL)
    {
        fputs("File error", stderr);
        exit(1);
    }

    fileSize = getFileSize(program);
    std::cout << "Loaded file size is " << fileSize << " bytes \n";
    buffer = (unsigned char *)malloc(getFileSize(program));
    /*
        if (program != NULL)
        {
            while ((c = fgetc(program)) != EOF)
            {
                memory[0x200 + memCount] = c;
                std::cout << std::hex << memory[0x200 + memCount] << "\n";
                memCount++;
            }
            std::cout << "Successfully loaded program";
        }
        else
        {
            fclose(program);
            std::cout << "ERROR: could not load program";
        }*/

    // copy the file into the buffer:
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

    for (int i = 0; i < fileSize; i++)
    {
        memory[0x200 + i] = buffer[i];
    }
}

void Chip8CPU::emulateCycle()
{
    unsigned short int opCode = 0x0;

    opCode = (int)(memory[pc] << 8 | (memory[pc + 1]));
    std::cout << opCode << "\n";
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
    pc += 2;
}