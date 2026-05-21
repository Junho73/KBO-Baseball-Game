#ifndef BASEBALL_H
#define BASEBALL_H

#include <stdbool.h>

typedef struct {
    char name[20];
    int height;
    int weight;
    int power;
    int agility;
    int stamina;
    int points;
    int currentMatchIndex;
} Player;

typedef struct {
    char teamName[30];
    int isTopTier;         
    char managerName[20];  
    char acePitcher[20];   
    char aceBatter[20];    
} TeamInfo;

void GotoXY(int x, int y);
void HideCursor();
void ShowConsoleCursor();
void DrawTitle();
bool WaitStartKey();
void ShowLoadingScreen();
int SelectTeam();
void DrawPlayerModel(int height, int weight, int x, int y);
Player CreatePlayer();
int MainLobby(Player* p, const char* teamName);
void ShowSchedule(Player* p, const char* teamName);
void ShowStatus(Player* p, const char* teamName);
void PointShop(Player* p);

TeamInfo GetTeamInfo(int index);
void ShowPreGameStory(Player* p, int teamIndex);

#endif