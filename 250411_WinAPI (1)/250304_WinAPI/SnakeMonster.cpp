#include "pch.h"
#include "SnakeMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"

SnakeMonster::SnakeMonster()
{
}

SnakeMonster::~SnakeMonster()
{
}

HRESULT SnakeMonster::Init()
{
    snakeImage = ImageManager::GetInstance()->FindImage("Snake_Monster");

    //snakeCollider = new BoxCollider(
    //    { 20.0f , 15.0f },     // Offset
    //    { 85.0f, 80.0f },  // 
    //    this
    //    );

    //snakeCollider = new BoxCollider(
    //    { 20.0f , 15.0f },     // Offset
    //    { 85.0f, 85.0f },  // 
    //    CollisionMaskType::MONSTER,this
    //);

    colliderSize = { 100.0f, 50.0f };
    colliderOffsetY = 30.f;

    snakeCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this
    );

   // Pos = { (WINSIZE_X / 2), (WINSIZE_Y / 2)};
    SetPos({ 300,270 });
    monsterHP = 1;
    damage = 1; 
    moveSpeed = 80.0f;
    monsterState = MonsterState::IDLE;

    currFrame = { 0,0 };
    moveFrameInfo = { {0,0},{5,0} }; 
    attackFrameInfo = { { 0,1 }, { 5,1 } };
    dir = { 1,1 };

    // attackMoveInfo;
    currFrameInfo = { { 0,0 }, {0, 0} };

	isTouchingLeft = false;
	isTouchingRight = false;
	isTouchingTop = false;
	isTouchingBottom = false;

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

    return S_OK;
}

void SnakeMonster::Release()
{
}

void SnakeMonster::Update(float TimeDelta)
{
    CheckTileCollision();
    CheckPlayerCollision();
    CheckItemCollision();
   
    // 벽 안 만났을 때 
    if (!isTileTouchingRight && !isTileTouchingLeft && !isPlayerTouchingRight && !isPlayerTouchingLeft && isTileTouchingRightBottom && isTileTouchingLeftBottom)
    {
        monsterState = MonsterState::MOVE;
        meetPlayerLeft = false;
        meetPlayerRight = false;
        hasBottomTile = false;
        Move();
    }
    //벽 만났을 때 Update
    else if (isTileTouchingRight || isTileTouchingLeft )
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        Move();
    }

    // 오른쪽으로 가는데 밑에 타일이 없을 때 
    if (!isTileTouchingRightBottom && !hasBottomTile && dir.x > 0)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        hasBottomTile = true;
        Move();
    }

    // 왼쪽으로 가는데 밑에 타일이 없을 때 
    else if (!isTileTouchingLeftBottom && !hasBottomTile && dir.x < 0)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        hasBottomTile = true;
        Move();
    }

    // Player 만났을 때 Update , 데미지도 포함 
    if (dir.x < 0 && isPlayerTouchingLeft )
    {
        monsterState = MonsterState::ATTACK;
        meetPlayerLeft = true;
    }

    if (dir.x > 0 && isPlayerTouchingRight)
    {
        monsterState = MonsterState::ATTACK;
        meetPlayerRight = true;
    }

	//Player가 위에서 밟았을 때, 아이템과 충돌했을 때  데미지 닳기 
    if ((!isPlayerTouchingLeft && !isPlayerTouchingRight && !isPlayerTouchingBottom && isPlayerTouchingCenterTop) || isItemTouchingLeft || isItemTouchingRight || isItemTouchingBottom || isItemTouchingTop)
    {
        monsterHP--;
    }

    if (monsterHP == 0)
        monsterState = MonsterState::DEAD;
   
    FrameUpdate(TimeDelta);
}

void SnakeMonster::FrameUpdate(float TimeDelta)
{
    // 기본 Move
    elipsedTime += TimeDelta;
    if (monsterState == MonsterState::MOVE)
    {
        currFrameInfo = moveFrameInfo;

        if (elipsedTime > 0.1f)
        {
            currFrame.x++;
            if (currFrame.x > moveFrameInfo.endFrame.x)
            {
                currFrame.x = moveFrameInfo.startFrame.x;
            }

            currFrame.y = moveFrameInfo.startFrame.y;
            elipsedTime = 0;
        }
    }

    // 공격할 때 
    if (monsterState == MonsterState::ATTACK)
    {
        currFrameInfo = attackFrameInfo;

        if (elipsedTime > 0.1f)
        {
			currFrame.x++;
            if (currFrame.x > attackFrameInfo.endFrame.x)
            {
                currFrame.x = attackFrameInfo.startFrame.x;
            }
            currFrame.y = attackFrameInfo.startFrame.y;
            elipsedTime = 0;
        }

            
    }
}

void SnakeMonster::CheckTileCollision()
{
    float maxDist = 10.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isTileTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE, true, debugTime)/* ||
       CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime)*/;

    isTileTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, true, debugTime) /*||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime)*/;

    isTileTouchingLeftBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, true, debugTime);
    isTileTouchingRightBottom = CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, true, debugTime);
}

void SnakeMonster::CheckPlayerCollision()
{
    float maxDist = 20.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT centerTop = { Pos.x , Pos.y - colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isPlayerTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::PLAYER, true, debugTime) ||
       CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingCenterTop = CollisionManager::GetInstance()->RaycastType({ centerTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime);

}

void SnakeMonster::CheckItemCollision()
{
    float maxDist = 10.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isPlayerTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::PLAYER, true, debugTime);
}

void SnakeMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if(monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
}

void SnakeMonster::ReverseMove()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
}

void SnakeMonster::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (snakeImage)
    {
        if (monsterHP == 1 && monsterState == MonsterState::MOVE)
        {
            if (dir.x > 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (monsterHP == 1 && monsterState == MonsterState::ATTACK)
        {
            if (dir.x > 0 && meetPlayerRight)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0 && meetPlayerLeft)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}

void SnakeMonster::Detect(GameObject* obj)
{
    int i = 5;
    //if (auto player = obj->GetType<Character>())
    //{
    //    
    //    SetDestroy();
    //}

    //else if (auto player = obj->GetType<Character>())
    //{

    //}
}
