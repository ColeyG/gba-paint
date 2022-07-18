if (debounce == 0)
{
  if (keyStates & KEY_LEFT)
  {
    place(vram, cursorX, cursorY, colorUnderCursor);
    colorUnderCursor = vram[cursorY * 240 + (cursorX - 1)];
    debounce = startingDebounce;
    cursorX--;
    place(vram, cursorX, cursorY, rred);
  }

  if (keyStates & KEY_RIGHT)
  {
    place(vram, cursorX, cursorY, colorUnderCursor);
    colorUnderCursor = vram[cursorY * 240 + (cursorX + 1)];
    debounce = startingDebounce;
    cursorX++;
    place(vram, cursorX, cursorY, rred);
  }

  if (keyStates & KEY_UP)
  {
    place(vram, cursorX, cursorY, colorUnderCursor);
    colorUnderCursor = vram[(cursorY - 1) * 240 + cursorX];
    debounce = startingDebounce;
    cursorY--;
    place(vram, cursorX, cursorY, rred);
  }

  if (keyStates & KEY_DOWN)
  {
    place(vram, cursorX, cursorY, colorUnderCursor);
    colorUnderCursor = vram[(cursorY + 1) * 240 + cursorX];
    debounce = startingDebounce;
    cursorY++;
    place(vram, cursorX, cursorY, rred);
  }

  if (keyStates & KEY_R)
  {
    int newColor = currentColor + 1;

    if (newColor > paletteSize)
    {
      newColor = 0;
    }

    selectColor(vram, newColor);

    debounce = startingDebounce;
  }

  if (keyStates & KEY_L)
  {
    int newColor = currentColor - 1;

    if (newColor < 0)
    {
      newColor = paletteSize - 1;
    }

    selectColor(vram, newColor);

    debounce = startingDebounce;
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
