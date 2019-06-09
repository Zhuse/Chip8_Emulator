#include "chip8.h" // Your cpu core implementation
 
Chip8::CPU chip8CPU;
 
int main(int argc, char **argv) 
{
  // Set up render system and register input callbacks
  /* setupGraphics();
  setupInput(); */
 
  // Initialize the Chip8 system and load the game into the memory  
  chip8CPU.initializeMemory();
  std::cout << "Hello World \n";
  chip8CPU.loadGame();
  std::cout << "not loading";
  // Emulation loop
  while(true)
  {
    // Emulate one cycle
    chip8CPU.emulateCycle();
 
 /*
    // If the draw flag is set, update the screen
    if(chip8CPU.drawFlag)
      drawGraphics();
 
    // Store key press state (Press and Release)
    chip8CPU.setKeys();	*/
  }
 
  return 0;
}