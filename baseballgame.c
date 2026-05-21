#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "baseball.h"

void GotoXY(int x, int y) {
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void ShowConsoleCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void DrawTitle() {
    int startX = 6;
    int startY = 4;
    GotoXY(startX, startY);
    printf("==================================================\n");
    GotoXY(startX, startY + 1);
    printf("            KBO BASEBALL CHAMPIONSHIP             \n");
    GotoXY(startX, startY + 2);
    printf("==================================================\n");
}

bool WaitStartKey() {
    bool showText = true;
    int i;
    char key;
    while (true) {
        GotoXY(11, 7);
        if (showText) printf(">> Press 'P' to Start Game! <<");
        else printf("                              ");
        showText = !showText;
        for (i = 0; i < 5; i++) {
            if (_kbhit() != 0) {
                key = _getch();
                if (key == 'p' || key == 'P') return true;
            }
            Sleep(100);
        }
    }
    return false;
}

void ShowLoadingScreen() {
    int i;
    system("cls");
    HideCursor();
    DrawTitle();
    if (WaitStartKey()) {
        system("cls");
        GotoXY(13, 10);
        printf("Loading");
        for (i = 0; i < 3; i++) {
            Sleep(400);
            printf(".");
        }
        Sleep(500);
        system("cls");
    }
}

int SelectTeam() {
    const char* teams[10] = {
        "기아 타이거즈", "삼성 라이온즈", "LG 트윈스", "두산 베어스", "KT 위즈",
        "SSG 랜더스", "롯데 자이언츠", "한화 이글스", "NC 다이노스", "키움 히어로즈"
    };
    int currentCursor = 0, startX = 10, startY = 5, i, key;
    system("cls");
    HideCursor();
    GotoXY(startX - 2, startY - 3); printf("==========================================");
    GotoXY(startX, startY - 2);     printf("  드래프트에 참여할 구단을 선택하세요!  ");
    GotoXY(startX - 2, startY - 1); printf("==========================================");

    for (i = 0; i < 10; i++) {
        GotoXY(startX + 2, startY + i);
        printf("%d. %s", i + 1, teams[i]);
    }
    while (true) {
        for (i = 0; i < 10; i++) {
            GotoXY(startX, startY + i); printf("  ");
        }
        GotoXY(startX, startY + currentCursor); printf(">");
        key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) { currentCursor--; if (currentCursor < 0) currentCursor = 9; }
            else if (key == 80) { currentCursor++; if (currentCursor > 9) currentCursor = 0; }
        }
        else if (key == 32 || key == 13) return currentCursor;
    }
}

void DrawPlayerModel(int height, int weight, int x, int y) {
    for (int i = 0; i < 17; i++) {
        GotoXY(x, y + i);
        if (i == 0 || i == 16) printf("----------------------");
        else printf("|                    |");
    }
    int cx = x + 4;
    int cy = y + 2;
    GotoXY(cx, cy);     printf("  ___  ");
    GotoXY(cx, cy + 1); printf(" (o_o) ");
    GotoXY(cx, cy + 2);
    if (weight <= 80)      printf(" / | \\ ");
    else if (weight <= 100) printf(" /|||\\ ");
    else                   printf("/[   ]\\");

    int torsoLength = 1;
    if (height >= 180) torsoLength = 2;
    if (height >= 195) torsoLength = 3;
    if (height >= 205) torsoLength = 4;

    for (int i = 0; i < torsoLength; i++) {
        GotoXY(cx, cy + 3 + i);
        if (weight <= 80)      printf("   |   ");
        else if (weight <= 100) printf("  |||  ");
        else                   printf(" [   ] ");
    }

    int legStartY = cy + 3 + torsoLength;
    for (int i = 0; i < 3; i++) {
        GotoXY(cx, legStartY + i);
        printf("  | |  ");
    }
}

Player CreatePlayer() {
    Player p = { "유망주", 175, 75, 0, 0, 0, 0, 0 };
    int cursor = 0;
    int key;
    bool isStatRolled = false;
    HideCursor();

    while (true) {
        system("cls");
        GotoXY(2, 2); printf("=======================================");
        GotoXY(2, 3); printf("         선수의 스펙을 만드세요!       ");
        GotoXY(2, 4); printf("=======================================");

        GotoXY(2, 6);  printf("%s 1. 이름(Name): %s (스페이스바로 변경)", (cursor == 0) ? ">" : " ", p.name);
        GotoXY(2, 8);  printf("%s 2. 키(Height): %d cm (<-/-> 조절)", (cursor == 1) ? ">" : " ", p.height);
        GotoXY(2, 10); printf("%s 3. 몸무게(Weight): %d kg (<-/-> 조절)", (cursor == 2) ? ">" : " ", p.weight);

        GotoXY(2, 12); printf("%s 4. 스탯 주사위 굴리기 (단축키: D)", (cursor == 3) ? ">" : " ");
        GotoXY(4, 13); printf("   - 파워  (Power):   %d", p.power);
        GotoXY(4, 14); printf("   - 민첩  (Agility): %d", p.agility);
        GotoXY(4, 15); printf("   - 지구력(Stamina): %d", p.stamina);

        GotoXY(2, 17); printf("%s 5. 생성 완료 (스페이스바)", (cursor == 4) ? ">" : " ");
        GotoXY(2, 19); printf("[안내] 방향키 이동, 좌우키 조절, D 주사위 굴림");

        DrawPlayerModel(p.height, p.weight, 25, 4);

        key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 72) { cursor--; if (cursor < 0) cursor = 4; }
            else if (key == 80) { cursor++; if (cursor > 4) cursor = 0; }
            else if (key == 75) {
                if (cursor == 1 && p.height > 165) p.height -= 5;
                if (cursor == 2 && p.weight > 65) p.weight -= 5;
            }
            else if (key == 77) {
                if (cursor == 1 && p.height < 210) p.height += 5;
                if (cursor == 2 && p.weight < 130) p.weight += 5;
            }
        }
        else if (key == 'd' || key == 'D') {
            p.power = (rand() % 6 + 1) + (rand() % 6 + 1);
            p.agility = (rand() % 6 + 1) + (rand() % 6 + 1);
            p.stamina = (rand() % 6 + 1) + (rand() % 6 + 1);
            isStatRolled = true;
        }
        else if (key == 32 || key == 13) {
            if (cursor == 0) {
                GotoXY(2, 21); printf("새로운 이름을 입력하세요: ");
                ShowConsoleCursor();
                scanf("%19s", p.name);
                HideCursor();
            }
            else if (cursor == 4) {
                if (isStatRolled) return p;
                else {
                    GotoXY(2, 21); printf("[경고] 주사위를 먼저 굴려주세요!");
                    Sleep(1000);
                }
            }
        }
    }
}

