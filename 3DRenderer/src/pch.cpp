#include "pch.h"

void show_console_cursor(bool flag)
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO     cursorInfo;

    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = flag; 
    SetConsoleCursorInfo(out, &cursorInfo);
}