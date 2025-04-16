#include "CameraManager.h"

HRESULT CameraManager::Init()
{
	pos = { 0,0 };
	target = { 0,0 };
	mapSize = { 0,0 };
	offset = { WINSIZE_X / 2,WINSIZE_Y / 2 };

	mapHeight = 0.0f;
	mapWidth = 0.0f;

	return S_OK;
}

void CameraManager::Release()
{
}

void CameraManager::Update(const FPOINT& playerPos, float TimeDelta)
{
	target = playerPos;

	pos.x = -(target.x) + offset.x;
	pos.y = -(target.y) + offset.y;

	float x = -(mapWidth - WINSIZE_X);
	float y = -(mapHeight - WINSIZE_Y);

	if (pos.x < x) pos.x = x;
	if (pos.x > -x) pos.x = -x;
	if (pos.y < y) pos.y = y;
	if (pos.y > -y) pos.y = -y;
}
