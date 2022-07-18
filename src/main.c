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
// int uiColorMap[] = {0x7C00, 0x03E0, 0x7C1F, 0x3879};
// int ui[] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
// int uiSize = 12;
// int uiWidth = 4;

// Gross Globals Area
const int canvasWidth = 230;
const int canvasHeight = 100;
const int startingCursorTimer = 1000;
int cursorStateTimer = startingCursorTimer;
int cursorColorState = 0; // Iterates 0 through to 6
int cursorX = 3;
int cursorY = 3;
int colorUnderCursor = 0x000;
#include "inc/debounces.c"

// Colors
const int offWhite = 0x7BDE;
const int canvasColor = 0x7FBE;
const int offBlack = 0x0421;
const int rred = 0x001F;
const int uiBg = 0x1C03;
const int pickerColor = 0x02FF;

#include "inc/colorPalette.c"

void place(volatile unsigned short vram[], int x, int y, int color)
{
  vram[y * 240 + x] = color;

  // This function should check if the cursor is there and update its bank if it is
}

/**
 * Yes, this is bad.
 * Don't do this in real programs.
 * Our program won't hit maintainability issues due to size.
 */
#include "functions/absoluteDouble.c"
#include "functions/drawImage.c"
#include "functions/drawRectangle.c"
// #include "functions/drawLine.c" // Something is weird in here. The lines don't draw full length

void selectColor(volatile unsigned short vram[], int color)
{
  int lineOffsetX = (currentColor % 8) * 16 + 15;
  int lineOffsetY = (currentColor / 8) * 19 + 127;

  place(vram, lineOffsetX, lineOffsetY, uiBg);
  place(vram, lineOffsetX - 2, lineOffsetY, uiBg);
  place(vram, lineOffsetX + 2, lineOffsetY, uiBg);
  place(vram, lineOffsetX, lineOffsetY + 1, uiBg);

  currentColor = color;

  lineOffsetX = (currentColor % 8) * 16 + 15;
  lineOffsetY = (currentColor / 8) * 19 + 127;

  place(vram, lineOffsetX, lineOffsetY, pickerColor);
  place(vram, lineOffsetX - 2, lineOffsetY, pickerColor);
  place(vram, lineOffsetX + 2, lineOffsetY, pickerColor);
  place(vram, lineOffsetX, lineOffsetY + 1, pickerColor);
}

void update(volatile unsigned short vram[], int keyStates)
{
#include "inc/controls.c"

  // Placing the cursor
  if (cursorStateTimer == 0)
  {
    if (cursorColorState == 0)
    {
      cursorColorState++;
      place(vram, cursorX, cursorY, rred);
    }
    else if (cursorColorState == 2)
    {
      cursorColorState++;
      place(vram, cursorX, cursorY, colorUnderCursor);
    }
    else if (cursorColorState == 4)
    {
      cursorColorState++;
      place(vram, cursorX, cursorY, offBlack);
    }
    else if (cursorColorState == 6)
    {
      cursorColorState = 0;
      place(vram, cursorX, cursorY, colorUnderCursor);
    }
    else
    {
      cursorColorState++;
    }

    cursorStateTimer = startingCursorTimer;
  }
  else
  {
    cursorStateTimer--;
  }
}

int main(void)
{
  // All of the init stuff...

  // Write into the I/O registers, setting video display parameters.
  volatile unsigned char *ioram = (unsigned char *)0x04000000;
  ioram[0] = 0x03; // Use video mode 3 (in BG2, a 16bpp bitmap in VRAM)
  ioram[1] = 0x04; // Enable BG2 (BG0 = 1, BG1 = 2, BG2 = 4, ...)

  // Write pixel colours into VRAM
  volatile unsigned short *vram = (unsigned short *)0x06000000;
  uint32 keyStates = 0;

  srand(time(NULL));

  // drawImage(vram, ui, uiSize, uiColorMap, 0, 0, uiWidth);
  drawRectangle(vram, 5, 5, canvasWidth, canvasHeight, lightWhite);
  drawRectangle(vram, 0, 110, 239, 50, uiBg);

  drawRectangle(vram, 10, 115, 11, 11, offBlack);
  drawRectangle(vram, 26, 115, 11, 11, darkBlue);
  drawRectangle(vram, 42, 115, 11, 11, darkPurple);
  drawRectangle(vram, 58, 115, 11, 11, darkGreen);
  drawRectangle(vram, 74, 115, 11, 11, brown);
  drawRectangle(vram, 90, 115, 11, 11, darkGrey);
  drawRectangle(vram, 106, 115, 11, 11, lightGrey);
  drawRectangle(vram, 122, 115, 11, 11, lightWhite);

  drawRectangle(vram, 10, 134, 11, 11, red);
  drawRectangle(vram, 26, 134, 11, 11, orange);
  drawRectangle(vram, 42, 134, 11, 11, yellow);
  drawRectangle(vram, 58, 134, 11, 11, green);
  drawRectangle(vram, 74, 134, 11, 11, blue);
  drawRectangle(vram, 90, 134, 11, 11, lavendar);
  drawRectangle(vram, 106, 134, 11, 11, pink);
  drawRectangle(vram, 122, 134, 11, 11, peach);

  selectColor(vram, 0);
  selectColor(vram, 1);
  selectColor(vram, 2);
  selectColor(vram, 9);

  // Then the core loop...
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
