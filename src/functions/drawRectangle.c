void drawRectangle(volatile unsigned short vram[], int x, int y, int width, int height, int color)
{
  // line(vram, x, y, x + width, y, color);
  // line(vram, x + width, y, x + width, y + height, color);
  // line(vram, x, y, x, y + height, color);
  // line(vram, x, y + height, x + width, y + height, color);

  int i;
  int n;

  for (i = 0; i < width; i++)
  {
    for (n = 0; n < height; n++)
    {
      place(vram, i + x, n + y, color);
    }
  }
}
