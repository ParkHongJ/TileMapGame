#include "pch.h"
#include "Gate.h"
#include "Image.h"
#include "CameraManager.h"
#include "ImageManager.h"
#include "CommonFunction.h"
#include "GameManager.h"
#include "Collider.h"
HRESULT Gate::Init()
{
	gateImage = ImageManager::GetInstance()->FindImage("GateOpen");
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;

	Pos.x = 216.f;
	Pos.y = 216.f;
	
	objectName = OBJECTNAME::GATE;
	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::WORLDOBJECT, this);
	return S_OK;
}

void Gate::Release()
{
}

void Gate::Update(float TimeDelta)
{
	if (bNextStage)
	{
		nextStageDelay -= TimeDelta;
		if (nextStageDelay <= 0.f)
		{
			GameManager::GetInstance()->TravelToNextScene();
		}
	}
}

void Gate::LateUpdate(float TimeDelta)
{
}

void Gate::Render(ID2D1RenderTarget* renderTarget)
{
	//총 6번을 렌더해야한다.
	// 총 6 방향: 좌, 좌상, 상, 우상, 우, 원점
	constexpr int offsetCount = 9;
	FPOINT offsets[offsetCount] =
	{
	{-1, -1}, // 좌상
	{  0, -1}, // 상
	{  1, -1}, // 우상
	{-1,  0}, // 좌
	{  0,  0}, // 원점
	{  1,  0}, // 우
	{-1,  1}, // 좌하
	{  0,  1}, // 하
	{  1,  1}  // 우하
	};

	for (int i = 0; i < offsetCount; ++i)
	{
		FPOINT imageOffset = {
			offsets[i].x * GAME_TILE_SIZE,
			offsets[i].y * GAME_TILE_SIZE
		};

		
	}
	FPOINT drawPos = Pos + CameraManager::GetInstance()->GetPos();

	gateImage->Render(renderTarget, drawPos.x, drawPos.y - GAME_TILE_SIZE * 0.25f, objectScale, objectScale, 0, 0, 396, 396);

	DrawCenteredRect(renderTarget, drawPos, { 20.f,20.f }, D2D1::ColorF::Red);
}

void Gate::EnterGate()
{
	//다음 씬으로 가즈앗

	if (bNextStage == false)
	{
		bNextStage = true;
	}
}
