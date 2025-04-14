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
	�����Ϸ����� �ش� �ڵ带 �ڿ� ���ǵ� �ڵ�� �����Ѵ�. 
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
	extern Ű���� : ������ �Լ��� �ٸ� ���Ͽ� ���ǵǾ� �ִ� ���
	����� �˸��� Ű����.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;


enum class PlayerState {
    IDLE,               // ������ �� ����
    MOVE,               // �¿� �̵�
    LOOKUP_START,       // �� ����
    LOOKUP_RELEASE,     // �� ���� ���� ����
    LOOKDOWN_START,     // �Ʒ� ����
    LOOKDOWN_RELEASE,   // �Ʒ� ���� ���� ����
    LOOKDOWN_MOVE,      // �Ʒ� �� ���·� �̵�
    LOOKDOWN_MOVESTOP,
    JUMP,               // ���� ��
    FALL,               // ���� ��
    CLIMB,              // ��ٸ�/�� ������
    ATTACK,             // ���� or ��ô ���� (ä�� ��)
    CROUCH,             // �ɱ�
    HANG,               // ���ǿ� �Ŵ޸�
    HURT,               // ������ �԰� ���� ����
    DIE,                // ���
    THROW,              // ������/NPC ������
    HOLD,               // ������/�� ��� �ִ� ����
    PUSH,               // ���ڳ� ��� �б�
    EXIT,               // �������� �ⱸ ����
};

typedef struct tagCurrFrameInfo
{
    POINT startFrame;
	POINT endFrame;
} CurrFrameInfo;