// CommonFunction.h
/*
	inline: 컴파일러에게 함수 호출을 최적화 하도록 요청하는 키워드
	일반적인 함수들은 함수 호출 시에 매개변수를 스택메모리 저장, 
	함수 시작주소를 호출, 반환주소로 이동
	// main.cpp
	int result = 3 + 6;

	inline int Add(int a, int b)
	{
		return a + b;
	}
*/
#pragma once
#include "config.h"

#include <codecvt>
inline RECT GetRect(int left, int top, int width, int height)
{
	RECT rc{ left, top, left + width, top + height };
	return rc;
}

inline RECT GetRectAtCenter(int x, int y, int width, int height)
{
	RECT rc{ x - (width / 2), y - (height / 2),
		x + (width / 2), y + (height / 2) };
	return rc;
}

inline void RenderStar(HDC hdc, int posX, int posY)
{
	MoveToEx(hdc, posX + 60, posY + 20, NULL);
	LineTo(hdc, posX - 60, posY + 20);
	LineTo(hdc, posX + 40, posY - 60);
	LineTo(hdc, posX, posY + 60);
	LineTo(hdc, posX - 40, posY - 60);
	LineTo(hdc, posX + 60, posY + 20);
}

// 좌상단 기준
inline void RenderRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderRectAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Rectangle(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

// 좌상단 기준
inline void RenderEllipse(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

// 가운데 기준
inline void RenderEllipseAtCenter(HDC hdc, int centerX, int centerY, int width, int height)
{
	Ellipse(hdc, centerX - (width / 2), centerY - (height / 2),
		centerX + (width / 2), centerY + (height / 2));
}

inline bool PointInRect(POINT ptMouse, RECT rc)
{
	if (ptMouse.x < rc.left || ptMouse.x > rc.right
		|| ptMouse.y < rc.top || ptMouse.y > rc.bottom)
	{
		return false;
	}
	return true;
}

inline bool RectInRect(RECT rc1, RECT rc2)
{
	if (rc1.right < rc2.left || rc1.left > rc2.right
		|| rc1.top > rc2.bottom || rc1.bottom < rc2.top)
	{
		return false;
	}
	return true;
}

inline void UpdateRect(RECT& rc, FPOINT pt)
{
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	rc.left = pt.x - (width / 2);
	rc.right = rc.left + width;
	rc.top = pt.y - (height / 2);
	rc.bottom = rc.top + height;
}

inline float GetAngle(FPOINT start, FPOINT end)
{
	float x = end.x - start.x;
	float y = end.y - start.y;

	 return -atan2f(y, x);	// 라디안
}

inline float GetDistance(FPOINT p1, FPOINT p2)
{
	float x = p1.x - p2.x;
	float y = p1.y - p2.y;

	 return sqrtf(x * x + y * y);
}

inline FPOINT RotatePoint(const FPOINT& p, double angleRad) {
	FPOINT rotated;
	rotated.x = p.x * std::cos(angleRad) - p.y * std::sin(angleRad);
	rotated.y = p.x * std::sin(angleRad) + p.y * std::cos(angleRad);
	return rotated;
}

inline void SetClientRect(HWND hWnd, int width, int height)
{
	RECT rc;
	DWORD style = GetWindowLong(hWnd, GWL_STYLE);
	DWORD exStyle = GetWindowLong(hWnd, GWL_EXSTYLE);

	SetRect(&rc, 0, 0, width, height);
	AdjustWindowRectEx(&rc, style, GetMenu(hWnd) != NULL, exStyle);
	if (style & WS_VSCROLL)
		rc.right += GetSystemMetrics(SM_CXVSCROLL);
	if (style & WS_HSCROLL)
		rc.bottom += GetSystemMetrics(SM_CYHSCROLL);

	SetWindowPos(hWnd, NULL, 0, 0, rc.right - rc.left, 
		rc.bottom - rc.top, SWP_NOMOVE | SWP_NOZORDER);
}

inline void DrawCenteredRect(ID2D1RenderTarget* rt, FPOINT center, float halfSize, D2D1::ColorF color, float thickness = 1.0f)
{
	ComPtr<ID2D1SolidColorBrush> tempBrush;
	rt->CreateSolidColorBrush(color, &tempBrush);

	D2D1_RECT_F rect = D2D1::RectF(
		center.x - halfSize, center.y - halfSize,
		center.x + halfSize, center.y + halfSize
	);
	rt->DrawRectangle(rect, tempBrush.Get(), thickness);
}

inline void DrawCenteredRect(ID2D1RenderTarget* rt, FPOINT center, FPOINT scale, D2D1::ColorF color, float thickness = 1.0f)
{
	ComPtr<ID2D1SolidColorBrush> tempBrush;
	rt->CreateSolidColorBrush(color, &tempBrush);

	D2D1_RECT_F rect = D2D1::RectF(
		center.x - scale.x * 0.5f, center.y - scale.y * 0.5f,
		center.x + scale.x * 0.5f, center.y + scale.y * 0.5f
	);
	rt->DrawRectangle(rect, tempBrush.Get(), thickness);
}

inline void DrawLine(ID2D1RenderTarget* rt, FPOINT from, FPOINT to, D2D1::ColorF color, float thickness = 1.0f)
{
	ComPtr<ID2D1SolidColorBrush> tempBrush;
	rt->CreateSolidColorBrush(color, &tempBrush);

	rt->DrawLine(
		{ from.x, from.y },
		{ to.x,to.y },
		tempBrush.Get(),
		thickness  // 선 두께
	);
}

inline float RandomRange(float min, float max)
{
	float r = (float)rand() / RAND_MAX; // 0.0 ~ 1.0
	return min + (max - min) * r;
}

inline void ClampVector(FPOINT& vec, float maxLength)
{
	float len = sqrtf(vec.x * vec.x + vec.y * vec.y);
	if (len > maxLength)
	{
		float scale = maxLength / len;
		vec.x *= scale;
		vec.y *= scale;
	}
}

inline FPOINT RotateVector(const FPOINT& vec, float degrees)
{
	float rad = degrees * 3.1415926f / 180.0f;
	float cosA = cosf(rad);
	float sinA = sinf(rad);

	return {
		vec.x * cosA - vec.y * sinA,
		vec.x * sinA + vec.y * cosA
	};
}

inline float Dot(const FPOINT& a, const FPOINT& b)
{
	return a.x * b.x + a.y * b.y;
}

inline FPOINT Reflect(const FPOINT& velocity, const FPOINT& normal)
{
	float dot = Dot(velocity, normal);
	return {
		velocity.x - 2.0f * normal.x * dot,
		velocity.y - 2.0f * normal.y * dot
	};
}


template<typename T>
T Clamp(T value, T min, T max)
{
	if (value < min) return min;
	if (value > max) return max;
	return value;
}

inline void DrawD2DText(
	ID2D1RenderTarget* rt,
	const wchar_t* fmt,
	float x, float y,
	... // 가변 인자
)
{
	// 1. 문자열 포맷팅
	wchar_t buffer[256];

	va_list args;
	va_start(args, fmt);
	vswprintf(buffer, 256, fmt, args);
	va_end(args);

	// 2. 출력 영역 (자동 정렬)
	D2D1_RECT_F rect = D2D1::RectF(x, y, x + 1000, y + 1000); // 가로폭 넉넉히

	// 3. 텍스트 출력
	rt->DrawText(buffer, (UINT32)wcslen(buffer), GtextFormat.Get(), &rect, GBrush.Get());
}


static std::wstring Utf8ToWstring(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.from_bytes(str);
}
static std::string WStringToString(const std::wstring& wstr)
{
	if (wstr.empty()) return {};

	int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string result(size_needed - 1, 0); // 마지막 널문자 제외
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], size_needed, nullptr, nullptr);

	return result;
}
inline float EaseInOut(float t)
{
	return t < 0.5f ? 4.f * t * t * t : 1.f - powf(-2.f * t + 2.f, 3) / 2.f;
}