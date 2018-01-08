#define _WIN32_WINNT 0x0501  // Windows XP = 0501H
#include <curses.h>
#include <iostream>
#include <fstream>

// defined keys
#include "Keys.h"
// used for GetAsyncKeyState()
#include <windows.h>

using namespace std;

BOOL IsWindowEnabled( )
{
    if (GetConsoleWindow() == GetActiveWindow())
    {
        return false;
    } else { return false; }
}

extern void StartLevel();
void Movement();

class lemming
{
public:
    int x;
    int y;
    char chara;
    string type;
};
// make 10 lemmings
const int lems_alive = 10;
lemming lems_ingame[lems_alive];

int InAir,move_dir = 1;
bool isDead = false;
int mx, my,sellem;
bool GameRun = false;
char Map[18][79];
char Queue[14];
string QueTypes[14];
int lemdropcount,que_num = 0;
bool AHeld,AddToQueue,Droplem,StartGame,displaylem,PLAYSA,EndLevel,OnLadder,
HIDEDOOR,HIDEDOOR2,IsPushing;
bool CanDig,Digging,
CanBuildBridge,BuildingBridge,
CanBuildLadder,BuildingLadder;
int lemcount,lemgamecount,LadderCount;
float HoldTime;
int bgnum;
extern int level;

char BGS[4][18][79] = { "------------------------------------------------------------------------------",
                        "|@@@@@@@@@@@@@@@@@###########################################################|",
                        "|#@@@@@@@@@@@@@@@####@@@@#########@@@@#@@#######@@###############@@@@@#######|",
                        "|##@@@####@@@@@#####@@@@@@#######@@@@@@@@@#####@@@@######@@@####@@@@@########|",
                        "|##########@@@#######@@@@#########@@@@@@@@@###@@@@######@@@@@####@@@@@@@#####|",
                        "|##@@@@########@@@########@@@@######@@@@@@#####@@#####@@@@@@######@@@@#######|",
                        "|##@@@@@@#####@@@@@######@@@@@@#######@@@############@@@@@#########@@########|",
                        "|###@@@@@@#####@@@########@@@#########################@@@####################|",
                        "|####@@@@##################@@@###############################################|",
                        "|############################################################################|",
                        "|############################################################################|",
                        "|###################################################GGGGG####################|",
                        "|#################################################GGGGGGGGGG####GGGG#########|",
                        "|###############################################GGGGGGGGGGGGG#GGGGGGGG#######|",
                        "|#############################################GGGGGGGGGGGGGGGGGGGGGGGGGG#####|",
                        "|############################################GGGGGGGGGGGGGGGGGGGGGGGGGGGG####|",
                        "|############################################################################|",
                        "------------------------------------------------------------------------------",
                        // space
                        "------------------------------------------------------------------------------",
                        "|@@@@@@@@#########################@@@@@@@@@@@@@@@@@@@@@@@@@##@@@@@@@@###@@@@#|",
                        "|@@@@@@#############@@@@#########@@@@@@@@@@@@@@@@@@@@@@@@@@@##@@@@@@###@@@@@@|",
                        "|##@@#############@@@@@@@###########@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#|",
                        "|##############@@@@@@@################@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@###|",
                        "|##################@@@@#########################@@@@@@@@@@@@@@@@@@@@@########|",
                        "|#############################################@@@@@@@###@@@@#################|",
                        "|########################################################@@@@@###############|",
                        "|#################################################################GGGGGGGGGGG|",
                        "|############################################################GGGGGGGGGGGGGGGG|",
                        "|########################################################GGGGGGGGGGGGGGGGGGGG|",
                        "|GGGG################################################GGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGG##########################################GGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGG#####################################GGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGG###################################GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "------------------------------------------------------------------------------",
                        // space
                        // space
                        "------------------------------------------------------------------------------",
                        "|##################@@@@@#####@@@@@@@@@@@@@@@@################################|",
                        "|##################@@@@@@@@@@@@######@@@@@@@@@###############################|",
                        "|##############@@@@@@@@@@@########@@@@@@@@@@#################################|",
                        "|################@@@@@@@#############@@@@@@@@@@@#############################|",
                        "|####################@@@@@@@#################################################|",
                        "|############################################################################|",
                        "|#######################################################################GGGGG|",
                        "|#####################################################################GGGGGGG|",
                        "|##################################################################GGGGGGGGGG|",
                        "|###############################################################GGGGGGGGGGGGG|",
                        "|GGG#########################################################GGGGGGGGGGGGGGGG|",
                        "|GGGGGG###################################################GGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGG#############################################GGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "|GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG|",
                        "------------------------------------------------------------------------------"
                      };

