#include "pch.h"
#include "SkeletonMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"
#include "Character.h"
#include "Particle.h"
#include "ParticleManager.h"


SkeletonMonster::SkeletonMonster()
{
    //int i = 5;
}

SkeletonMonster::~SkeletonMonster()
{
}

HRESULT SkeletonMonster::Init()
{
    skeletonImage = ImageManager::GetInstance()->FindImage("Skeleton_Monster");
    player = new Character();

    colliderSize = { 50.0f, 40.0f };
    colliderOffsetY = -10.f;

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

    MoveJumpStart(200, 90);

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
    ApplyGravity(TimeDelta);

    // 벽 안 만났을 때 
    if (!isTileTouchingRight && !isTileTouchingLeft && !isPlayerTouchingRight && !isPlayerTouchingLeft && isTileTouchingRightBottom && isTileTouchingLeftBottom && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::MOVE;
    }
    //벽 만났을 때 Update
    if (isTileTouchingRight && dir.x > 0 && monsterState == MonsterState::MOVE)
    {
        dir.x *= -1;
    }
    else if (isTileTouchingLeft && dir.x < 0 && monsterState == MonsterState::MOVE)
    {
        dir.x *= -1;
    }

    // Player 만났을 때 Update , 데미지도 포함 
    if (dir.x < 0 && isPlayerTouchingLeft && monsterState != MonsterState::DEAD)
    {
        monsterState = MonsterState::ATTACK;     
    }

    if (dir.x > 0 && isPlayerTouchingRight)
    {
       monsterState = MonsterState::ATTACK;
    }

    if (monsterState == MonsterState::DEAD)
    {
        DeadEvent(TimeDelta);
    }

    Move();
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

        if (elipsedTime > 0.1f)
        {
            if (currFrame.x > deadFrameInfo.endFrame.x)
            {
                currFrame.x--;
            }

            else
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

    isTileTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE, false, debugTime)/* ||
       CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime)*/;

    isTileTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, false, debugTime) /*||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime)*/;

    isTileTouchingLeftBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
    isTileTouchingRightBottom = CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
}

void SkeletonMonster::CheckPlayerCollision()
{
    float maxDist = 25.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT centerTop = { Pos.x , Pos.y - colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isPlayerTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::PLAYER, false, debugTime);

    isPlayerTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::PLAYER, false, debugTime);

    isPlayerTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::PLAYER, false, debugTime);

    isPlayerTouchingCenterTop = CollisionManager::GetInstance()->RaycastType({ centerTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, true, debugTime);
}

void SkeletonMonster::CheckItemCollision()
{
    float maxDist = 15.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isPlayerTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::PLAYER, false, debugTime);

    isPlayerTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::PLAYER, false, debugTime);

    isPlayerTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::PLAYER, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::PLAYER, false, debugTime);
}

void SkeletonMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
        Pos.x += dir.x * moveSpeed * time;
}

void SkeletonMonster::ApplyGravity(float TimeDelta)
{
    if (!isTileTouchingLeftBottom && !isTileTouchingRightBottom)
        Pos.y += 200.f * TimeDelta;
    else if (isTileTouchingLeftBottom && isTileTouchingRightBottom)
        Pos.y = Pos.y;
}

void SkeletonMonster::ReverseMove()
{
}

void SkeletonMonster::Detect(GameObject* obj)
{
    //if (isChangeCol)
    //{
    //    return;
    //}

    FPOINT playerPos = player->GetPos();
    

    if (auto player = obj->GetType<Character>())
    {

        playerPos = player->GetPos();
        float playerPosBottom = playerPos.y + 30;
        float monsterPosTop = Pos.y;

        if (playerPosBottom < monsterPosTop)
        {
            monsterState = MonsterState::DEAD;
            deadElipsedTime = 0.0f;

            //colliderSize = { 0.0f, 0.0f };
            if (!isChangeCol)
            {
                CollisionManager::GetInstance()->ChangeZ(CollisionMaskType::MONSTER, ORDER_Z::Z_CAVE, this);
                isChangeCol = true;
            }
           
            for (int i = 0; i < 5; i++)
            {
                float radius = 30.f; // 원하는 반경

                float angle = RandomRange(0.f, 360.f); // 또는 rand() % 360
                float dist = RandomRange(0.f, radius); // 균일한 거리

                // 라디안 변환
                float rad = angle * (3.141592f / 180.f);

                // 오프셋 계산
                FPOINT offset = {
                    cosf(rad) * dist,
                    sinf(rad) * dist
                };

                FPOINT spawnPos = Pos + offset;

                Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", spawnPos, (rand() % 360), 35.f, 2.5f, 4, 1);
                StarOption* starOp = new StarOption(30.f);

                particle->AddParticleOption(starOp);

                //3 6
            }
            {
                Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos, (rand() % 360), 55.f, 2.5f, 3, 6);
                StarOption* starOp = new StarOption(10.f);

                particle->AddParticleOption(starOp);

                //3 6
            }

            for (int i = 0; i < 5; i++)
            {
                FPOINT randPos = { RandomRange(-10, 10.f), RandomRange(-10, 10.f) };
                Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, 0.f, 30.f, 2.f, 0, 0);

                PhysicsOption* physicsOp = new PhysicsOption;
                SizeOption* sizeOp = new SizeOption(0.04f);
                TrailOption* trailOp = new TrailOption("Effect", 0.02f, 0.2f);

                float angleRad = RandomRange(3.141592f, 3.141592f * 2.0f);
                float speed = RandomRange(350.f, 575.0f);            // 속도도 랜덤

                velocity =
                {
                    sinf(angleRad) * speed,
                    -cosf(angleRad) * speed * 1.5f  // 135도 (왼쪽 위)
                };

                physicsOp->Init(velocity, 0.3f);
                //physicsOp->Init(velocity, 0.5f);

                particle->AddParticleOption(physicsOp);
                particle->AddParticleOption(sizeOp);
                particle->AddParticleOption(trailOp);
            }
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
        if (deadElipsedTime > 2.0f)
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
        if (/*monsterHP == 1 && */monsterState == MonsterState::MOVE || monsterState == MonsterState::IDLE)
        {
            if (dir.x > 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (/*monsterHP == 1 &&*/ monsterState == MonsterState::ATTACK)
        {
            if (dir.x > 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }

        if (monsterState == MonsterState::DEAD)
        {
            if (dir.x > 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0)
            {
                skeletonImage->FrameRender(renderTarget, pos.x, pos.y - 25, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}
