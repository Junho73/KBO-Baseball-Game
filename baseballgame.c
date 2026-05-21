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

    GotoXY(4, 6);  printf("■ 이름 : %s", p->name);
    GotoXY(4, 8);  printf("■ 신체 : %d cm / %d kg", p->height, p->weight);

    GotoXY(4, 11); printf("[ 현재 능력치 ]");
    GotoXY(4, 12); printf(" - 파워   (Power)   : %d", p->power);
    GotoXY(4, 13); printf(" - 민첩   (Agility) : %d", p->agility);
    GotoXY(4, 14); printf(" - 지구력 (Stamina) : %d", p->stamina);

    GotoXY(4, 17); printf("[ 보유 자산 ]");
    GotoXY(4, 18); printf(" - 훈련 포인트      : %d P", p->points);

    DrawPlayerModel(p->height, p->weight, 32, 6);

    GotoXY(2, 24); printf("==================================================");
    GotoXY(2, 25); printf("아무 키나 누르면 클럽 하우스로 돌아갑니다...");

    _getch();
}

void PointShop(Player* p) {
    int cursor = 0;
    int key;
    bool messageTrigger = false;
    char message[50] = "";

    while (true) {
        system("cls");
        HideCursor();

        GotoXY(2, 2);  printf("======================================================");
        GotoXY(2, 3);  printf("            [ 훈련 센터 & 포인트 상점 ]               ");
        GotoXY(2, 4);  printf("======================================================");

        GotoXY(4, 6);  printf("?? 보유 포인트 : %d P", p->points);
        GotoXY(4, 7);  printf("------------------------------------------------------");

        GotoXY(4, 9);  printf("[ 현재 능력치 및 게임 적용 보너스 ]");
        GotoXY(4, 10); printf(" 파워   : %3d (장타 보너스 Lv.%d)", p->power, p->power / 10);
        GotoXY(4, 11); printf(" 민첩   : %3d (타이밍 보너스 Lv.%d)", p->agility, p->agility / 10);
        GotoXY(4, 12); printf(" 지구력 : %3d (체력 감소 방어 Lv.%d)", p->stamina, p->stamina / 10);
        GotoXY(4, 13); printf(" * 팁: 스탯 10마다 인게임 확률이 비약적으로 상승합니다!");

        GotoXY(4, 15); printf("------------------------------------------------------");

        GotoXY(6, 17); printf("%s 1. 파워 향상 특훈   (+1 파워)   [ 10 P ]", (cursor == 0) ? "▶" : "  ");
        GotoXY(6, 18); printf("%s 2. 민첩 향상 특훈   (+1 민첩)   [ 10 P ]", (cursor == 1) ? "▶" : "  ");
        GotoXY(6, 19); printf("%s 3. 지구력 향상 특훈 (+1 지구력) [ 10 P ]", (cursor == 2) ? "▶" : "  ");
        GotoXY(6, 20); printf("%s 4. [도박] 랜덤 비약 (+3 랜덤)   [ 10 P ]", (cursor == 3) ? "▶" : "  ");
        GotoXY(6, 21); printf("%s 5. 클럽 하우스로 돌아가기", (cursor == 4) ? "▶" : "  ");

        if (messageTrigger) {
            GotoXY(4, 23); printf(">> %s", message);
            messageTrigger = false;
        }

        GotoXY(2, 25); printf("======================================================");

        key = _getch();
        if (key == 224) {
            key = _getch();
            if (key == 72) { cursor--; if (cursor < 0) cursor = 4; }
            else if (key == 80) { cursor++; if (cursor > 4) cursor = 0; }
        }
        else if (key == 32 || key == 13) {
            if (cursor == 4) return;

            if (p->points < 10) {
                sprintf(message, "포인트가 부족합니다! (현재 %d P)", p->points);
                messageTrigger = true;
                continue;
            }

            if (cursor == 0) {
                if (p->power >= 100) { sprintf(message, "파워가 이미 최대치(100)입니다!"); messageTrigger = true; }
                else { p->points -= 10; p->power++; sprintf(message, "파워가 1 상승했습니다!"); messageTrigger = true; }
            }
            else if (cursor == 1) {
                if (p->agility >= 100) { sprintf(message, "민첩이 이미 최대치(100)입니다!"); messageTrigger = true; }
                else { p->points -= 10; p->agility++; sprintf(message, "민첩이 1 상승했습니다!"); messageTrigger = true; }
            }
            else if (cursor == 2) {
                if (p->stamina >= 100) { sprintf(message, "지구력이 이미 최대치(100)입니다!"); messageTrigger = true; }
                else { p->points -= 10; p->stamina++; sprintf(message, "지구력이 1 상승했습니다!"); messageTrigger = true; }
            }
            else if (cursor == 3) {
                if (p->power == 100 && p->agility == 100 && p->stamina == 100) {
                    sprintf(message, "모든 스탯이 최대치입니다!"); messageTrigger = true;
                }
                else {
                    p->points -= 10;
                    int added = 0;
                    while (added < 3) {
                        int r = rand() % 3;
                        if (r == 0 && p->power < 100) { p->power++; added++; }
                        else if (r == 1 && p->agility < 100) { p->agility++; added++; }
                        else if (r == 2 && p->stamina < 100) { p->stamina++; added++; }

                        if (p->power == 100 && p->agility == 100 && p->stamina == 100) break;
                    }
                    sprintf(message, "신비한 비약을 마시고 총 %d 스탯이 올랐습니다!", added);
                    messageTrigger = true;
                }
            }
        }
    }
}

