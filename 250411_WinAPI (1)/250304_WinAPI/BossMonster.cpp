#include "pch.h"
#include "BossMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"
#include "Character.h"

BossMonster::BossMonster()
{
    // 확인 후 지워야함 
    objectScale = { GAME_TILE_SIZE / 253 , GAME_TILE_SIZE / 253 };
}

BossMonster::~BossMonster()
{
}

HRESULT BossMonster::Init()
{
    bossImage = ImageManager::GetInstance()->FindImage("Boss_Monster");
    player = new Character();

    colliderSize = { 80.0f, 80.0f };
    colliderOffsetY = 10.f;

    bossCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this);

    SetPos({ 550,350 });
    monsterHP = 10;
    damage = 1;
    moveSpeed = 55.0f;
    monsterState = MonsterState::IDLE;

    currFrame = { 0,0 };
    moveFrameInfo = { {0,3},{5,3} };
    attackMoveInfo = { {0,1},{6,1} };
    attackMoveStartInfo = { {0,1},{4,1} };
    attackFrameInfo = { { 0,2 }, { 3,2 } };
    dir = { 1,1 };

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

    playerPos = player->GetPos();
    playerPosBottom = playerPos.y + 128 / 2;
    playerPosLeftBottom = { playerPos.x - 128 / 2, playerPosBottom };
    playerPosRightBottom = { playerPos.x + 128 / 2,playerPosBottom };

    monsterPosTop = Pos.y - 128 / 2;
    monsterPosLeftTop = { Pos.x - 128 / 2,monsterPosTop };
    monsterPosRightTop = { Pos.x + 128 / 2, monsterPosTop };
	return S_OK;
}

void BossMonster::Release()
{
    __super::Release();
}

void BossMonster::Update(float TimeDelta)
{
    CheckTileCollision();
    CheckPlayerCollision();
    CheckItemCollision();

	// 기본 move
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
    else if (isTileTouchingRight || isTileTouchingLeft)
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

    // 플레이어한테 attackMove (점프) 
    MeetPlayer();
    MoveJump(TimeDelta);

    // 플레이어한테 밟히거나 공격 당했을 때 attack(구르기) 
    ApplyGravity(TimeDelta);
    FrameUpdate(TimeDelta);
}

void BossMonster::FrameUpdate(float TimeDelta)
{
    elipsedTime += TimeDelta;
    if (monsterState == MonsterState::MOVE)
    {
        currFrameInfo = moveFrameInfo;

        if (elipsedTime > 0.2f)
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

    // 점프
    if (monsterState == MonsterState::ATTACKMOVE)
    {
        currFrameInfo = attackMoveInfo;

        if (elipsedTime > 0.1f)
        {
            currFrame.x++;
            if (currFrame.x > attackMoveInfo.endFrame.x)
            {
                currFrame.x = attackMoveInfo.startFrame.x;
            }
            currFrame.y = attackMoveInfo.startFrame.y;
            elipsedTime = 0;
        }
    }

    // 구르기 
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

void BossMonster::CheckTileCollision()
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

    isTileTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE, false, debugTime)/* ||
       CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime)*/;

    isTileTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, false, debugTime) /*||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime)*/;

    isTileTouchingLeftBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
    isTileTouchingRightBottom = CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
}

void BossMonster::CheckPlayerCollision()
{
    float maxDist = 100.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT centerLeft = { Pos.x - colliderSize.x / 2, Pos.y + colliderOffsetY };
    FPOINT centerRight = { Pos.x + colliderSize.x / 2, Pos.y + colliderOffsetY };
    FPOINT centerTop = { Pos.x , Pos.y - colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isPlayerTouchingLeft = CollisionManager::GetInstance()->RaycastType({ centerLeft, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingRight = CollisionManager::GetInstance()->RaycastType({ centerRight, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::PLAYER, true, debugTime);

    /*isPlayerTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::PLAYER, true, debugTime);

    isPlayerTouchingCenterTop = CollisionManager::GetInstance()->RaycastType({ centerTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime);*/
}

void BossMonster::CheckItemCollision()
{
}

void BossMonster::MeetPlayer()
{
    if (monsterState == MonsterState::ATTACK)
    {
        meetPlayer = false;
        return;
    }

    if ((isPlayerTouchingRight || isPlayerTouchingLeft) && isTileTouchingLeftBottom && isTileTouchingRightBottom && !meetPlayer/*&& monsterState!=MonsterState::ATTACKMOVE*/)
    {        
        //meetPlayer = true;

        if (isPlayerTouchingRight /*&& dir.x > 0 && !isPlayerTouchingLeft*/)
        {
            dir.x = 1;
            monsterState = MonsterState::ATTACKMOVE;
            MoveJumpStart(300.f, 60.f);
            meetPlayer = true;
        }
        else if (isPlayerTouchingLeft /*&& dir.x < 0 && !isPlayerTouchingRight*/)
        {
            dir.x = -1;
            monsterState = MonsterState::ATTACKMOVE;
            MoveJumpStart(300.f, 120.f);
            meetPlayer = true;
        }

        if (monsterState == MonsterState::ATTACKMOVE && isPlayerTouchingRight)
        {
            dir.x = 1;
            monsterState == MonsterState::ATTACK;
            Move();
        }
        else if (monsterState == MonsterState::ATTACKMOVE && isPlayerTouchingRight)
        {
            dir.x = -1;
            monsterState == MonsterState::ATTACK;
            Move();
        }

    }
   

        
    // else if (isTileTouchingLeftBottom && isTileTouchingRightBottom)

}

void BossMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
}

void BossMonster::ApplyGravity(float TimeDelta)
{
    // 타일이 밑에 없을 때 떨어져요잇
    if (!isTileTouchingLeftBottom && !isTileTouchingRightBottom)
        Pos.y += moveSpeed * TimeDelta;
    /*else if (isTileTouchingLeftBottom && isTileTouchingRightBottom)
        monsterState = MonsterState::MOVE;*/
}

void BossMonster::ReverseMove()
{
}

void BossMonster::Detect(GameObject* obj)
{
    if (auto player = obj->GetType<Character>())
    {
        playerPos = player->GetPos();
        float playerPosBottom = playerPos.y + 20;
        float monsterPosTop = Pos.y;

        if (playerPosBottom < monsterPosTop)
        {
            monsterHP--;
            if (monsterHP == 0)
            {
                SetDestroy();
            }
        }
    }
    // 타일과 비교해서 타일 setDestroy
    else if (auto player = obj->GetType<Character>())
    {

    }
}

void BossMonster::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (bossImage)
    {
        if (/*monsterHP == 1 && */monsterState == MonsterState::MOVE)
        {
            if (dir.x > 0)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
        
        if (monsterState == MonsterState::ATTACKMOVE)
        {
            if (dir.x > 0)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (/*monsterHP == 1 && */monsterState == MonsterState::ATTACK)
        {
            if (dir.x > 0 && meetPlayerRight)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0 && meetPlayerLeft)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}
