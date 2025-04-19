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

    snakeCollider = new BoxCollider(
        { 20.0f , 15.0f },     // Offset
        { 85.0f, 80.0f },  // 
        this
        );

   // Pos = { (WINSIZE_X / 2), (WINSIZE_Y / 2)};
    SetPos({ 230,250 });
    moveSpeed = 100.0f;
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

    return S_OK;
}

void SnakeMonster::Release()
{
}

void SnakeMonster::Update(float TimeDelta)
{
    CheckCollision();
   
    // 벽 안 만났을 때 
    if (!isTouchingRight && !isTouchingLeft)
    {
        monsterState = MonsterState::MOVE;
        Move();
    }
    //벽 만났을 때 Update
    /*if (isTouchingRight || isTouchingLeft)
    {
        monsterState = MonsterState::MOVE;
        dir.x *= -1;
        Move();
    }*/

    // Player 만났을 때 Update
    if (isTouchingRight || isTouchingLeft)
    {
        monsterState = MonsterState::ATTACK;
        meetPlayer = true;
    }

    //플레이어랑 만났을 때 경우 bool 값을 나눠줘야할 것 같음 
    /*if(isTouchingRight && isTouchingLeft)
    *   meetPlayer = true;
        monsterState = MonsterState::ATTACK*/

    // 벽에 부딪혔을때 
    //if (isTouchingRight)
    //{
    //    //meetWall = true;
    //    monsterState = MonsterState::MOVE;
    //    ReverseMove();
    //}

    //if (isTouchingLeft)
    //{
    //    monsterState = MonsterState::MOVE;
    //    Move();
    //}

   
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
    //if (monsterState == MonsterState::MOVE && elipsedTime > 0.1f)
    //{
    //   // currFrame.x = moveFrameInfo.startFrame.x;

    //    currFrame.x++;

    //    if (currFrame.x > moveFrameInfo.endFrame.x)
    //    {
    //        currFrame.x = moveFrameInfo.startFrame.x;
    //    }
    //    
    //    currFrame.y = moveFrameInfo.startFrame.y;
    //    elipsedTime = 0;
    //}

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

void SnakeMonster::CheckCollision()
{
    float maxDist = 15.0f;
    float debugTime = 0.0f;

    // Collider 기준 100x100일 때
    FPOINT leftTop = { Pos.x - 45.f, Pos.y - 30.f };
    FPOINT rightTop = { Pos.x + 45.f, Pos.y - 30.f };
    FPOINT leftBottom = { Pos.x - 30.f, Pos.y + 45.f };
    FPOINT rightBottom = { Pos.x + 30.f, Pos.y + 45.f };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, true, debugTime)/* ||
        CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime)*/;

    isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, true, debugTime) /*||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime)*/;

    isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, true, debugTime);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, true, debugTime);
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

void SnakeMonster::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (snakeImage)
    {
        if (monsterState == MonsterState::MOVE)
        {
            if (dir.x > 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, false);
            }

            if (dir.x < 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y,true);
            }
        }

        if (monsterState == MonsterState::ATTACK && meetPlayer)
        {
            if (dir.x > 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, false);
            }

            if (dir.x < 0)
            {
                snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y, true);
            }
        }
    }
}
