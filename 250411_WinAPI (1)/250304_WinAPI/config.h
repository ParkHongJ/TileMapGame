// config.h

#pragma once
#pragma comment(lib, "Winmm.lib")
#pragma warning(disable : 4996)
#pragma warning(disable : 4244)
#include <Windows.h>
#include <string>
#include <iostream>
#include <bitset>
#include <map>
#include <vector>
#include <d2d1.h>             // �⺻ D2D (������)
#include <wrl/client.h>

#pragma comment(lib, "d2d1.lib")
#include <dwrite.h>
#pragma comment(lib, "dwrite.lib")
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

#define GAME_TILE_SIZE 64.f
#define ATLAS_TILE_SIZE 128.0f

#define DEG_TO_RAD(degree) ((3.14 / 180.0) * degree)
#define RAD_TO_DEG(radian) ((180.0 / 3.14) * radian)

// Character settings

#define CHARACTER_MOVE_DEFAULT_SPEED 300.f
#define CHARACTER_MOVE_SLOW_SPEED 150.f
#define ANIMATION_FRAME_TIME 0.1f

#define IDLESTATE 0
#define MOVESTATE 1
#define ATTACKSTATE 2
#define INTERACTIONSTATE 3



typedef struct tagFPOINT
{
    float x, y;

    // ������
    tagFPOINT() : x(0.f), y(0.f) {}
    tagFPOINT(float _x, float _y) : x(_x), y(_y) {}

    // ������ �����ε�
    tagFPOINT operator+(const tagFPOINT& rhs) const { return { x + rhs.x, y + rhs.y }; }
    tagFPOINT operator-(const tagFPOINT& rhs) const { return { x - rhs.x, y - rhs.y }; }
    tagFPOINT operator*(float scalar) const { return { x * scalar, y * scalar }; }
    tagFPOINT operator/(float scalar) const { return { x / scalar, y / scalar }; }

    tagFPOINT& operator+=(const tagFPOINT& rhs) { x += rhs.x; y += rhs.y; return *this; }
    tagFPOINT& operator-=(const tagFPOINT& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
    tagFPOINT& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
    tagFPOINT& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }

    bool operator==(const tagFPOINT& rhs) const { return x == rhs.x && y == rhs.y; }
    bool operator!=(const tagFPOINT& rhs) const { return !(*this == rhs); }

    // ���� ����
    float Length() const { return std::sqrt(x * x + y * y); }

    // ����ȭ
    tagFPOINT Normalized() const
    {
        float len = Length();
        return (len > 0.0001f) ? tagFPOINT{ x / len, y / len } : tagFPOINT{ 0.f, 0.f };
    }

    // ����
    float Dot(const tagFPOINT& rhs) const { return x * rhs.x + y * rhs.y; }

    // ���� (2D���� ��Į�� ��)
    float Cross(const tagFPOINT& rhs) const { return x * rhs.y - y * rhs.x; }

    // �Ÿ�
    float Distance(const tagFPOINT& rhs) const
    {
        float dx = x - rhs.x;
        float dy = y - rhs.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // ����ȭ (in-place)
    void Normalize()
    {
        float len = Length();
        if (len > 0.0001f) { x /= len; y /= len; }
    }
}FPOINT;

struct FRECT
{
    float left, top, right, bottom;
};
/*
	extern Ű���� : ������ �Լ��� �ٸ� ���Ͽ� ���ǵǾ� �ִ� ���
	����� �˸��� Ű����.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

extern ComPtr<ID2D1SolidColorBrush> GBrush; 
extern ComPtr<IDWriteFactory> GdwriteFactory;
extern ComPtr<IDWriteTextFormat> GtextFormat;

enum class AnimationMode 
{
    Loop,      // ��� �ݺ�
    Hold,      // ������ �����ӿ��� ����
    FreezeAtX, // Ư�� �����ӿ��� ���� (�ɼ�)
};

struct FrameInfo 
{
    POINT startFrame;
    POINT endFrame;
    AnimationMode mode = AnimationMode::Loop; 
};

