#include "pch.h"
#include "SpiderMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"

SpiderMonster::SpiderMonster()
{
}

SpiderMonster::~SpiderMonster()
{
}

HRESULT SpiderMonster::Init()
{
    spiderImage = ImageManager::GetInstance()->FindImage("Spider_Monster");

    colliderSize = { 100.0f, 50.0f };
    colliderOffsetY = 30.f;

    spiderCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this
    );

    SetPos({ 300,200 });
    monsterHP = 1;
    damage = 1;
    moveSpeed = 80.0f;
    monsterState = MonsterState::IDLE;

    currFrame = { 0,0 };
    IdleFrameInfo = { {8,3},{8,3} };
    attackMoveInfo = { {8,3 }, { 12,3 }};
    moveFrameInfo = { {0,3},{7,5} };
    //attackFrameInfo = { { 0,3 }, { 5,3 } };
    dir = { 1,1 };
    currFrameInfo = { {8,3},{8,3} };

    return S_OK;
}

void SpiderMonster::Release()
{
}

void SpiderMonster::Update(float TimeDelta)
{
    CheckTileCollision();
    CheckPlayerCollision();
    CheckItemCollision();
    FrameUpdate(TimeDelta);
    // 플레이어가 밑에 없을 때 (Idle) 
}

void SpiderMonster::FrameUpdate(float TimeDelta)
{
    elipsedTime += TimeDelta;

    // 기본 자세 -> 위에 거꾸로 붙어있음 
    if (monsterState == MonsterState::IDLE)
    {
        currFrame.y  = IdleFrameInfo.endFrame.y;
    }

    // 플레이어가 밑에 지나갔을 때 
    if (monsterState == MonsterState::ATTACKMOVE)
    {
        currFrameInfo = attackMoveInfo;
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

    // 공격할 때 -> 플레이어를 향해 점프 
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

void SpiderMonster::CheckTileCollision()
{
}

void SpiderMonster::CheckPlayerCollision()
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

void SpiderMonster::CheckItemCollision()
{
}

void SpiderMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
  /*  if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;*/
}

void SpiderMonster::ReverseMove()
{
}

void SpiderMonster::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (spiderImage)
    {
        if (monsterHP == 1 && monsterState == MonsterState::IDLE)
        {
            spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
        }

        if (monsterHP == 1 && monsterState == MonsterState::ATTACKMOVE)
        {
            spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
        }

        if (monsterHP == 1 && monsterState == MonsterState::MOVE)
        {
            if (dir.x > 0)
            {
                spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (monsterHP == 1 && monsterState == MonsterState::ATTACK)
        {
            if (dir.x > 0 && meetPlayerRight)
            {
                spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0 && meetPlayerLeft)
            {
                spiderImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}