// queue animation
char QueueAS[13][5][79] = { "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |    ###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |i   ###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |   i###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |      ###  | |                              |",
                            "|                             | |      [i]  | |                              |",
                            "|                             | |    ~i###  | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |        ###| |                              |",
                            "|                             | |        [i]| |                              |",
                            "|                             | |      ~i###| |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |          #| |                              |",
                            "|                             | |          [| |                              |",
                            "|                             | |        ~i#| |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |#          | |                              |",
                            "|                             | |]          | |                              |",
                            "|                             | |#          | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |###        | |                              |",
                            "|                             | |[i]        | |                              |",
                            "|                             | |###        | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |  ###      | |                              |",
                            "|                             | |  [i]      | |                              |",
                            "|                             | |~i###      | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |  ~i###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |   i###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |i   ###    | |                              |",
                            "------------------------------- ------------- --------------------------------",
                            // space
                            "------------------------------- ------------- --------------------------------",
                            "|                             | |    ###    | |                              |",
                            "|                             | |    [i]    | |                              |",
                            "|                             | |    ###    | |                              |",
                            "------------------------------- ------------- --------------------------------"
                          };


extern int GameSpeed;
extern int AnimationSpeed;

// if map settings are avalible
void settings(string data)
{
    //if (data[0] == '0') { BlockAmount = 0; }
    //lemmax = (lemneed + 1);
}

void SaveGame(int lvl)
{
    ofstream set;
    set.open("Settings.txt");
    if (!set.is_open())
    {
    }
    else
    {
        set.clear();
        set << lvl;
    }
    set.close();
}
// open text document to read map
void OpenMap(char* filename)
{
    ifstream mapfile;
    mapfile.open(filename);
    string line;
    if (!mapfile.is_open())
    {
        // if map file wasent found
        cout << "error finding map" << endl;
    }
    else
    {
        string data;
        // get settings lines
        getline(mapfile, data); // settings are all on one line
        // top line eg: 01034
        // numbers represent integer settings

        // set settings read from the map file
        settings(data);
        int y = 0,x = 0;
        for (y = 0; ( (! mapfile.eof() ) && (y < 18) ); y++ )
        {
            // get line from mapfile and place it into 'line'
            getline(mapfile, line);
            for (x = 0; (x < 79) && (x < line.length()); x++)
            {
                // place the char of 'line[x]' into the 'Map[y][x]' cordinates
                Map[y][x] = line[x];
            }
            if (x < line.length()){} // if map is smaller than the set x
        }
        if (y < 18){} // if map is smaller than the set y
        mapfile.close();
    }
}

void PlaySound1(char* filename)
{
    PlaySound(TEXT (filename),NULL,SND_FILENAME | SND_ASYNC | SND_NOSTOP | SND_NOWAIT);
}

void CheckLevel(int lvl)
{
    if (bgnum > 2)
    {
        bgnum = 0;
    }
    if (lvl == 1)
    {
        bgnum= 0;
        OpenMap("Map01.txt");
        SaveGame(lvl);
    }
    else if (lvl == 2)
    {
        bgnum++;
        OpenMap("Map02.txt");
        SaveGame(lvl);
    }
    else if (lvl == 3)
    {
        bgnum++;
        OpenMap("Map03.txt");
        SaveGame(lvl);
    }
    else if (lvl == 4)
    {
        bgnum++;
        OpenMap("Map04.txt");
        SaveGame(lvl);
    }
    else if (lvl == 5)
    {
        bgnum++;
        OpenMap("Map05.txt");
        SaveGame(lvl);
    }
    else if (lvl == 6)
    {
        bgnum++;
        OpenMap("Map06.txt");
        SaveGame(lvl);
    }
    else if (lvl == 7)
    {
        bgnum++;
        OpenMap("Map07.txt");
        SaveGame(lvl);
    }
    else if (lvl == 8)
    {
        bgnum++;
        OpenMap("Map08.txt");
        SaveGame(lvl);
    }
    else if (lvl == 9)
    {
        bgnum++;
        OpenMap("Map09.txt");
        SaveGame(lvl);
    }
    else if (lvl == 10)
    {
        bgnum++;
        OpenMap("Map09.txt");
        SaveGame(lvl);
    }
    else if (lvl == 11)
    {
        bgnum++;
        OpenMap("Map10.txt");
        SaveGame(lvl);
    }
}

void reset()
{
    for (int c = 0; c < 14; c++)
    {
        Queue[c] = ' ';
        QueTypes[c] = "";
    }
    StartGame = false;
    displaylem = false;
    lemgamecount = 0;
    lemcount = 0;
    isDead = false;
    lemdropcount = 0;
    que_num = 0;
    sellem = 0;
    lemdropcount = 0;
    CanDig = false;
    Digging = false;
    CanBuildBridge = false;
    BuildingBridge = false;
    CanBuildLadder = false;
    BuildingLadder = false;
    OnLadder = false;
    IsPushing = false;
    LadderCount = 0;
    flushinp();
}

void DrawBG(int num)
{
    for (int y = 0; y < 18; y++)
    {
        for (int x = 0; x < 79; x++)
        {
            switch (BGS[num][y][x])
            {
            case '#':
            {
                if (Map[y][x] == ' ')
                {
                    if (y < 9)
                    {
                        mvaddch (y,x,ACS_BOARD|COLOR_PAIR(3)|A_BOLD);
                    }
                    else
                    {
                        mvaddch (y,x,ACS_BOARD|COLOR_PAIR(3));
                    }
                }
            }
            break;
            case 'G':
            {
                if (Map[y][x] == ' ')
                {
                    mvaddch (y,x,ACS_BOARD|COLOR_PAIR(9));
                }
            }
            break;
            case '@':
            {
                if (Map[y][x] == ' ')
                {
                    mvaddch (y,x,ACS_BOARD|COLOR_PAIR(1));
                }
            }
            break;
            }
        }
    }
}

