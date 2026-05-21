#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <stdlib.h>
#include "baseball.h"

int main() {
    srand((unsigned int)time(NULL));

    const char* teams[10] = {
        "기아 타이거즈", "삼성 라이온즈", "LG 트윈스", "두산 베어스", "KT 위즈",
        "SSG 랜더스", "롯데 자이언츠", "한화 이글스", "NC 다이노스", "키움 히어로즈"
    };

    ShowLoadingScreen();
    int selectedTeamIndex = SelectTeam();

    Player myPlayer = CreatePlayer();
    myPlayer.points = 100;

    while (true) {
        int choice = MainLobby(&myPlayer, teams[selectedTeamIndex]);

        system("cls");
        switch (choice) {
        case 0:
            ShowSchedule(&myPlayer, teams[selectedTeamIndex]);
            break;
        case 1:
            ShowStatus(&myPlayer, teams[selectedTeamIndex]);
            break;
        case 2:
            PointShop(&myPlayer);
            break;
        case 3:
            GotoXY(5, 5); printf(">> 대망의 [다음 경기 시작] 로직으로 진입합니다! <<");
            break;
        }
        GotoXY(5, 7);
        system("pause");
    }

    return 0;
}