// 각 팀의 실제 데이터를 반환하는 함수 (데이터베이스 역할)
TeamInfo GetTeamInfo(int index) {
    TeamInfo teams[10] = {
        {"기아 타이거즈", 0, "이범호", "양현종", "김도영"},     // 0: 하위 (우쭈쭈)
        {"삼성 라이온즈", 1, "박진만", "원태인", "구자욱"},     // 1: 상위 (경쟁)
        {"LG 트윈스", 1, "염경엽", "엔스", "오지환"},           // 2: 상위 (경쟁)
        {"두산 베어스", 0, "이승엽", "곽빈", "양의지"},         // 3: 하위 (우쭈쭈)
        {"KT 위즈", 0, "이강철", "고영표", "강백호"},           // 4: 하위 (우쭈쭈)
        {"SSG 랜더스", 1, "이숭용", "김광현", "최정"},          // 5: 상위 (경쟁)
        {"롯데 자이언츠", 0, "김태형", "박세웅", "전준우"},     // 6: 하위 (우쭈쭈)
        {"한화 이글스", 1, "김경문", "류현진", "노시환"},       // 7: 상위 (경쟁)
        {"NC 다이노스", 1, "강인권", "하트", "박민우"},         // 8: 상위 (경쟁)
        {"키움 히어로즈", 0, "홍원기", "후라도", "김혜성"}      // 9: 하위 (우쭈쭈)
    };
    return teams[index];
}

void ShowPreGameStory(Player* p, int teamIndex) {
    TeamInfo t = GetTeamInfo(teamIndex); 

    system("cls");
    HideCursor();

    GotoXY(4, 4); printf("======================================================");
    GotoXY(4, 5); printf("               [ %s 감독실 ]               ", t.teamName);
    GotoXY(4, 6); printf("======================================================");

    GotoXY(4, 8); printf("■ %s 감독이 당신을 바라보며 입을 연다.", t.managerName);

    if (t.isTopTier == 1) {
        GotoXY(4, 10); printf("\"%s 선수, 우리 팀은 현재 왕조를 노리는 강팀이네.\"", p->name);
        GotoXY(4, 11); printf("\"투수조에는 %s, 타자조에는 %s 같은 리그 최고의 에이스들이 있지.\"", t.acePitcher, t.aceBatter);
        GotoXY(4, 12); printf("\"자네가 '이도류'라는 건 알지만, 이 선배들과의 험난한 경쟁에서 살아남아야 할 거야.\"");
        GotoXY(4, 13); printf("\"너의 역량을 이번 경기에서 직접 증명해 보이도록 해!\"");
    }
    else {
        GotoXY(4, 10); printf("\"%s 선수! 드디어 와주었군. 우리 팀의 구세주가 되어주게!\"", p->name);
        GotoXY(4, 11); printf("\"현재 %s 선수가 마운드에서, %s 선수가 타석에서 고군분투하고 있지만 역부족이야.\"", t.acePitcher, t.aceBatter);
        GotoXY(4, 12); printf("\"자네의 그 압도적인 '이도류' 재능이 우리 팀을 더 높은 곳으로 이끌어 줄 거라 믿어 의심치 않네.\"");
        GotoXY(4, 13); printf("\"다치지 말고, 부담 없이 자네의 플레이를 마음껏 펼쳐보게나!\"");
    }

    GotoXY(4, 16); printf("------------------------------------------------------");
    GotoXY(4, 17); printf(" >> 결의를 다지며 감독실을 나서 그라운드로 향한다...");

    GotoXY(4, 20); printf("아무 키나 누르면 첫 번째 타석/마운드로 입장합니다...");
    _getch();
}