int MainLobby(Player* p, const char* teamName) {
    int cursor = 0;
    int key;

    while (true) {
        system("cls");
        HideCursor();

        GotoXY(5, 2);  printf("==================================================");
        GotoXY(5, 3);  printf("             [%s] 클럽 하우스             ", teamName);
        GotoXY(5, 4);  printf("==================================================");

        GotoXY(5, 6);  printf(" ? 선수: %s (초대형 유망주)", p->name);
        GotoXY(5, 7);  printf(" ?? 보유 포인트: %d P", p->points);
        GotoXY(5, 8);  printf("--------------------------------------------------");

        GotoXY(7, 10); printf("%s 1. 경기 일정 (Schedule)", (cursor == 0) ? "▶" : "  ");
        GotoXY(7, 12); printf("%s 2. 내 스테이터스 (Status)", (cursor == 1) ? "▶" : "  ");
        GotoXY(7, 14); printf("%s 3. 포인트 상점 (Point Shop)", (cursor == 2) ? "▶" : "  ");
        GotoXY(7, 16); printf("%s 4. 다음 경기 시작 (Start Game)", (cursor == 3) ? "▶" : "  ");

        GotoXY(5, 19); printf("--------------------------------------------------");
        GotoXY(5, 20); printf("[안내] 방향키로 이동, 스페이스바/엔터로 선택");

        key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) {
                cursor--; if (cursor < 0) cursor = 3;
            }
            else if (key == 80) {
                cursor++; if (cursor > 3) cursor = 0;
            }
        }
        else if (key == 32 || key == 13) {
            return cursor;
        }
    }
}

void ShowSchedule(Player* p, const char* teamName) {
    int i;
    const char* dates[10] = {
        "03/28 (토) - [개막전] 새로운 시작",
        "04/11 (토) - [원정] 봄날의 연전",
        "05/05 (화) - [홈] 라이벌 매치",
        "06/06 (토) - [원정] 현충일 매치",
        "07/18 (토) - [이벤트] 라이벌 매치",
        "08/15 (토) - [홈] 광복절 매치",
        "09/12 (토) - [원정] 가을야구 쟁탈전",
        "10/03 (토) - [홈] 정규시즌 최종전",
        "10/17 (토) - [포스트시즌] 플레이오프",
        "10/24 (토) - [한국시리즈] 우승을 향해"
    };

    system("cls");
    HideCursor();

    GotoXY(4, 2);  printf("======================================================");
    GotoXY(4, 3);  printf("            [%s] 2026 시즌 핵심 경기 일정             ", teamName);
    GotoXY(4, 4);  printf("======================================================");

    for (i = 0; i < 10; i++) {
        GotoXY(6, 6 + i);
        if (i < p->currentMatchIndex) {
            printf(" [완료] %s", dates[i]);
        }
        else if (i == p->currentMatchIndex) {
            printf(" ▶▶▶ %s (NEXT GAME)", dates[i]);
        }
        else {
            printf(" [대기] %s", dates[i]);
        }
    }

    GotoXY(4, 18); printf("======================================================");
    GotoXY(4, 20); printf("아무 키나 누르면 클럽 하우스로 돌아갑니다...");

    _getch();
}

void ShowStatus(Player* p, const char* teamName) {
    system("cls");
    HideCursor();

    GotoXY(2, 2);  printf("==================================================");
    GotoXY(2, 3);  printf("           [%s] 소속 선수 스테이터스          ", teamName);
    GotoXY(2, 4);  printf("==================================================");

    // 1. 좌측 텍스트 정보 출력
    GotoXY(4, 6);  printf("■ 이름 : %s", p->name);
    GotoXY(4, 8);  printf("■ 신체 : %d cm / %d kg", p->height, p->weight);

    GotoXY(4, 11); printf("[ 현재 능력치 ]");
    GotoXY(4, 12); printf(" - 파워   (Power)   : %d", p->power);
    GotoXY(4, 13); printf(" - 민첩   (Agility) : %d", p->agility);
    GotoXY(4, 14); printf(" - 지구력 (Stamina) : %d", p->stamina);

    GotoXY(4, 17); printf("[ 보유 자산 ]");
    GotoXY(4, 18); printf(" - 훈련 포인트      : %d P", p->points);

    // 2. 우측 캐릭터 모델링 렌더링 (X좌표 32, Y좌표 6)
    DrawPlayerModel(p->height, p->weight, 32, 6);

    GotoXY(2, 24); printf("==================================================");
    GotoXY(2, 25); printf("아무 키나 누르면 클럽 하우스로 돌아갑니다...");

    _getch(); // 키 입력 대기
}