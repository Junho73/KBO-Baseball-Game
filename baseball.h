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
    int currentMatchIndex; // [추가됨] 현재 몇 번째 경기인지 추적 (0~9)
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

// [추가됨] 경기 일정 UI 함수
void ShowSchedule(Player* p, const char* teamName);

#endif