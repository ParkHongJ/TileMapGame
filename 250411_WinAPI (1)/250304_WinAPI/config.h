// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")

#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"

/*
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
*/
#define WINSIZE_X	1080
#define WINSIZE_Y	500
#define TILEMAPTOOL_X	1420
#define TILEMAPTOOL_Y	700

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

#define ANIMATION_FRAME_TIME 0.065f



typedef struct tagFPOINT
{
	float x;
	float y;
} FPOINT;

/*
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;


enum class PlayerState {
    IDLE,               // 가만히 서 있음
    MOVE,               // 좌우 이동
    LOOKUP_START,       // 위 보기
    LOOKUP_RELEASE,     // 위 보기 종료 시작
    LOOKDOWN_START,     // 아래 보기
    LOOKDOWN_RELEASE,   // 아래 보기 종료 시작
    LOOKDOWN_MOVE,      // 아래 본 상태로 이동
    LOOKDOWN_MOVESTOP,
    JUMP,               // 점프 중
    FALL,               // 낙하 중
    CLIMB,              // 사다리/벽 오르기
    ATTACK,             // 근접 or 투척 공격 (채찍 등)
    CROUCH,             // 앉기
    HANG,               // 발판에 매달림
    HURT,               // 데미지 입고 무적 상태
    DIE,                // 사망
    THROW,              // 아이템/NPC 던지기
    HOLD,               // 아이템/적 들고 있는 상태
    PUSH,               // 상자나 블록 밀기
    EXIT,               // 스테이지 출구 도달
};

typedef struct tagCurrFrameInfo
{
    POINT startFrame;
	POINT endFrame;
} CurrFrameInfo;