void SetMapColors()
{
    if (bgnum > 2)
    {
        bgnum = 0;
    }
    DrawBG(bgnum);
    for (my = 0; my < 18; my++)
    {
        for (mx = 0; mx < 78; mx++)
        {
            switch (Map[my][mx])
            {
            case 'N':
            {
                mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(8)|A_BOLD);

            }
            break;
            case 'C':
            {
                mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(5));
            }
            case 'S':
            {
                int y2 = my-1;
                if (Map[y2][mx] == 'C')
                {
                    HIDEDOOR = true;
                    mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(2)|A_BOLD);
                }
                else
                {
                    HIDEDOOR = false;
                    mvaddch(my,mx,ACS_BLOCK|COLOR_PAIR(4)|A_BOLD);
                }
            }
            break;
            case 'T':
            {
                int y2 = my-1;
                if (Map[y2][mx] == 'C')
                {
                    HIDEDOOR2 = true;
                    mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(2)|A_BOLD);
                }
                else
                {
                    HIDEDOOR2 = false;
                    mvaddch(my,mx,ACS_BLOCK|COLOR_PAIR(4)|A_BOLD);
                }
            }
            break;
            case 'D':
            {
                if (HIDEDOOR == false)
                {
                    mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(5));
                }
            }
            break;
            case 'H':
            {
                if (HIDEDOOR2 == false)
                {
                    mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(5));
                }
            }
            break;
            case 'B':
            {
                mvaddch (my,mx,ACS_BLOCK|COLOR_PAIR(7));
            }
            break;
            case '-':
            {
                mvaddch (my,mx,Map[my][mx]|COLOR_PAIR(4)| A_BOLD);
            }
            break;
            case '|':
            {
                mvaddch (my,mx,Map[my][mx]|COLOR_PAIR(4)| A_BOLD);
            }
            break;
            case '*':
            {
                mvaddch (my,mx,Map[my][mx]|COLOR_PAIR(4)| A_BOLD);
            }
            break;
            default:
            {
                if (Map[my][mx] != ' ')
                {
                    mvaddch (my,mx,Map[my][mx]|COLOR_PAIR(1)| A_BOLD);
                }
            }
            break;
            }
        }
    }
}

// lemming select animation
void LemSA()
{
    int i = 0;
    do
    {
        clear();
        SetMapColors();
        // set colors for bottom border
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 78; x++)
            {
                switch (QueueAS[i][y][x])
                {
                case ' ':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(1)| A_BOLD);
                }
                break;
                case '-':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(4)| A_BOLD);
                }
                break;
                case '|':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(4)| A_BOLD);
                }
                break;
                case ']':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(5)| A_BOLD);
                }
                break;
                case '[':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(5)| A_BOLD);
                }
                break;
                case 'i':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(2)| A_BOLD);
                }
                break;
                default:
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(1)| A_BOLD);
                }
                break;
                }
            }
        }
        // print queue
        mvprintw (21,2,"Queue : ");
        for (int i = 0; i < 14; i++)
        {
            if (Queue[i] != 0)
            {
                mvaddch (21,10+i,Queue[i]);
            }
        }
        // slower animation needed
        napms(AnimationSpeed / 4);
        refresh();
        i++;
    }
    while (i != 13);
}

// lemming select animation
void revLemSA()
{
    int i = 12;
    do
    {
        clear();
        SetMapColors();
        // set colors for bottom border
        for (int y = 0; y < 5; y++)
        {
            for (int x = 0; x < 78; x++)
            {
                switch (QueueAS[i][y][x])
                {
                case ' ':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(1)| A_BOLD);
                }
                break;
                case '-':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(4)| A_BOLD);
                }
                break;
                case '|':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(4)| A_BOLD);
                }
                break;
                case ']':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(5)| A_BOLD);
                }
                break;
                case '[':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(5)| A_BOLD);
                }
                break;
                case 'i':
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(2)| A_BOLD);
                }
                break;
                default:
                {
                    mvaddch (y+18,x,QueueAS[i][y][x]|COLOR_PAIR(1)| A_BOLD);
                }
                break;
                }
            }
        }
        // print queue
        mvprintw (21,2,"Queue : ");
        for (int i = 0; i < 14; i++)
        {
            if (Queue[i] != 0)
            {
                mvaddch (21,10+i,Queue[i]);
            }
        }
        // slower animation needed
        napms(AnimationSpeed / 4);
        refresh();
        i--;
    }
    while (i != 0);
}

