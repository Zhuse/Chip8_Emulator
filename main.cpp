#include "chip8.h" // Your cpu core implementation
#include <map>
const int WINDOW_GFX_SCALE = 8;
const int FPS = 60;
const int CLOCK_SPEED_IN_HZ = 600;
const char *defaultColours[] = { "0", "0", "0"};
SDL_Window *initWindow();
std::map<SDL_Keycode, unsigned char> getKeyboard();
void drawGraphics(SDL_Renderer *renderer, unsigned char gfx[], unsigned int r, unsigned int g, unsigned int b);

int main(int argc, char *argv[])
{
  SDL_Window *window = NULL;
  SDL_Event event;
  SDL_Keycode eventKey;
  SDL_Renderer *renderer;
  Chip8CPU cpu;

  std::map<SDL_Keycode, unsigned char> keyMap = getKeyboard();

  unsigned int red;
  unsigned int green;
  unsigned int blue;

  try {
     red = std::stoi(argv[2]);
     green = std::stoi(argv[3]);
     blue = std::stoi(argv[4]);
  } catch (...) {
    red = 255;
    green = 255;
    blue = 255;
  }

  window = SDL_CreateWindow(
      "Chip-8 Interpreter", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      WINDOW_GFX_SCALE * 64,
      WINDOW_GFX_SCALE * 32,
      SDL_WINDOW_SHOWN);

  // Setup renderer
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  cpu.initializeMemory();
  cpu.loadGame(argv[1]);

  //Interpreter event loop
  while (true)
  {
    if (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        exit(0);
      } else if (event.type == SDL_KEYDOWN) { //Handle keyboard key press
        eventKey = event.key.keysym.sym;
        if (keyMap.count(eventKey) == 1) {
          cpu.inputKeys[keyMap[eventKey]] = true;
        }
      } else if (event.type == SDL_KEYUP) { //Handle keyboard key release
        eventKey = event.key.keysym.sym;
        if (keyMap.count(eventKey) == 1) {
          cpu.inputKeys[keyMap[eventKey]] = false;
        }
      }
    }

    //Begin clock cycle
    for (unsigned int i = 0; i < CLOCK_SPEED_IN_HZ / FPS; i++) {
      cpu.emulateCycle();
    }

    //Draw graphics if required
    if (cpu.drawFlag)
    {
      drawGraphics(renderer, cpu.gfx, red, green, blue);
    }

    //Delay to reach 60 FPS
    SDL_Delay(1000 / FPS);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}

void drawGraphics(SDL_Renderer *renderer, unsigned char gfx[], unsigned int r, unsigned int g, unsigned int b)
{
  SDL_Rect rect;
  int index = 0;
  rect.w = WINDOW_GFX_SCALE;
  rect.h = WINDOW_GFX_SCALE;
  for (unsigned int i = 0; i < 32; i++)
  {
    for (unsigned int c = 0; c < 64; c++)
    {
      index = i * 64 + c;
      rect.x = c * WINDOW_GFX_SCALE;
      rect.y = i * WINDOW_GFX_SCALE;
      gfx[index] ? SDL_SetRenderDrawColor(renderer, r, g, b, 100) : SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
      SDL_RenderFillRect(renderer, &rect);
    }
  }
  SDL_RenderPresent(renderer);
}

std::map<SDL_Keycode, unsigned char> getKeyboard() {
  std::map<SDL_Keycode, unsigned char> keyboard;
  SDL_Keycode hexPad [] = {
    SDLK_1, 
    SDLK_2, 
    SDLK_3, 
    SDLK_4, 
    SDLK_q, 
    SDLK_w, 
    SDLK_e, 
    SDLK_r, 
    SDLK_a, 
    SDLK_s, 
    SDLK_d, 
    SDLK_f, 
    SDLK_z, 
    SDLK_x, 
    SDLK_c, 
    SDLK_v
    };
  unsigned char hexPadMapped [] = {
    0x1, 
    0x2, 
    0x3, 
    0xC, 
    0x4, 
    0x5, 
    0x6, 
    0xD, 
    0x7, 
    0x8, 
    0x9, 
    0xE, 
    0xA, 
    0x0,
    0xB, 
    0xF
    };
  for (unsigned int i = 0; i < sizeof(hexPad); i++) {
    keyboard.insert(std::make_pair(hexPad[i], hexPadMapped[i]));
  }
  return keyboard;
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
      SDL_WINDOW_OPENGL        // flag
  );

  // Check that the window was successfully created
  if (window == NULL)
  {
    // In the case that the window could not be made...
    printf("Could not create window: %s\n", SDL_GetError());
    return NULL;
  }

  return window;
}