#include "Camera.h"
#include "Character.h"
#include "TimerManager.h"

HRESULT Camera::Init()
{
	pos = { 0,0 };			
	target = { 0,0 };	
	mapSize = { 0,0 };
	offset = { WINSIZE_X / 2,WINSIZE_Y / 2 };

	mapHeight = 0.0f;
	mapWidth = 0.0f;

	moveSpeed = 0.0f;
    return S_OK;
}

void Camera::Release()
{
}

void Camera::Update(const FPOINT& playerPos, float TimeDelta)
{
	target = playerPos;
	moveSpeed = 5.0f;

	pos.x = -(target.x) + offset.x;
	pos.y = -(target.y) + offset.y; 

	float x = -(mapWidth - WINSIZE_X);
	float y = -(mapHeight - WINSIZE_Y);

	if (pos.x < x) pos.x = x;
	if (pos.x > -x) pos.x = -x;
	if (pos.y < y) pos.y = y;
	if (pos.y > -y) pos.y = -y;
	//if (pos.x > mapWidth) pos.x = mapWidth;

	/*if (mapWidth < pos.x) pos.x = mapWidth;
	if (mapHeight < pos.y) pos.y = mapHeight;*/

	/*if (pos.x < x) pos.x = x;
	if (x > 0) pos.x = 0;
	if (pos.y > y) pos.y = y; 
	if (y > 0) pos.y = y;*/

	/*pos.x -= (target.x - pos.x) * moveSpeed * TimeDelta;
	pos.y -= (target.y - pos.y) * moveSpeed * TimeDelta;*/

	/*pos.x += target.x * (-1);
	pos.y += target.y * (-1);*/

	// Ű �Է��� ī�޶󿡼� �޾ƾ��ϴ��� ,, ĳ���Ϳ��� �޾ƾ��ϴ��� ,, 
}

void Camera::Render(ID2D1HwndRenderTarget* renderTarget)
{
}