void DrawMap()
{
    if ((EndLevel == true) || (GetAsyncKeyState(VK_R)) )
    {
        reset();
        CheckLevel(level);
    }
    PLAYSA = false;
    clear();
    // set color
    SetMapColors();
    // bottom boarder colors
    // display active leming and get movement
    for (int y = 0; y < 5; y++)
    {
        for (int x = 0; x < 78; x++)
        {
            switch (QueueAS[0][y][x])
            {
            case ' ':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(1)| A_BOLD);
            }
            break;
            case '-':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(4)| A_BOLD);
            }
            break;
            case '|':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(4)| A_BOLD);
            }
            break;
            case 'i':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(2)| A_BOLD);
            }
            break;
            case ']':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(5)| A_BOLD);
            }
            break;
            case '[':
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(5)| A_BOLD);
            }
            break;
            default:
            {
                mvaddch (y+18,x,QueueAS[0][y][x]|COLOR_PAIR(1)| A_BOLD);
            }
            break;
            }
        }
    }

    // if key was held
    if (AddToQueue == true)
    {
        if (lemcount != 14)
        {
            if (sellem == 0)
            {
                Queue[lemcount] = 'x';
                QueTypes[lemcount] = "Exploder";
                lemcount++;
            }
            else if (sellem == 1)
            {
                Queue[lemcount] = 'd';
                QueTypes[lemcount] = "Digger";
                lemcount++;
            }
            else if (sellem == 2)
            {
                Queue[lemcount] = 'a';
                QueTypes[lemcount] = "Ladder Builder";
                lemcount++;
            }
            else if (sellem == 3)
            {
                Queue[lemcount] = 'b';
                QueTypes[lemcount] = "Bridge Builder";
                lemcount++;
            }
            else if (sellem == 4)
            {
                Queue[lemcount] = 'w';
                QueTypes[lemcount] = "Wall Maker";
                lemcount++;
            }
            else if (sellem == 5)
            {
                Queue[lemcount] = 'p';
                QueTypes[lemcount] = "Pusher";
                lemcount++;
            }
            else if (sellem == 6)
            {
                Queue[lemcount] = 'n';
                QueTypes[lemcount] = "Normal";
                lemcount++;
            }
            else if (sellem == 7)
            {
                Queue[lemcount] = 'g';
                QueTypes[lemcount] = "Gold";
                lemgamecount = 0;
                Droplem = true;
                lemcount++;
            }
        }
        if (sellem == 8)
        {
            if (lemcount <= 0)
            {
                lemcount = 0;
            }
            else
            {
                lemcount--;
            }
            Queue[lemcount] = ' ';
            QueTypes[lemcount] = "None";
        }
        HoldTime = 0;
        AHeld = false;
        AddToQueue = false;
        Sleep(GameSpeed);
    }
    // get key if game aint started
    if (StartGame == false)
    {
        if (GetAsyncKeyState(VK_LEFT))
        {
            revLemSA();
            sellem--;
        }
        if (GetAsyncKeyState(VK_RIGHT))
        {
            LemSA();
            sellem++;
        }
        if (GetAsyncKeyState(VK_SPACE))
        {
            AddToQueue = true;
        }
    }
    // display lemming queue
    //mvprintw (21,2,"Queue : ");
    mvprintw(21,2,"Queue : ");
    for (int i = 0; i < 14; i++)
    {
        if (Queue[i] != 0)
        {
            mvaddch (21,10+i,Queue[i]);
        }
    }

    // start game when gold lemming is droped, and drop first lemming
    if (Droplem == true)
    {
        StartGame = true;
        for (int y = 0; y < 18; y++)
        {
            for (int x = 0; x < 78; x++)
            {
                switch (Map[y][x])
                {
                case 'L':
                {
                    lems_ingame[lemgamecount].chara = Queue[lemgamecount];
                    Queue[lemgamecount] = ' ';
                    lems_ingame[lemgamecount].x = x;
                    lems_ingame[lemgamecount].y = (y+1);
                    lems_ingame[lemgamecount].type = QueTypes[lemgamecount];
                    displaylem = true;
                    PlaySound1("LETSGO.WAV");
                    Sleep(GameSpeed + 1000);
                    //mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x,lems_ingame[lemgamecount].chara|COLOR_PAIR(2)|A_BOLD);
                    lemdropcount = 0;
                    CanDig = false;
                    Digging = false;
                    CanBuildBridge = false;
                    BuildingBridge = false;
                    CanBuildLadder = false;
                    BuildingLadder = false;
                    HoldTime = 0;
                    AHeld = false;
                    OnLadder = false;
                    IsPushing = false;
                    LadderCount = 0;
                    flushinp();
                }
                break;
                }
            }
        }
        Droplem = false;
    }
    if (StartGame == false)
    {
        mvprintw(23,1,"Use Arrow Keys To Pick A Lemming, Press Space To Add It To The Queue");
        // test sel limits
        if (sellem > 8)
        {
            sellem = 0;
        }
        if (sellem < 0)
        {
            sellem = 8;
        }
        // display selected lemming name
        if (sellem == 0)
        {
            mvprintw(21,47, "Exploder Lemming" );
            mvprintw(24,1,"Description: Will Destroy Breakable Blocks when the Space Button is pressed.");
        }

        if (sellem == 1)
        {
            mvprintw(21,47, "Digger Lemming" );
            mvprintw(24,1,"Description: Activate Digger Lemming to dig thorugh the next breakable block. ");
        }

        if (sellem == 2)
        {
            mvprintw(21,47, "Ladder Builder" );
            mvprintw(24,1,"Description: Will Go Upwards 5 Units A Ladder when the Space Button is pressed.");
        }

        if (sellem == 3)
        {
            mvprintw(21,47, "Bridge Builder Lemming" );
            mvprintw(24,1,"Description: Activate Bridge Builder to build a block below 2 units or more.");
        }

        if (sellem == 4)
        {
            mvprintw(21,47, "Wall Maker Lemming" );
            mvprintw(24,1,"Description: Will change into a 2 unit wall when the Space Button is pressed.");
        }

        if (sellem == 5)
        {
            mvprintw(21,47, "Pusher Lemming" );
            mvprintw(24,1,"Description: This Lemming can push red blocks onto switches.");
        }

        if (sellem == 6)
        {
            mvprintw(21,47, "Normal Lemming" );
            mvprintw(24,1,"Description: This Lemming does nothing special, can be used to test the path.");
        }

        if (sellem == 7)
        {
            mvprintw(21,47, "Gold Lemming" );
            mvprintw(24,1,"Description: Start The Game, THIS LEMMING MUST MAKE IT TO THE EXIT TO WIN!");
        }

        if (sellem == 8)
        {
            mvprintw(21,47, "Remove Lemming" );
            mvprintw(24,1,"Description: Remove last lemming added to the queue");
        }
    }
    // display active leming and get movement
    if (StartGame == true)
    {
        if (displaylem == true)
        {
            Movement();
            mvprintw(23,1,"Press Space Button To Activate Current Lemming, R Button To Restart");
            mvprintw(19,47,"Current: %s",lems_ingame[lemgamecount].type.c_str());
            mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x,lems_ingame[lemgamecount].chara|COLOR_PAIR(2)|A_BOLD);
        }
    }
    refresh();
    if (PLAYSA == false)
    {
        napms(GameSpeed);
    }
}

