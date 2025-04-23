#include "pch.h"
#include "SkeletonMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"
#include "Character.h"

SkeletonMonster::SkeletonMonster()
{
    int i = 5;
}

SkeletonMonster::~SkeletonMonster()
{
}

HRESULT SkeletonMonster::Init()
{
    skeletonImage = ImageManager::GetInstance()->FindImage("Skeleton_Monster");
    player = new Character();

    colliderSize = { 50.0f, 50.0f };
    colliderOffsetY = 0.f;

    skeletonCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this
    );

    SetPos({ 600,220 });
    monsterHP = 1;
    damage = 1;
    moveSpeed = 80.0f;
    monsterState = MonsterState::IDLE;
    deadElipsedTime = 0.0f;

    currFrame = { 0,0 };
   /* IdleFrameInfo = { {8,2},{8,2} };
    attackMoveInfo = { {8,2 }, { 12,2 }};*/
    moveFrameInfo = { {5,2},{13,2} };
    attackFrameInfo = { {5,2},{13,2} };
    deadFrameInfo = { {4,2},{0,2} };
    dir = { 1,1 };
    currFrameInfo = { {0,0},{0,0} };

    return S_OK;
}

void SkeletonMonster::Release()
{
    __super::Release();
}

void SkeletonMonster::Update(float TimeDelta)
{
    CheckTileCollision();
    CheckPlayerCollision();
    CheckItemCollision();

    // 벽 안 만났을 때 
    if (!isTileTouchingRight && !isTileTouchingLeft && !isPlayerTouchingRight && !isPlayerTouchingLeft && isTileTouchingRightBottom && isTileTouchingLeftBottom && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::MOVE;
        meetPlayerLeft = false;
        meetPlayerRight = false;
        hasBottomTile = false;
        Move();
    }
    //벽 만났을 때 Update
    else if (isTileTouchingRight || isTileTouchingLeft && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        Move();
    }

    // 오른쪽으로 가는데 밑에 타일이 없을 때 
    if (!isTileTouchingRightBottom && !hasBottomTile && dir.x > 0 && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        hasBottomTile = true;
        Move();

    }
    // 왼쪽으로 가는데 밑에 타일이 없을 때 
    else if (!isTileTouchingLeftBottom && !hasBottomTile && dir.x < 0 && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        hasBottomTile = true;
        Move();
    }

    // Player 만났을 때 Update , 데미지도 포함 
    if (dir.x < 0 && isPlayerTouchingLeft && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::ATTACK;
        meetPlayerLeft = true;
    }

    if (dir.x > 0 && isPlayerTouchingRight)
    {
       monsterState = MonsterState::ATTACK;
        meetPlayerRight = true;
    }

    if (monsterState == MonsterState::DEAD)
    {
        DeadEvent(TimeDelta);
    }

    //Player가 위에서 밟았을 때, 아이템과 충돌했을 때  데미지 닳기 
   /* if (!isPlayerTouchingLeft && !isPlayerTouchingRight && !isPlayerTouchingBottom && isPlayerTouchingCenterTop)
    {
        monsterHP--;
    }*/

    /*if (monsterState == MonsterState::DEAD)
    { 
		SetDestroy();
    }*/
       

    FrameUpdate(TimeDelta);
 
}

void SkeletonMonster::FrameUpdate(float TimeDelta)
{
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

    if (monsterState == MonsterState::DEAD)
    {
        currFrameInfo = deadFrameInfo;

        if (elipsedTime > 1.0f)
        {
            currFrame.x--;
            
            if (currFrame.x < deadFrameInfo.endFrame.x)
            {
                currFrame.x = deadFrameInfo.endFrame.x;
            }
            currFrame.y = deadFrameInfo.endFrame.y;
            elipsedTime = 0;
        }
    }
}

void SkeletonMonster::CheckTileCollision()
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

void SkeletonMonster::CheckPlayerCollision()
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

void SkeletonMonster::CheckItemCollision()
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

void SkeletonMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
}

void SkeletonMonster::ReverseMove()
{
}

void SkeletonMonster::Detect(GameObject* obj)
{
    FPOINT playerPos = player->GetPos();
    

    if (auto player = obj->GetType<Character>())
    {

        playerPos = player->GetPos();
        float playerPosBottom = playerPos.y + 20;
        float monsterPosTop = Pos.y;

        if (/*playerPosBottom < monsterPosTop &&*/ monsterState != MonsterState::DEAD)
        {
            monsterState = MonsterState::DEAD;
            deadElipsedTime = 0.0f;
        }

    }

    else if (auto player = obj->GetType<Character>())
    {

    }
}

void SkeletonMonster::DeadEvent(float TimeDelta)
{
    deadElipsedTime += TimeDelta;
    if (monsterState == MonsterState::DEAD)
    {
        if (deadElipsedTime > 1.5f)
        {
            SetDestroy();
        }
            
    }
}

void SkeletonMonster::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (skeletonImage)
    {
        if (/*monsterHP == 1 && */monsterState == MonsterState::MOVE)
        {
            if (dir.x > 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (/*monsterHP == 1 &&*/ monsterState == MonsterState::ATTACK)
        {
            if (dir.x > 0 && meetPlayerRight)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0 && meetPlayerLeft)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (monsterState == MonsterState::DEAD)
        {
            if (dir.x > 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}
