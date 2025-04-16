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
#include <d2d1.h>             // �⺻ D2D (������)
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
	�����Ϸ����� �ش� �ڵ带 �ڿ� ���ǵ� �ڵ�� �����Ѵ�. 
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
	extern Ű���� : ������ �Լ��� �ٸ� ���Ͽ� ���ǵǾ� �ִ� ���
	����� �˸��� Ű����.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;


enum class PlayerState {
    IDLE,               // ���� ����
    MOVE,               // �¿� �̵�
    
    LOOKUP_START,       // �� ���� ����
    LOOKUP_IDLE,        // ���� �ִ�� �� ���� ����
    LOOKUP_RELEASE,     // �� ���� ���� ����
 
    LOOKDOWN_START,     // �Ʒ� ���� ����
    LOOKDOWN_IDLE,      // ���帰 ä�� ������ ����
    LOOKDOWN_RELEASE,   // �Ʒ� ���� ���� ����
    LOOKDOWN_MOVE,      // �Ʒ� �� ���·� �̵�
    
    CLIMB_LADDER,       // ��ٸ� ������
    CLIMB_ROPE,         // ���� ������
    
    ON_NOTTAMEDPET,     // �� ������  �� ��������
    ONPET_IDLE,        // �� Ÿ�� ���� ����
    ONPET_MOVE,        // �� Ÿ�� �̵�
    
    ONPET_LOOKDOWN_IDLE,    // �� ������ �Ʒ� ����
    ONPET_LOOKDOWN_START,    // �� ������ �Ʒ� ����
    ONPET_LOOKDOWN_RELEASE,    // �� ������ �Ʒ� ����
   
    ONPET_LOOKUP_START,      // �� ������ �� ����
    ONPET_LOOKUP_RELEASE,      // �� ������ �� ����

    ATTACK,             // ���� or ��ô ���� (ä�� ��)
    
    ALMOST_FALL,        // Ÿ�� �𼭸� ��������
    HANG,               // Ÿ�� ���� �Ŵ޸�
    HURT,               // ������ �԰� ���� ����
    DIE,                // ���
    
    FALL,               // �ٴڿ� �΋H�� -> �΋H�� �Ŀ� DIE + ���� �� �յ� -> LOOKDOWN_IDLE -> �Ͼ 
    FALL_STUNEFFECT,
    
    THROW,              // ������/NPC ������
    HOLD,               // ������/�� ��� �ִ� ����
    PUSH,               // ���ڳ� ��� �б�

    ENTER_TUNNEL,       // �ͳ� ����
    EXIT_TUNNEL,        // �ͳ� ����
    EXIT,               // �������� �ⱸ ����
};

typedef struct tagCurrFrameInfo
{
    POINT startFrame;
	POINT endFrame;
    //POINT initFrame = {-1, -1}; maybe don`t need?
} FrameInfo;