#include "pch.h"
#include "BossMonster.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "Collider.h"
#include "TimerManager.h"
#include "Character.h"
#include "Tile.h"
#include "Item.h"
#include "GunBullet.h"

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
    tile = new Tile();
    items = new Item();
    bullet = new GunBullet();

    colliderSize = { 80.0f, 70.0f };
    colliderOffsetY = 5.f;

    bossCollider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        CollisionMaskType::MONSTER, this);

    SetPos({ 550,200 });
    monsterHP = 1000;
    damage = 2;
    moveSpeed = 55.0f;
    heatCoolTime = 0.0f;
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

    wasMove = false;
    wasAttackMove = false;
    wasAttack = false;

    playerPos = player->GetPos();
    playerPosBottom = playerPos.y + 128 / 2;
    playerPosLeftBottom = { playerPos.x - 128 / 2, playerPosBottom };
    playerPosRightBottom = { playerPos.x + 128 / 2,playerPosBottom };

    monsterPosTop = Pos.y - 128 / 2;
    monsterPosLeftTop = { Pos.x - 128 / 2,monsterPosTop };
    monsterPosRightTop = { Pos.x + 128 / 2, monsterPosTop };
    MoveJumpStart(200,90);
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
    ApplyGravity(TimeDelta);
    isInAir = bPhysics || velocity.y != 0.f;
	// 기본 move
	// 벽 안 만났을 때 
    if (monsterState != MonsterState::ATTACK && !isTileTouchingRight && !isTileTouchingLeft && isTileTouchingRightBottom && isTileTouchingLeftBottom && !isPlayerTouchingLeft && !isPlayerTouchingRight)
    {
        monsterState = MonsterState::MOVE;
    }
    //벽 만났을 때 Update
    if ((isTileTouchingRight || isTileTouchingRightCenter) && dir.x > 0 && monsterState == MonsterState::MOVE)
    {
        dir.x *= -1;
    }
    else if ((isTileTouchingLeft || isTileTouchingLeftCenter) && dir.x < 0 && monsterState == MonsterState::MOVE)
    {
        dir.x *= -1;
    }

    //rolling 중에 바닥이 없으면 ApplyGravity 
    //rolling 중에 바닥이 있으면 구르면서 앞으로 
    MeetPlayer(TimeDelta);
    MoveJump(TimeDelta);
    
    Move();
    FrameUpdate(TimeDelta);
   
}

