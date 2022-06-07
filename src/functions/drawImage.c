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
