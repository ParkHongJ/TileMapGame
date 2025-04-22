#include "pch.h"
#include "Monster.h"
//#include "CollisionManager.h"

Monster::Monster()
{
}

Monster::~Monster()
{
}

HRESULT Monster::Init()
{
	monsterState = MonsterState::IDLE;
	damage = 0;
	monsterHP = 0;
	dir = { -1,-1 };
	moveSpeed = 0.0f;

	startFrameIndexX = 0;
	startFrameIndexY = 0;

	endFrameIndexX = 0;
	endFrameIndexY = 0;

	curFrameIndexX = 0;
	curFrameIndexY = 0;

	elipsedTime = 0.0f;
	frameSpeed = 0.0f;
	meetPlayer = false;
	isAttack = false;
	isDamaged = false;
	isFlip = false;

	isTileTouchingLeft = false;
	isTileTouchingRight = false;
	isTileTouchingTop = false;
	isTileTouchingLeftBottom = false;
	isTileTouchingRightBottom = false;

	hasBottomTile = true;

	isPlayerTouchingLeft = false;
	isPlayerTouchingRight = false;
	isPlayerTouchingTop = false;
	isPlayerTouchingBottom = false;
	isPlayerTouchingCenterTop = false;

	isItemTouchingLeft = false;
	isItemTouchingRight = false;
	isItemTouchingTop = false;
	isItemTouchingBottom = false;

	meetPlayerLeft = false;
	meetPlayerRight = false;

	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
	return S_OK;
}

void Monster::Release()
{
}

void Monster::Update(float TimeDelta)
{
	Move();
	FrameUpdate(TimeDelta);
}

void Monster::FrameUpdate(float TimeDelta)
{
}

void Monster::CheckTileCollision()
{
}

void Monster::CheckPlayerCollision()
{
}

void Monster::CheckItemCollision()
{
}

void Monster::Move()
{
}

void Monster::Render(ID2D1RenderTarget* renderTarget)
{
	
}

void Monster::Detect(GameObject* obj)
{

}
