#include "CameraManager.h"

HRESULT CameraManager::Init()
{
	pos = { 0,0 };
	target = { 0,0 };
	mapSize = { 0,0 };
	offset = { WINSIZE_X / 2,WINSIZE_Y / 2 };

	mapHeight = 0.0f;
	mapWidth = 0.0f;

	ViewPort.left = 0;
	ViewPort.top = 0;
	ViewPort.right = 0;
	ViewPort.bottom = 0;

	yOffset = 0.0f;
	currYOffset = 0.0f;
	yOffsetSpeed = 15.0f;

	isLookUp = false;
	isLookDown = false;
	return S_OK;
}

void CameraManager::Release()
{
}

void CameraManager::Update( float TimeDelta)
{
	//target = playerPos;
	currYOffset += (yOffset - currYOffset) * yOffsetSpeed * TimeDelta;




	if (isLookUp && !isLookDown)
	{
		yOffset = 600.0f;
		pos.y = -(target.y) + offset.y + currYOffset;
	}
	else if (!isLookUp && !isLookDown)
	{
		yOffset = 0.0f;
		pos.y = -(target.y) + offset.y + currYOffset;
	}
	if (isLookDown && !isLookUp)
	{
		yOffset = -600.0f;
		pos.y = -(target.y) + offset.y + currYOffset;
	}
	else if (!isLookUp && !isLookDown)
	{
		yOffset = 0.0f;
		pos.y = -(target.y) + offset.y + currYOffset;
	}

	//else
	//{
	//	yOffset = 0.0f;
	//}

	pos.x = -(target.x) + offset.x;
	pos.y = -(target.y) + offset.y + currYOffset;

	float x = -(TILEMAPTOOL_X - WINSIZE_X);
	float y = -(TILEMAPTOOL_Y - WINSIZE_Y);

	if (pos.x < x) pos.x = x;
	if (pos.x > -x) pos.x = -x;
	if (pos.y < y) pos.y = y;
	if (pos.y > -y) pos.y = -y;

	/*ViewPort.left = -pos.x;
	ViewPort.top = -pos.y;
	ViewPort.right = ViewPort.left + WINSIZE_X;
	ViewPort.bottom = ViewPort.top + WINSIZE_Y;*/

	// 확인용 뷰포트 크기 
	ViewPort.left = -(pos.x - 50);
	ViewPort.top = -(pos.y - 50);
	ViewPort.right = ViewPort.left + WINSIZE_X - 100;
	ViewPort.bottom = ViewPort.top + WINSIZE_Y - 100;
}

//Viewport CameraManager::GetInRect()
//{
//	Viewport rect;
//	rect.left = -pos.x;
//	rect.top = -pos.y;
//	rect.right = rect.left + WINSIZE_X;
//	rect.bottom = rect.top + WINSIZE_Y;
//
//	//rect.left = 100;
//	//rect.top = 100;
//	//rect.right = 100;
//	//rect.bottom = 100;ko
//
//	return  ();
//}