void BossMonster::FrameUpdate(float TimeDelta)
{
    elipsedTime += TimeDelta;
    if (monsterState == MonsterState::MOVE || monsterState == MonsterState::WAITATTACK || monsterState == MonsterState::IDLE)
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
    FPOINT centerLeft = { Pos.x - colliderSize.x / 2, Pos.y + colliderOffsetY + 15};
    FPOINT centerRight = { Pos.x + colliderSize.x / 2, Pos.y + colliderOffsetY + 15};
    FPOINT centerTop = { Pos.x , Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT centerBottom = { Pos.x , Pos.y + colliderSize.y / 2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;
    // To do 가운데 레이선 추가 벽 뿌수는용 
    isTileTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE, false, debugTime) ||
       CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime);

    isTileTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, false, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime);

    isTileTouchingLeftBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
    isTileTouchingRightBottom = CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);

    isTileTouchingLeftCenter = CollisionManager::GetInstance()->RaycastType({ centerLeft, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);
    isTileTouchingRightCenter = CollisionManager::GetInstance()->RaycastType({ centerRight, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, false, debugTime);


}

void BossMonster::CheckPlayerCollision()
{
    float maxDist = 200.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    FPOINT centerLeft = { Pos.x - colliderSize.x / 2, Pos.y + colliderOffsetY };
    FPOINT centerRight = { Pos.x + colliderSize.x / 2, Pos.y + colliderOffsetY + 20};
    FPOINT centerTop = { Pos.x , Pos.y - colliderSize.y / 2 + colliderOffsetY + 20};

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

void BossMonster::MeetPlayer(float TimeDelta)
{
    bool isOnGround = isTileTouchingLeftBottom || isTileTouchingRightBottom;
   /* if (monsterState == MonsterState::ATTACK || monsterState == MonsterState::ATTACKMOVE || monsterState == MonsterState::WAITATTACK)
        return;*/

    if (monsterState == MonsterState::IDLE && ((isPlayerTouchingLeft && dir.x > 0) || (isPlayerTouchingRight && dir.x < 0)) && isOnGround)
    {
        monsterState = MonsterState::MOVE;
        //Move();
        //return;
    }

    if (monsterState == MonsterState::MOVE && isOnGround)
    {
        if (isPlayerTouchingLeft && dir.x > 0)
        {
            dir.x *= -1;
        }
        else if (isPlayerTouchingRight && dir.x < 0)
        {
            dir.x *= -1;
        }
        // MOVE 상태일 때 attackMove로 상태 변환
        if (isPlayerTouchingLeft && dir.x < 0 )
        {    
           
            MoveJumpStart(550.f, 100.f);
            wasMove = true;
            monsterState = MonsterState::ATTACKMOVE;
        }
        else if (isPlayerTouchingRight && dir.x > 0)
        {            
             
            MoveJumpStart(550.f, 70.f);
            wasMove = true;
            monsterState = MonsterState::ATTACKMOVE;
        }
    }


    if (monsterState == MonsterState::ATTACKMOVE && wasMove && isOnGround)
    {           
        //attackDuration += TimeDelta;
        wasMove = false;
        //// Move();
        ///*if (!isOnGround)
        //    return;
        //else*/
        //if (attackDuration > attackCoolTime)
        //{
        //    attackDuration = 0.f;
        //    monsterState = MonsterState::WAITATTACK;
        //}
        monsterState == MonsterState::WAITATTACK;
        wasAttackMove = true;
    }

    if (monsterState == MonsterState::WAITATTACK && wasAttackMove && isOnGround)
    {
        //if (dir.x < 0)
        //{
        //   // if (isPlayerTouchingLeft)
        //    monsterState = MonsterState::ATTACK;
        //}
        //else if (dir.x > 0)
        //{
        //   // if (isPlayerTouchingRight)
        //        monsterState = MonsterState::ATTACK;
        //}
       /* if (((isPlayerTouchingLeft && dir.x < 0) || (isPlayerTouchingRight && dir.x > 0)) && isOnGround)
        {
            monsterState = MonsterState::ATTACK;
        }
        else if ((isPlayerTouchingLeft && dir.x > 0) || (isPlayerTouchingRight && dir.x < 0))
        {
            monsterState = MonsterState::MOVE;
        }*/
        if (dir.x > 0)
        {
            if ((isPlayerTouchingRight) && isOnGround)
            {
                monsterState = MonsterState::ATTACK;
            }
            else if ((isPlayerTouchingLeft) && isOnGround)
            {
                dir.x = 1;
                monsterState = MonsterState::ATTACK;
            }
        }
        if (dir.x < 0)
        {
            if ((isPlayerTouchingLeft) && isOnGround)
            {
                monsterState = MonsterState::ATTACK;
            }
            else if ((isPlayerTouchingRight) && isOnGround)
            {
                dir.x = -1;
                monsterState = MonsterState::ATTACK;
            }
        }

        /* if ((isPlayerTouchingRight && dir.x > 0) || (isPlayerTouchingLeft && dir.x < 0) && isOnGround)
        {
            monsterState = MonsterState::ATTACK;
        }*/
        else if ((isPlayerTouchingLeft && dir.x > 0) || (isPlayerTouchingRight && dir.x < 0))
        {
            monsterState = MonsterState::MOVE;
        }
    }


    if (monsterState == MonsterState::ATTACK)
    {
        attackDuration += TimeDelta;
        wasAttackMove = false;
        //Move();
        if (attackDuration <= rollingTime)
            Move();
        else if (attackDuration > rollingTime)
        {
            attackDuration = 0.f;
            monsterState = MonsterState::WAITATTACK;
        }
        wasAttack = true;
    }
    else if (monsterState == MonsterState::WAITATTACK && wasAttack)
    {
        monsterState = MonsterState::MOVE;
        wasAttack = false;
    }


    //AttackMove(점프)를 한 후 플레이어가 아직 레이 범위 안에 있을 때 
    //if (monsterState == MonsterState::ATTACKMOVE)
    //{
    //    if (isInAir || !isOnGround)
    //        monsterState == MonsterState::ATTACKMOVE;

    //    else if (!isInAir || isOnGround)
    //    {
    //        if ((isPlayerTouchingLeft && dir.x < 0) || (isPlayerTouchingRight && dir.x > 0))
    //        {
    //            monsterState = MonsterState::ATTACK;
    //            //return;
    //            //meetPlayer = true;
    //        }
    //        else if ((isPlayerTouchingLeft && dir.x > 0) || (isPlayerTouchingRight && dir.x < 0))
    //        {
    //            monsterState = MonsterState::MOVE;
    //            Move();
    //        }
    //    }
    //   /* else if (isInAir)
    //        monsterState == MonsterState::ATTACKMOVE;*/
    //}

    //if (monsterState == MonsterState::ATTACK)
    //{
    //    
    //}
}

void BossMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    if (monsterState == MonsterState::MOVE)
    {
        moveSpeed = 55.f;
        Pos.x += dir.x * moveSpeed * time;
    }
    if (monsterState == MonsterState::ATTACK)
    {
        moveSpeed = 80.f;
        Pos.x += dir.x * moveSpeed * time;
    }
   /* if (monsterState == MonsterState::MOVE || monsterState == MonsterState::ATTACK)
        Pos.x += dir.x * moveSpeed * time;*/
}

void BossMonster::ApplyGravity(float TimeDelta)
{
    // 타일이 밑에 없을 때 떨어져요잇
    if (!isTileTouchingLeftBottom && !isTileTouchingRightBottom)
        Pos.y += 200.f * TimeDelta;
    //else if (isTileTouchingLeftBottom || isTileTouchingRightBottom)
    //{
    //    //Pos.y 
    //    Move();
    //}
     
    if (monsterState == MonsterState::ATTACK)
    {
        if (!isTileTouchingLeftBottom && !isTileTouchingRightBottom)
            Pos.y += 400.f * TimeDelta;
        /*else if (isTileTouchingLeftBottom || isTileTouchingRightBottom)
        {
           
            Move();
        }*/
    }
        //monsterState = MonsterState::MOVE;
}

void BossMonster::ReverseMove()
{
}

void BossMonster::Detect(GameObject* obj)
{
    if (auto player = obj->GetType<Character>())
    {
        float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
        playerPos = player->GetPos();
        float playerPosBottom = playerPos.y + 30;
        float monsterPosTop = Pos.y;

        if (playerPosBottom < monsterPosTop)
        {
            MoveJumpStart(200.f, 90.f);
            monsterState = MonsterState::ATTACK;      
        }
    }
 
    else if (auto player = obj->GetType<Character>())
    {

    }
    // 타일과 비교해서 타일 Destroy
    if (auto tile = obj->GetType<Tile>())
    {
        FPOINT tilePos = tile->GetPos(); 
        float tileTop = tilePos.y - 20;
        float monsterPosBottom = Pos.y;

        if (monsterState == MonsterState::ATTACK)
        {
            if(tilePos.y < monsterPosBottom + 30.f)
                tile->Destruction();
        }
    }

    if (auto items = obj->GetType<Item>())
    {
        FPOINT itemsPos = items->GetPos();
        float itemsRight = itemsPos.x + 5;
        float itemsLeft = itemsPos.y - 5;
        
        if (itemsRight > Pos.x - 20)
        {
            MoveJumpStart(250.f, 70.f);
            if (dir.x > 0)
            {
                dir.x *= -1;
            }
        }
        else if (itemsLeft < Pos.x + 20)
        {
            MoveJumpStart(250.f, 110.f);
            if (dir.x < 0)
            {
                dir.x *= -1;
            }
        }
    }

    if (auto gunBullet = obj->GetType<GunBullet>())
    {
        FPOINT bulletPos = bullet->GetPos();
        float bulletRight = bulletPos.x + 5;
        float bulletLeft = bulletPos.y - 5;

        if (bulletRight > Pos.x - 20)
        {
            MoveJumpStart(250.f, 70.f);
            /*if (dir.x > 0)
            {
                dir.x *= -1;
            }*/
        }
        else if (bulletLeft < Pos.x + 20)
        {
            MoveJumpStart(250.f, 110.f);
            /*if (dir.x < 0)
            {
                dir.x *= -1;
            }*/
        }
    }
}

void BossMonster::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (bossImage)
    {
        if (/*monsterHP == 1 && */monsterState == MonsterState::MOVE || monsterState == MonsterState::WAITATTACK)
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
            if (dir.x > 0)
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, false);
            }

            if (dir.x < 0 )
            {
                bossImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, objectScale.x, objectScale.y, true);
            }
        }
    }
}
