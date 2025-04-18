#include "pch.h"
#include "HyoCharacter.h"
#include "Image.h"
#include "ImageManager.h"
#include "CommonFunction.h"
#include "KeyManager.h"
#include "CameraManager.h"

HRESULT HyoCharacter::Init()
{
	// testBackGround = ImageManager::GetInstance()->FindImage("testBackGround");
	testCamera = ImageManager::GetInstance()->FindImage("testCamera");

	SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });

	/*mapSizeHeight = testBackGround->GetHeight();
	mapSizeWidth = testBackGround->GetWidth();*/

	dir = -1;
	backGroundPos = { WINSIZE_X / 2,WINSIZE_Y / 2 };
	
	lookUpTime = 0.0f;
	lookDownTime = 0.0f;

	isLookUp = false;
	isLookDown = false;

	return S_OK;	
}

void HyoCharacter::Release()
{
	
}

void HyoCharacter::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
	{
		Pos.x += 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
	{
		Pos.x -= 200 * TimeDelta;
	}
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
	{
		Pos.y -= 200 * TimeDelta;
		lookUpTime += TimeDelta;

		if (lookUpTime >= 3.0f)
		{
			isLookUp = true;
		}
	}
	else
	{
		lookUpTime = 0.0f;
		isLookUp = false;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
	{
		Pos.y += 200 * TimeDelta;
		lookDownTime += TimeDelta;

		if (lookDownTime >= 3.0f)
		{
			isLookDown = true;
		}
	}
	else
	{
		lookDownTime = 0.0f;
		isLookDown = false;
	}
	
	CameraManager::GetInstance()->SetLookingState(isLookUp, isLookDown);
}

void HyoCharacter::Render(ID2D1HwndRenderTarget* renderTarget)
{
	/*testBackGround->Render(renderTarget, backGroundPos.x + CameraManager::GetInstance()->GetPos().x,
		backGroundPos.y + CameraManager::GetInstance()->GetPos().y);*/

	testCamera->Render(renderTarget, Pos.x + CameraManager::GetInstance()->GetPos().x,
		Pos.y + CameraManager::GetInstance()->GetPos().y);
}

//void HyoCharacter::TestRender(ID2D1HwndRenderTarget* renderTarget, const FPOINT& cameraPos)
//{
//	if (testBackGround)
//	{
//		testBackGround->Render(renderTarget, backGroundPos.x + CameraManager::GetInstance()->GetPos().x,
//			backGroundPos.y + CameraManager::GetInstance()->GetPos().y);
//	}
//	if (testCamera)
//	{
//		testCamera->Render(renderTarget, Pos.x + cameraPos.x, Pos.y + cameraPos.y);
//	}
//}
