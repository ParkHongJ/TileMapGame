#include "pch.h"
#include "RopeController.h"
#include "Rope.h"
#include "image.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "ImageManager.h"


HRESULT RopeController::Init()
{
	// ¥Û¥Û¿Ã 12, 9, ~ 15, 9 / 0, 12 ~ 9, 12
	//for (int i = 0; i < 4; ++i)
	//{
	//	Rope* temp = new Rope();
	//	temp->SetFrame(i + 12, 9);
	//	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
	//	FPOINT dest = { 300, float(i * 30) };
	//	temp->SetPos(dest);
	//}

	//for (int i = 0; i < 10; ++i)
	//{
	//	Rope* temp = new Rope();
	//	temp->SetFrame(i, 12);
	//	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
	//	FPOINT dest = { 300, float((i * 30) + 120) };
	//	temp->SetPos(dest);
	//}
	objectScale = { GAME_TILE_SIZE / (ATLAS_TILE_SIZE * 1.25f), GAME_TILE_SIZE / (ATLAS_TILE_SIZE * 1.25f) };
	image = ImageManager::GetInstance()->FindImage("Tae_Player");
	objectRenderId = RENDER_NPC;
	curFrameX = 12;
	curFrameY = 9;
	return S_OK;
}

void RopeController::Update(float TimeDelta)
{

	//if(KeyManager)
	Shake(TimeDelta);

	if (true == isMove)
	{
		UpMove(TimeDelta);
	}

	else if(true == isLanch)
	{
		DownMove(TimeDelta);
	}
}

void RopeController::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
	image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameX, curFrameY, objectScale.x, objectScale.y); // ¿”¿«∞™
}

void RopeController::Release()
{

}

void RopeController::Detect(GameObject* obj)
{

}

void RopeController::Shoot(FPOINT pos)
{
	isMove = true;
	isLanch = false;
	Pos = pos;
}

void RopeController::UpMove(float TimeDelta)
{
	RaycastHit Up;
	if (!CollisionManager::GetInstance()->RaycastType({ Pos, {0.f, -1.f} }, 10.f, Up, CollisionMaskType::TILE, true, 1.0f) && (SumDis < MaxDis))
	{
		Pos.y -= 800.f * TimeDelta;
		SumDis += 800.f * TimeDelta;

		return;
	}

	else
	{
		isLanch = true;
		isMove = false;
		curFrameX = 13;
		curFrameY = 9;
		downTime = 0.f;
	}
}

void RopeController::DownMove(float TimeDelta)
{
	downTime -= TimeDelta;

	if (0.f >= downTime)
	{
		CreateRope();
	}
}

void RopeController::Shake(float TimeDelta)
{

}

void RopeController::CreateRope()
{
	RaycastHit Down;
	FPOINT newPos = Pos;
	newPos.y += createCnt * 64;

	FPOINT leftPos = newPos;
	leftPos.x -= GAME_TILE_SIZE / 2;

	FPOINT RightPos = newPos;
	RightPos.x += GAME_TILE_SIZE / 2;

	if (CollisionManager::GetInstance()->RaycastType({ leftPos, {0.f, 1.f} }, 30.f, Down, CollisionMaskType::TILE, true, 1.0f) || 
	CollisionManager::GetInstance()->RaycastType({ RightPos, {0.f, 1.f} }, 30.f, Down, CollisionMaskType::TILE, true, 1.0f))
	{
		isMove = false;
		isLanch = false;
		return;
	}

	Rope* temp = new Rope();

	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);

	if (0 >= createCnt)
	{
		temp->SetFrame(1, 12);
		temp->SetPos(Pos);
		temp->SetObjectRenderId(RENDER_NPC);
	}

	else
	{
		temp->SetFrame(0, 12);
		temp->SetPos(Ropes[0]->GetPos());
	}

	Ropes.push_back(temp);
	++createCnt;

	Ropes[0]->SetPos(newPos);

	downTime = downMaxTime;
	//	Rope* temp = new Rope();
//	temp->SetFrame(i, 12);
//	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
//	FPOINT dest = { 300, float((i * 30) + 120) };

}
