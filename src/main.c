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

#define KEY_RIGHT 0x0050
#define KEY_LEFT 0x0060
#define KEY_UP 0x0070
#define KEY_DOWN 0x0080

// #define KEY_R 0x0100
// #define KEY_L 0x0100

#define KEY_ANY 0x03FF

typedef unsigned int uint32;

// Gross Globals Area
int x = 10;
int y = 10;
int colour = 0x001F;

void place(volatile unsigned short vram[], int x, int y, int color)
{
  vram[y * 240 + x] = color;
}

void update(volatile unsigned short vram[], int keyStates)
{
  if (keyStates & KEY_LEFT)
  {
    x--;
  }

  if (keyStates & KEY_RIGHT)
  {
    x++;
  }

  if (keyStates & KEY_UP)
  {
    y--;
  }

  if (keyStates & KEY_DOWN)
  {
    y++;
  }

  if (x > 240)
  {
    x = 240;
  }

  if (x < 0)
  {
    x = 0;
  }

  if (y > 160)
  {
    y = 160;
  }

  if (y < 0)
  {
    y = 0;
  }

  place(vram, x, y, colour);

  // colour++;
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

  // Wait forever
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
