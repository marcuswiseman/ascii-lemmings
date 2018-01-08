#define _WIN32_WINNT 0x0500
#include <iostream>
#include <windows.h>
#include <fstream>

#define xsize 79
#define ysize 24

HWND GetConsoleWindow();

using namespace std;

void WinLock();
void WinUnlock();
void WinClear();
void SetColorTo(int k);

void SetColorTo(int k)
{
   HANDLE  hConsole;
   hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
   SetConsoleTextAttribute(hConsole, k);
}

// place before clear
void WinLock()
{
     LockWindowUpdate(GetConsoleWindow());
}

// place before sleep
void WinUnlock()
{
     LockWindowUpdate(NULL);
}

void WinClear()
{
     system("cls");
}
