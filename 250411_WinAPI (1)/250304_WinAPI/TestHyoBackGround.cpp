#include "pch.h"
#include "TestHyoBackGround.h"
#include "Image.h"
#include "CameraManager.h"
#include "CommonFunction.h"
#include "KeyManager.h"

//HRESULT TestHyoBackGround::Init()
//{
//	SetClientRect(g_hWnd, TILEMAPTOOL_X, TILEMAPTOOL_Y);
//	image = ImageManager::GetInstance()->FindImage("testTile");
//	tileSize = 20;
//	startX = 0;
//	endX = 0;
//	startY = 0;
//	endY = 0;
//	return S_OK;
//}
//
//void TestHyoBackGround::Update(float TimeDelta)
//{
//
//}
//
//void TestHyoBackGround::Render(ID2D1HwndRenderTarget* renderTarget)
//{
//	Viewport viewRender = CameraManager::GetInstance()->GetInRect();
//
//	startX = viewRender.left / tileSize;
//	endX = viewRender.right / tileSize;
//	startY = viewRender.top / tileSize;
//	endY = viewRender.bottom / tileSize;
//	
//	for (int i = 0; i < endY; i++)
//	{
//		for (int j = 0; j < endX; j++)
//		{
//			if (i < 0 || j < 0 || i > TILEMAPTOOL_X || j > TILEMAPTOOL_Y) continue;
//
//			float worldX = j * tileSize;
//			float worldY = i * tileSize;
//
//			float screenX = worldX + CameraManager::GetInstance()->GetPos().x;
//			float screenY = worldY + CameraManager::GetInstance()->GetPos().y;
//
//			image->Render(renderTarget, screenX, screenY);
//		}
//	}
//	//image->Render(renderTarget, 0, 0);
//}
//
//void TestHyoBackGround::Release()
//{
//}

HRESULT TestHyoBackGround::Init(int idX, int idY)
{
	image = ImageManager::GetInstance()->FindImage("testTile");

	this->idX = idX;
	this->idY = idY;

	center.x = idX * TILE_SIZE + (TILE_SIZE / 2);
	center.y = idY * TILE_SIZE + (TILE_SIZE / 2);

	rc.left = idX * TILE_SIZE;
	rc.right = rc.left + TILE_SIZE;
	rc.top = idY * TILE_SIZE;
	rc.bottom = rc.top + TILE_SIZE;

	return S_OK;
}

void TestHyoBackGround::Release()
{
}

void TestHyoBackGround::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
	{
		/*rc.right += 200 * TimeDelta;
		rc.left += 200 * TimeDelta;*/
		center.x -= 200 * TimeDelta;
		//Pos.x += 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
	{
		/*rc.right -= 200 * TimeDelta;
		rc.left -= 200 * TimeDelta;*/
		center.x += 200 * TimeDelta;
		//Pos.x -= 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	{
		/*rc.top -= 200 * TimeDelta;
		rc.bottom -= 200 * TimeDelta;*/
		center.y += 200 * TimeDelta;
		//Pos.y -= 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	{
		/*rc.top += 200 * TimeDelta;
		rc.bottom += 200 * TimeDelta;*/
		center.y -= 200 * TimeDelta;
		//Pos.y += 200 * TimeDelta;
	}
}

void TestHyoBackGround::Render(ID2D1HwndRenderTarget* renderTarget)
{
	Viewport viewTile = CameraManager::GetInstance()->GetInRect();
	//if (rc.right > WINSIZE_X || rc.left < 0 || rc.top < 0 || rc.bottom > WINSIZE_Y) continue; 

	DrawCenteredRect(renderTarget, center, TILE_SIZE, D2D1::ColorF::White);
	
	/*image->Render(renderTarget, Pos.x + CameraManager::GetInstance()->GetPos().x,
		Pos.y + CameraManager::GetInstance()->GetPos().y);*/
	
	//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

TestHyoBackGround::~TestHyoBackGround()
{
}