int byebyelem()
{
    int ani = 0;
    for (int i = 10; i > 0; i--)
    {
        if (ani == 0)
        {
            mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x,'-'|COLOR_PAIR(2)|A_BOLD);
            ani = 1;
        }
        else if (ani == 1)
        {
            mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x,'|'|COLOR_PAIR(2)|A_BOLD);
            ani = 0;
        }
        refresh();
        napms(AnimationSpeed / 2);
    }
    DrawBG(bgnum);
    displaylem = false;
    refresh();
}

void Movement()
{
    // gravity
    int gv = (lems_ingame[lemgamecount].y+1);
    int gx = lems_ingame[lemgamecount].x;
    switch (Map[gv][gx])
    {
    case' ':
    {
        int gv2 = (gv+1);
        if (Map[gv2][gx] == ' ')
        {
            if (CanBuildBridge == true)
            {
                BuildingBridge = true;
                if (BuildingBridge == true)
                {
                    if (move_dir == 1)
                    {
                        if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-1] == 'N')
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            byebyelem();
                            Droplem = true;
                            lemgamecount++;
                        }
                        else if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-1] == 'B')
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            byebyelem();
                            Droplem = true;
                            lemgamecount++;
                        }
                        else
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            //Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-1] = 'B';
                        }
                    }
                    else if (move_dir == 0)
                    {
                        if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+1] == 'N')
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            byebyelem();
                            Droplem = true;
                            lemgamecount++;
                        }
                        else if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+1] == 'B')
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            byebyelem();
                            Droplem = true;
                            lemgamecount++;
                        }
                        else
                        {
                            Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = 'B';
                            SetMapColors();
                            //Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+1] = 'B';
                        }
                    }
                }
            }
            else
            {
                InAir = true;
                lems_ingame[lemgamecount].y++;
            }
        }
        else
        {
            InAir = true;
            lems_ingame[lemgamecount].y++;
        }
        lemdropcount++;
    }
    break;
    case'*':
    {
        if (lems_ingame[lemgamecount].type == "Gold")
        {
            reset();
            CheckLevel(level);
            PlaySound1("FIRE.WAV");
            Sleep(GameSpeed);
        }
        else
        {
            Droplem = true;
            lemgamecount++;
            PlaySound1("FIRE.WAV");
            Sleep(GameSpeed);
        }
    }
    break;
    default:
    {
        InAir = false;
        if (lemdropcount > 8)
        {
            Droplem = true;
            lemgamecount++;
            PlaySound1("SPLAT.WAV");
            Sleep(GameSpeed);
            lemdropcount = 0;
        }
        else
        {
            lemdropcount = 0;
        }
    }
    break;
    }
    // if lemming is a bomber
    //ClearKey();
    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Exploder")
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                mvaddch(lems_ingame[lemgamecount].y-1,lems_ingame[lemgamecount].x,'*'|COLOR_PAIR(4)); // above
                mvaddch(lems_ingame[lemgamecount].y+1,lems_ingame[lemgamecount].x,'*'|COLOR_PAIR(4)); // below
                mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x-1,'*'|COLOR_PAIR(4)); // left
                mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x+1,'*'|COLOR_PAIR(4)); // right
                mvaddch(lems_ingame[lemgamecount].y-1,lems_ingame[lemgamecount].x-1,'*'|COLOR_PAIR(4)); // above left
                mvaddch(lems_ingame[lemgamecount].y+1,lems_ingame[lemgamecount].x-1,'*'|COLOR_PAIR(4)); // below left
                mvaddch(lems_ingame[lemgamecount].y-1,lems_ingame[lemgamecount].x+1,'*'|COLOR_PAIR(4)); // above right
                mvaddch(lems_ingame[lemgamecount].y+1,lems_ingame[lemgamecount].x+1,'*'|COLOR_PAIR(4)); // below right
                mvaddch(lems_ingame[lemgamecount].y+2,lems_ingame[lemgamecount].x+1,'*'|COLOR_PAIR(4)); // below 2 right 1
                mvaddch(lems_ingame[lemgamecount].y-2,lems_ingame[lemgamecount].x+1,'*'|COLOR_PAIR(4)); // above 2 right 1
                mvaddch(lems_ingame[lemgamecount].y+2,lems_ingame[lemgamecount].x-1,'*'|COLOR_PAIR(4)); // below 2 left 1
                mvaddch(lems_ingame[lemgamecount].y-2,lems_ingame[lemgamecount].x-1,'*'|COLOR_PAIR(4)); // above 2 left 1
                mvaddch(lems_ingame[lemgamecount].y-2,lems_ingame[lemgamecount].x,'*'|COLOR_PAIR(4)); // above 2
                mvaddch(lems_ingame[lemgamecount].y+2,lems_ingame[lemgamecount].x,'*'|COLOR_PAIR(4)); // below 2
                mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x-2,'*'|COLOR_PAIR(4)); // left 2
                mvaddch(lems_ingame[lemgamecount].y,lems_ingame[lemgamecount].x+2,'*'|COLOR_PAIR(4)); // right 2
                mvaddch(lems_ingame[lemgamecount].y-1,lems_ingame[lemgamecount].x-2,'*'|COLOR_PAIR(4)); // above left 2
                mvaddch(lems_ingame[lemgamecount].y+1,lems_ingame[lemgamecount].x-2,'*'|COLOR_PAIR(4)); // below left 2
                mvaddch(lems_ingame[lemgamecount].y-1,lems_ingame[lemgamecount].x+2,'*'|COLOR_PAIR(4)); // above right 2
                mvaddch(lems_ingame[lemgamecount].y+1,lems_ingame[lemgamecount].x+2,'*'|COLOR_PAIR(4)); // below right 2

                if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] == 'B')   // remove above
                {
                    Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] == 'B')   // remove below
                {
                    Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x-1] == 'B')   // remove left
                {
                    Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x-1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x+1] == 'B')   // remove right
                {
                    Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x+1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x-1] == 'B')   // remove above left
                {
                    Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x-1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-1] == 'B')   // remove below left
                {
                    Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x+1] == 'B')   // remove above right
                {
                    Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x+1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+1] == 'B')   // remove below roght
                {
                    Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+1] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x] == 'B')   // remove above
                {
                    Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x] == 'B')   // remove below
                {
                    Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x-2] == 'B')   // remove left
                {
                    Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x-2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x+2] == 'B')   // remove right
                {
                    Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x+2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x-2] == 'B')   // remove above left
                {
                    Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x-2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x-2] == 'B')   // remove below left
                {
                    Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x-2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x+2] == 'B')   // remove above right
                {
                    Map[lems_ingame[lemgamecount].y-2][lems_ingame[lemgamecount].x+2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x+2] == 'B')   // remove below roght
                {
                    Map[lems_ingame[lemgamecount].y+2][lems_ingame[lemgamecount].x+2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+2] == 'B')   // remove below 1 right 2
                {
                    Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x+2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x+2] == 'B')   // remove above 1 right 2
                {
                    Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x+2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-2] == 'B')   // remove below 1 left 2
                {
                    Map[lems_ingame[lemgamecount].y+1][lems_ingame[lemgamecount].x-2] = ' ';
                }
                if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x-2] == 'B')   // remove above 1 left 2
                {
                    Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x-2] = ' ';
                }
                Droplem = true;
                lemgamecount++;
                PlaySound1("BANG.WAV");
                Sleep(GameSpeed);
            }
            // get a key
        }
    }
    // if active lemming is a Digger
    if (lems_ingame[lemgamecount].type == "Digger")
    {
        if (CanDig == false)
        {
            mvprintw (20,2,"Does Action : No");
        }
        else
        {
            mvprintw (20,2,"Does Action : Yes");
        }
    }

    if (lems_ingame[lemgamecount].type == "Bridge Builder")
    {
        if (CanBuildBridge == false)
        {
            mvprintw (20,2,"Does Action : No");
        }
        else
        {
            mvprintw (20,2,"Does Action : Yes");
        }
    }

    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Digger")
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                if (CanDig == true)
                        {
                            CanDig = false;
                        }
                        else
                        {
                            CanDig = true;
                        }
            }
        }
    }
    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Wall Maker")
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] = 'N';
                Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x] = 'N'; // add blocks
                Droplem = true;
                lemgamecount++;
                PlaySound1("YIPPEE.WAV");
                Sleep(GameSpeed);
            }
        }
    }
    if (lems_ingame[lemgamecount].type == "Ladder Builder")
    {
        if (CanBuildLadder == false)
        {
            mvprintw (20,2,"Does Action : No");
        }
        else
        {
            mvprintw (20,2,"Does Action : Yes");
        }
    }
    // if active lemming is a Bridge Builder
    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Bridge Builder")
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                if (CanBuildBridge == true)
                        {
                            CanBuildBridge = false;
                        }
                        else
                        {
                            CanBuildBridge = true;
                        }
                        if (BuildingBridge == true)
                        {
                            byebyelem();
                            Droplem = true;
                            lemgamecount++;
                        }
            }
        }
    }
    if (GetAsyncKeyState(VK_R))
    {
        reset();
        CheckLevel(level);
    }
    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Ladder Builder")
        {
            if (GetAsyncKeyState(VK_SPACE) != 0)
            {
                if (CanBuildLadder == true)
                {
                    BuildingLadder = true;
                    CanBuildLadder = false;
                }
                else
                {
                    CanBuildLadder = true;
                }
            }
        }
    }

    if (InAir == false)
    {
        if (lems_ingame[lemgamecount].type == "Pusher")
        {
            if (GetAsyncKeyState(VK_SPACE))
            {
                byebyelem();
                Droplem = true;
                lemgamecount++;
            }
        }
    }

    if (CanBuildLadder == true)
    {
        int y2 = lems_ingame[lemgamecount].y-1;
        if (Map[y2][lems_ingame[lemgamecount].x] == ' ')
        {
            if (LadderCount < 5)
            {
                Map[lems_ingame[lemgamecount].y][lems_ingame[lemgamecount].x] = '-';
                Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] = '-';
                LadderCount++;
            }
            else
            {
                BuildingLadder = true;
            }
        }
        else
        {
            BuildingLadder = true;
        }
        lems_ingame[lemgamecount].y--;
    }
    if (BuildingLadder == true)
    {
        byebyelem();
        Droplem = true;
        lemgamecount++;
        CanBuildLadder = false;
        BuildingLadder = false;
    }
    else if ( CanBuildLadder == false)
    {
        if (OnLadder == true)
        {
            if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] == ' ')
            {
                lems_ingame[lemgamecount].y--;
                OnLadder = false;
            }
            else if (Map[lems_ingame[lemgamecount].y-1][lems_ingame[lemgamecount].x] == '-')
            {
                lems_ingame[lemgamecount].y--;
            } else { OnLadder = false; }
        }
        else
        {
            // move right
            if (InAir == false)
            {
                if (move_dir == 0)
                {
                    int x2 = (lems_ingame[lemgamecount].x+1);
                    int y = lems_ingame[lemgamecount].y;
                    switch (Map[y][x2])
                    {
                    case ' ':
                    {
                        // move right
                        if (Digging == true)
                        {
                            byebyelem();
                            CanDig = false;
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                            Digging = false;
                        }
                        else
                        {
                            lems_ingame[lemgamecount].x++;
                        }
                    }
                    break;
                    case 'C':
                    {
                        if (lems_ingame[lemgamecount].type == "Pusher")
                        {
                            IsPushing = true;
                            int y2 = (lems_ingame[lemgamecount].y+1);
                            int x2 = (lems_ingame[lemgamecount].x+1);
                            if (Map[y2][x2] == ' ')
                            {
                                Map[lems_ingame[lemgamecount].y][x2] = ' ';
                                Map[y2][x2] = 'C';
                            }
                            else
                            {
                                int x2 = (lems_ingame[lemgamecount].x+1);
                                int x22 = (lems_ingame[lemgamecount].x+2);
                                Map[lems_ingame[lemgamecount].y][x2] = ' ';
                                Map[lems_ingame[lemgamecount].y][x22] = 'C';
                                //lems_ingame[lemgamecount].x++;
                                //refresh();
                                napms(GameSpeed);
                            }
                        }
                        else
                        {
                            int y22 = (y-1);
                            int x22 = (lems_ingame[lemgamecount].x+1);
                            // above right
                            if (Map[y22][x22] == ' ')
                            {
                                lems_ingame[lemgamecount].y--;
                                lems_ingame[lemgamecount].x++;
                                move_dir = 0;
                            }
                            else
                            {
                                move_dir = 1;
                            }
                        }
                    }
                    break;
                    case '0':
                    {
                        if (lems_ingame[lemgamecount].type == "Gold")
                        {
                            StartGame = false;
                            level++;
                            reset();
                            CheckLevel(level);
                        }
                        else
                        {
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }
                    }
                    break;
                    case 'D':
                    {
                        if (HIDEDOOR == true)
                        {
                            lems_ingame[lemgamecount].x++;
                        }
                        else
                        {
                            move_dir = 1;
                        }
                    }
                    break;
                    case 'H':
                    {
                        if (HIDEDOOR2 == true)
                        {
                            lems_ingame[lemgamecount].x++;
                        }
                        else
                        {
                            move_dir = 1;
                        }
                    }
                    break;
                    case '-':
                    {
                        lems_ingame[lemgamecount].x++;
                        OnLadder = true;
                    }
                    break;
                    case 'B':
                    {
                        // dig if a has been pressed
                        if (CanDig == true)
                        {
                            lems_ingame[lemgamecount].x++;
                            Map[y][x2] = ' ';
                            Digging = true;
                        }
                        else
                        {
                            move_dir = 1;
                        }
                    }
                    break;
                    default:
                    {
                        OnLadder = false;
                        /*if (BuildingBridge == true)
                        {
                            CanBuildBridge = false;
                            Droplem = true;
                            lemgamecount++;
                            BuildingBridge = false;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }*/
                        if (Digging == true)
                        {
                            byebyelem();
                            CanDig = false;
                            Digging = false;
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }
                        else
                        {
                            move_dir = 1;
                        }
                        int y22 = (y-1);
                        int x22 = (lems_ingame[lemgamecount].x+1);
                        // above right
                        if (Map[y22][x22] == ' ')
                        {
                            lems_ingame[lemgamecount].y--;
                            lems_ingame[lemgamecount].x++;
                            move_dir = 0;
                        }
                        else
                        {
                            move_dir = 1;
                        }
                    }
                    break;
                    }
                }
                else if (move_dir == 1)
                {
                    int x2 = (lems_ingame[lemgamecount].x-1);
                    int y = lems_ingame[lemgamecount].y;
                    switch (Map[y][x2])
                    {
                    case ' ':
                    {
                        // move right
                        if (Digging == true)
                        {
                            byebyelem();
                            CanDig = false;
                            Digging = false;
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }
                        else
                        {
                            lems_ingame[lemgamecount].x--;
                        }
                    }
                    break;
                    case '0':
                    {
                        if (lems_ingame[lemgamecount].type == "Gold")
                        {
                            StartGame = false;
                            level++;
                            reset();
                            CheckLevel(level);
                        }
                        else
                        {
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }
                    }
                    break;
                    case 'C':
                    {
                        if (lems_ingame[lemgamecount].type == "Pusher")
                        {
                            IsPushing = true;
                            int y2 = (lems_ingame[lemgamecount].y+1);
                            int x2 = (lems_ingame[lemgamecount].x-1);
                            if (Map[y2][x2] == ' ')
                            {
                                Map[lems_ingame[lemgamecount].y][x2] = ' ';
                                Map[y2][x2] = 'C';
                            }
                            else
                            {
                                int x2 = (lems_ingame[lemgamecount].x-1);
                                int x22 = (lems_ingame[lemgamecount].x-2);
                                Map[lems_ingame[lemgamecount].y][x2] = ' ';
                                Map[lems_ingame[lemgamecount].y][x22] = 'C';
                                lems_ingame[lemgamecount].x--;
                                refresh();
                                napms(GameSpeed);
                            }
                        }
                        else
                        {
                            int y22 = (y-1);
                            int x22 = (lems_ingame[lemgamecount].x-1);
                            // above right
                            if (Map[y22][x22] == ' ')
                            {
                                lems_ingame[lemgamecount].y--;
                                lems_ingame[lemgamecount].x--;
                                move_dir = 1;
                            }
                            else
                            {
                                move_dir = 0;
                            }
                        }
                    }
                    break;
                    case '-':
                    {
                        lems_ingame[lemgamecount].x--;
                        OnLadder = true;
                    }
                    break;
                    case 'D':
                    {
                        if (HIDEDOOR == true)
                        {
                            lems_ingame[lemgamecount].x--;
                        }
                        else
                        {
                            move_dir = 0;
                        }
                    }
                    break;
                    case 'H':
                    {
                        if (HIDEDOOR2 == true)
                        {
                            lems_ingame[lemgamecount].x--;
                        }
                        else
                        {
                            move_dir = 0;
                        }
                    }
                    break;
                    case 'B':
                    {
                        // dig if a has been pressed
                        if (CanDig == true)
                        {
                            lems_ingame[lemgamecount].x--;
                            Map[y][x2] = ' ';
                            Digging = true;
                        }
                        else
                        {
                            move_dir = 0;
                        }
                    }
                    break;
                    default:
                    {
                        OnLadder = false;
                        /*if (BuildingBridge == true)
                        {
                            CanBuildBridge = false;
                            Droplem = true;
                            lemgamecount++;
                            BuildingBridge = false;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }*/
                        if (Digging == true)
                        {
                            byebyelem();
                            CanDig = false;
                            Digging = false;
                            Droplem = true;
                            lemgamecount++;
                            PlaySound1("YIPPEE.WAV");
                            Sleep(GameSpeed);
                        }
                        else
                        {
                            move_dir = 0;
                        }
                        int y22 = (y-1);
                        int x22 = (lems_ingame[lemgamecount].x-1);
                        // above right
                        if (Map[y22][x22] == ' ')
                        {
                            lems_ingame[lemgamecount].y--;
                            lems_ingame[lemgamecount].x--;
                            move_dir = 1;
                        }
                        else
                        {
                            move_dir = 0;
                        }
                    }
                    break;
                    }
                }
            }
        }
    }
}
