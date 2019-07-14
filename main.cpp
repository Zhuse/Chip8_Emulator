#include "chip8.h" // Your cpu core implementation

const int WINDOW_GFX_SCALE = 8;
SDL_Window *initWindow();
void drawGraphics(SDL_Renderer *renderer, unsigned char gfx[]);
int main(int argc, char **argv)
{
  SDL_Window *window = NULL;
  SDL_Event event;
  Chip8CPU cpu;
  window = SDL_CreateWindow(
      "Chip-8 Interpreter", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_GFX_SCALE * 64,
      WINDOW_GFX_SCALE * 32,
      SDL_WINDOW_SHOWN);

  // Setup renderer
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  cpu.initializeMemory();
  cpu.loadGame();
  while (true)
  {
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        exit(0);
      }
    }
    // Emulate one cycle
    cpu.emulateCycle();
    if (cpu.drawFlag)
    {
      drawGraphics(renderer, cpu.gfx);
    }
    /*
    // If the draw flag is set, update the screen
    if(chip8CPU.drawFlag)
      drawGraphics();
 
    // Store key press state (Press and Release)
    chip8CPU.setKeys();	*/
    SDL_Delay(16);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

void drawGraphics(SDL_Renderer *renderer, unsigned char gfx[])
{
  SDL_Rect rect;
  for (int i = 0; i < 32; i++)
  {
    for (int c = 0; c < 64; c++)
    {
      if (gfx[i * 64 + c])
      {
        rect.x = c * WINDOW_GFX_SCALE;
        rect.y = i * WINDOW_GFX_SCALE;
        rect.w = WINDOW_GFX_SCALE;
        rect.h = WINDOW_GFX_SCALE;
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 100);
        SDL_RenderFillRect(renderer, &rect);
      }
    }
  }
  SDL_RenderPresent(renderer);
}
SDL_Window *initWindow()
{
  SDL_Window *window;       // Declare a pointer
  SDL_Init(SDL_INIT_VIDEO); // Initialize SDL2

  // Create an application window with the following settings:
  window = SDL_CreateWindow(
      "CHIP-8 Interpreter",    // window title
      SDL_WINDOWPOS_UNDEFINED, // initial x position
      SDL_WINDOWPOS_UNDEFINED, // initial y position
      512,                     // width, in pixels
      256,                     // height, in pixels
      SDL_WINDOW_OPENGL        // flags - see below
  );

  // Check that the window was successfully created
  if (window == NULL)
  {
    // In the case that the window could not be made...
    printf("Could not create window: %s\n", SDL_GetError());
    return NULL;
  }
  // The window is open: could enter program loop here (see SDL_PollEvent())

  return window;
}