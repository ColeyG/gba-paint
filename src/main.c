#include <time.h>
#include <stdlib.h>

#define MEM_IO 0x04000000
#define REG_DISPLAY (*((volatile uint32 *)(MEM_IO)))
#define REG_DISPLAY_VCOUNT (*((volatile uint32 *)(MEM_IO + 0x0006)))
#define REG_KEY_INPUT (*((volatile uint32 *)(MEM_IO + 0x0130)))

#define KEY_A 0x0001
#define KEY_B 0x0002

#define KEY_SELECT 0x0004
#define KEY_START 0x0008

#define KEY_RIGHT 0x0010
#define KEY_LEFT 0x0020
#define KEY_UP 0x0040
#define KEY_DOWN 0x0080

#define KEY_R 0x0100
#define KEY_L 0x0200

#define KEY_ANY 0x03FF

typedef unsigned int uint32;

// "Image" Arrays
// TODO: store the UI in one array and the image in another
// TODO: hover states will be controlled by an in-to-out testing function that checks for entering and leaving certain areas
int uiColorMap[] = {0x7C00, 0x03E0, 0x7C1F, 0x3879};
int ui[] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
int uiSize = 12;
int uiWidth = 4;

// Gross Globals Area
int timer = 0;
const int startingCursorTimer = 100;
int cursorTimer = startingCursorTimer;
int cursorX = 100;
int cursorY = 10;
const int startingDebounce = 1000;
int debounce = startingDebounce;

// Colors
int offWhite = 0x7BDE;
int offBlack = 0x0421;

void place(volatile unsigned short vram[], int x, int y, int color)
{
  vram[y * 240 + x] = color;
}

void drawImage(volatile unsigned short vram[], int imageArray[], int imageSize, int colorArray[], int startX, int startY, int width)
{
  int i;
  int rowOffset = 0;
  int x = startX;
  int y = startY;

  for (i = 0; i < imageSize; i++)
  {
    place(vram, x, y, colorArray[imageArray[i]]);

    x++;
    rowOffset++;

    if (rowOffset > width - 1)
    {
      rowOffset = 0;
      x = startX;
      y++;
    }
  }
}

void update(volatile unsigned short vram[], int keyStates)
{
  if (debounce == 0)
  {
    if (keyStates & KEY_LEFT)
    {
      debounce = startingDebounce;
      cursorX--;
    }

    if (keyStates & KEY_RIGHT)
    {
      debounce = startingDebounce;
      cursorX++;
    }

    if (keyStates & KEY_UP)
    {
      debounce = startingDebounce;
      cursorY--;
    }

    if (keyStates & KEY_DOWN)
    {
      debounce = startingDebounce;
      cursorY++;
    }
  }

  if (cursorX > 239)
  {
    cursorX = 239;
  }

  if (cursorX < 0)
  {
    cursorX = 0;
  }

  if (cursorY > 159)
  {
    cursorY = 159;
  }

  if (cursorY < 0)
  {
    cursorY = 0;
  }

  if (debounce != 0)
  {
    debounce--;
  }

  // Placing the cursor
  if (cursorTimer == 0)
  {
    if (timer % 2)
    {
      place(vram, cursorX, cursorY, offBlack);
    }
    else
    {
      place(vram, cursorX, cursorY, offWhite);
    }

    cursorTimer = startingCursorTimer;
  }
  else
  {
    cursorTimer--;
  }

  timer++;
}

int main(void)
{
  // Write into the I/O registers, setting video display parameters.
  volatile unsigned char *ioram = (unsigned char *)0x04000000;
  ioram[0] = 0x03; // Use video mode 3 (in BG2, a 16bpp bitmap in VRAM)
  ioram[1] = 0x04; // Enable BG2 (BG0 = 1, BG1 = 2, BG2 = 4, ...)

  // Write pixel colours into VRAM
  volatile unsigned short *vram = (unsigned short *)0x06000000;
  uint32 keyStates = 0;

  srand(time(NULL));

  drawImage(vram, ui, uiSize, uiColorMap, 0, 0, uiWidth);

  while (1)
  {

    while (REG_DISPLAY_VCOUNT >= 160)
      ;
    while (REG_DISPLAY_VCOUNT < 160)
    {
      keyStates = ~REG_KEY_INPUT & KEY_ANY;

      update(vram, keyStates);
    }
  }

  return 0;
}
