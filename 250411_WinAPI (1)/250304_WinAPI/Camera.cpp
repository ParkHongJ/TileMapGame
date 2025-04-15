#include "Camera.h"
#include "Character.h"
#include "TimerManager.h"

HRESULT Camera::Init()
{
	pos = { 0,0 };			
	target = { 0,0 };		
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

	/*pos.x -= (target.x - pos.x) * moveSpeed * TimeDelta;
	pos.y -= (target.y - pos.y) * moveSpeed * TimeDelta;*/

	pos.x += target.x * (-1);
	pos.y += target.y * (-1);

	// Ű �Է��� ī�޶󿡼� �޾ƾ��ϴ��� ,, ĳ���Ϳ��� �޾ƾ��ϴ��� ,, 
}

void Camera::Render(ID2D1HwndRenderTarget* renderTarget)
{
}
