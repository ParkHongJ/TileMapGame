#include "pch.h"
#include "BatMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"

BatMonster::BatMonster()
{
}

BatMonster::~BatMonster()
{
}

HRESULT BatMonster::Init()
{
    snakeImage = ImageManager::GetInstance()->FindImage("Snake_Monster");
    return S_OK;
}

void BatMonster::Release()
{
}

void BatMonster::Update(float TimeDelta)
{
}

void BatMonster::FrameUpdate(float TimeDelta)
{
}

void BatMonster::CheckTileCollision()
{
}

void BatMonster::CheckPlayerCollision()
{
}

void BatMonster::CheckItemCollision()
{
}

void BatMonster::Move()
{
}

void BatMonster::ReverseMove()
{
}

void BatMonster::Render(ID2D1HwndRenderTarget* renderTarget)
{
}
