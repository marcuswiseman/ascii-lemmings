#include <iostream>
#include <windows.h>

using namespace std;
//MAP
char Map[10][20] = { "###################", //19 chars (20-1)
                     "#@ #",
                     "# #",
                     "# #",
                     "# + #", // + = adds 5 to health
                     "# #",
                     "# #", //@ = player
                     "# #",
                     "###################"
                   };

int PlayerX;
int PlayerY;
int Hp = 5;
int MaxHP = 25;
bool end = false; //boolean
int GameSpeed = 50; // the lower the faster it is(milliseconds)
int main() {
    while (end == false) {
        if (Hp > MaxHP) {
        Hp = MaxHP;
        }
        system("cls"); //clearscreen (same as in batch)
        for (int y = 0; y < 10; y++) { //display map
            cout << Map[y] << endl;
        }
        cout << "Position : " << PlayerX << "," << PlayerY << endl;
        cout << "HP : " << Hp << endl;

//map reading
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 20; x++) {
                switch (Map[y][x]) { // you dident add this ( { ) at the end
                case '@': { //everything to do with player movement
                    if (GetAsyncKeyState(VK_UP) != 0) {
                        int y2 = (y - 1);
                        switch (Map[y2][x]) { //this will see which char is up from the player
                        case ' ': {
//if player hits black space
                            Map[y][x]= ' ';
                            y -= 1;
                            Map[y2][x] = '@';
                        }
                        break;
                        case '+': {
//if player hits black space
                            Hp += 5;
                        }
                        break;
                        }
                    }
                    if (GetAsyncKeyState(VK_DOWN) != 0) {
                        int y2 = (y + 1);
                        switch (Map[y2][x]) { //this will see which char is up from the player
                        case ' ': {
//if player hits black space
                            Map[y][x]= ' ';
                            y += 1;
                            Map[y2][x] = '@';
                        }
                        break;
                        case '+': {
//if player hits black space
                            Hp += 5;
                        }
                        break;
                        }
                    }
                    if (GetAsyncKeyState(VK_LEFT) != 0) {
                        int x2 = (x - 1);
                        switch (Map[y][x2]) { //this will see which char is up from the player
                        case ' ': {
//if player hits black space
                            Map[y][x]= ' ';
                            x -= 1;
                            Map[y][x2] = '@';
                        }
                        break;
                        case '+': {
//if player hits black space
                            Hp += 5;
                        }
                        break;
                        }
                    }
                    if (GetAsyncKeyState(VK_RIGHT) != 0) {
                        int x2 = (x + 1);
                        switch (Map[y][x2]) { //this will see which char is up from the player
                        case ' ': {
//if player hits black space
                            Map[y][x] = ' ';
                            x += 1;
                            Map[y][x2] = '@';
                        }
                        break;
                        case '+': {
//if player hits black space
                            Hp += 5;
                        }
                        break;

                        }
                    }

                }
                break;
                PlayerX = x;
                PlayerY = y;
                }

        }
        }
// set game speed
        Sleep(GameSpeed); //Sleep needs cap 'S'
    }
    return 0; //exit if Boolean "exit" = true
}
