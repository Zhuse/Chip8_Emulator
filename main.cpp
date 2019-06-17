#include "chip8.h" // Your cpu core implementation

int main(int argc, char **argv) {
  Chip8CPU cpu;
  // Set up render system and register input callbacks
  /* setupGraphics();
  setupInput(); */
 
  // Initialize the Chip8 system and load the game into the memory  
  cpu.initializeMemory();
  std::cout << "Hello World \n";
  cpu.loadGame();
  // Emulation loop
  while(true)
  {
    // Emulate one cycle
     cpu.emulateCycle();
 
 /*
    // If the draw flag is set, update the screen
    if(chip8CPU.drawFlag)
      drawGraphics();
 
    // Store key press state (Press and Release)
    chip8CPU.setKeys();	*/
  }
 
  return 0;
}