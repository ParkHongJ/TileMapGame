#include "Missile.h"
#include "CommonFunction.h"
#include "Image.h"

HRESULT Missile::Init()
{
	pos = { 0, 0 };
	isActived = false;
	color = RGB(255, 255, 255);
	moveSpeed = 50.0f;
	angle = 90.0f;
	size = 20;
	//destination;
	image = ImageManager::GetInstance()->AddImage(
		"미사일", TEXT("Image/bullet.bmp"), 21, 21,
		true, RGB(255, 0, 255));
	if (image == nullptr)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Missile::Release()
{
}

void Missile::Update()
{
	Move();
	if (isActived && IsOutofScreen())
	{
		isActived = false;
	}
}

void Missile::Render(HDC hdc)
{
	if (image)
	{
		image->Render(hdc, pos.x, pos.y);
	}
	if (isActived)
	{
		RenderEllipseAtCenter(hdc, pos.x, pos.y, size, size);
	}
}

void Missile::Move()
{
	if (isActived)
	{
		//float amplitude = 5.5f;    // 사인 곡선의 진폭
		//float frequency = 1.2f;    // 사인 곡선의 주파수
		//static float time = 0.0f;
		//time += 0.02f;
		//if (time > 6.28f) time -= 6.28f;
		////// 사인 곡선을 따른 y축 변위 계산 및 적용
		//float verticalOffset = amplitude * std::sin(frequency * time);
		////float horizontalOffset = amplitude * std::cos(frequency * time);

		////pos.x += moveSpeed * cosf(DEG_TO_RAD(angle)) - horizontalOffset;
		////pos.y -= moveSpeed * sinf(DEG_TO_RAD(angle)) + verticalOffset;

		//FPOINT addPos;
		//addPos.x = moveSpeed * cosf(DEG_TO_RAD(-90)) + verticalOffset;
		//addPos.y = moveSpeed * sinf(DEG_TO_RAD(-90));
		//addPos = RotatePoint(addPos, DEG_TO_RAD(90));

		//pos.x += addPos.x;
		//pos.y -= addPos.y;

		pos.x += moveSpeed * TimerManager::GetInstance()->GetDeltaTime()
			* cosf(DEG_TO_RAD(-90));
		pos.y -= moveSpeed * TimerManager::GetInstance()->GetDeltaTime()
			* sinf(DEG_TO_RAD(-90));
	}
}

bool Missile::IsOutofScreen()
{
	float right = pos.x + size / 2;
	float left = pos.x - size / 2;
	float top = pos.y - size / 2;
	float bottom = pos.y + size / 2;

	if (right < 0 || left > WINSIZE_X 
		|| bottom < 0 || top > WINSIZE_Y)
		return true;

	return false;
}

Missile::Missile()
{
}

Missile::~Missile()
{
}
