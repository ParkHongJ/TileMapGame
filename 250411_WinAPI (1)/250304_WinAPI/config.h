// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable : 4996)
#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <d2d1.h>             // 기본 D2D (선택적)
#include <wrl/client.h>

#pragma comment(lib, "d2d1.lib")
using Microsoft::WRL::ComPtr;

using namespace std;

#include "KeyManager.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "ObjectManager.h"

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

#define IDLESTATE 0
#define MOVESTATE 1
#define ATTACKSTATE 2
#define INTERACTIONSTATE 3

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
    IDLE,               // 유후 상태
    MOVE,               // 좌우 이동
    
    LOOKUP_START,       // 위 보기 시작
    LOOKUP_IDLE,        // 위를 최대로 본 유후 상태
    LOOKUP_RELEASE,     // 위 보기 종료 시작
 
    LOOKDOWN_START,     // 아래 보기 시작
    LOOKDOWN_IDLE,      // 엎드린 채로 가만히 있음
    LOOKDOWN_RELEASE,   // 아래 보기 종료 시작
    LOOKDOWN_MOVE,      // 아래 본 상태로 이동
    
    CLIMB_LADDER,       // 사다리 오르기
    CLIMB_ROPE,         // 로프 오르기
    
    ON_NOTTAMEDPET,     // 펫 위에서  팔 휘적휘적
    ONPET_IDLE,        // 펫 타고 유후 상태
    ONPET_MOVE,        // 펫 타고 이동
    
    ONPET_LOOKDOWN_IDLE,    // 펫 위에서 아래 보기
    ONPET_LOOKDOWN_START,    // 펫 위에서 아래 보기
    ONPET_LOOKDOWN_RELEASE,    // 펫 위에서 아래 보기
   
    ONPET_LOOKUP_START,      // 펫 위에서 위 보기
    ONPET_LOOKUP_RELEASE,      // 펫 위에서 위 보기

    ATTACK,             // 근접 or 투척 공격 (채찍 등)
    
    ALMOST_FALL,        // 타일 모서리 휘적휘적
    HANG,               // 타일 끝에 매달림
    HURT,               // 데미지 입고 무적 상태
    DIE,                // 사망
    
    FALL,               // 바닥에 부딫힘 -> 부딫힌 후엔 DIE + 위에 새 둥둥 -> LOOKDOWN_IDLE -> 일어남 
    FALL_STUNEFFECT,
    
    THROW,              // 아이템/NPC 던지기
    HOLD,               // 아이템/적 들고 있는 상태
    PUSH,               // 상자나 블록 밀기

    ENTER_TUNNEL,       // 터널 진입
    EXIT_TUNNEL,        // 터널 나옴
    EXIT,               // 스테이지 출구 도달
};

typedef struct tagCurrFrameInfo
{
    POINT startFrame;
	POINT endFrame;
    //POINT initFrame = {-1, -1}; maybe don`t need?
} FrameInfo;