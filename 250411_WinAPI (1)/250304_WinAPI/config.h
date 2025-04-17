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
#include <d2d1.h>             // 기본 D2D (선택적)
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
	컴파일러에서 해당 코드를 뒤에 정의된 코드로 변경한다. 
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

    // 생성자
    tagFPOINT() : x(0.f), y(0.f) {}
    tagFPOINT(float _x, float _y) : x(_x), y(_y) {}

    // 연산자 오버로딩
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

    // 벡터 길이
    float Length() const { return std::sqrt(x * x + y * y); }

    // 정규화
    tagFPOINT Normalized() const
    {
        float len = Length();
        return (len > 0.0001f) ? tagFPOINT{ x / len, y / len } : tagFPOINT{ 0.f, 0.f };
    }

    // 내적
    float Dot(const tagFPOINT& rhs) const { return x * rhs.x + y * rhs.y; }

    // 외적 (2D에서 스칼라 값)
    float Cross(const tagFPOINT& rhs) const { return x * rhs.y - y * rhs.x; }

    // 거리
    float Distance(const tagFPOINT& rhs) const
    {
        float dx = x - rhs.x;
        float dy = y - rhs.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    // 정규화 (in-place)
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
	extern 키워드 : 변수나 함수가 다른 파일에 정의되어 있다 라는
	사실을 알리는 키워드.
*/
extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern POINT g_ptMouse;

extern ComPtr<ID2D1SolidColorBrush> GBrush; 
extern ComPtr<IDWriteFactory> GdwriteFactory;
extern ComPtr<IDWriteTextFormat> GtextFormat;

enum class AnimationMode 
{
    Loop,      // 계속 반복
    Hold,      // 마지막 프레임에서 멈춤
    FreezeAtX, // 특정 프레임에서 멈춤 (옵션)
};

struct FrameInfo 
{
    POINT startFrame;
    POINT endFrame;
    AnimationMode mode = AnimationMode::Loop; 
};

