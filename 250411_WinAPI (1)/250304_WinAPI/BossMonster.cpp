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
}

BossMonster::~BossMonster()
{
}

HRESULT BossMonster::Init()
{
    bossImage = ImageManager::GetInstance()->FindImage("Boss_Monster");
    player = new Character();

    colliderSize = { 50.0f, 30.0f };
    colliderOffsetY = 10.f;

    bossCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this);

    SetPos({ 300,270 });
    monsterHP = 1;
    damage = 1;
    moveSpeed = 80.0f;
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
	return E_NOTIMPL;
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

    // �÷��̾����� attackMove (����) 

    // �÷��̾����� �����ų� ���� ������ �� attack(������) 
}

void BossMonster::FrameUpdate(float TimeDelta)
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

    // ����
    if (monsterState == MonsterState::ATTACKMOVE)
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

    // ������ 
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

    // Collider ���� 
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
}

void BossMonster::CheckItemCollision()
{
}

void BossMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
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
            SetDestroy();
        }

        // Ÿ�ϰ� ���ؼ� Ÿ�� setDestroy

    }

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
