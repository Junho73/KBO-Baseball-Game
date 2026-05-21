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

// [새로 추가됨] 스테이터스 확인 UI 함수
void ShowStatus(Player* p, const char* teamName);

#endif