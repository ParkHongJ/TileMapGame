#include "pch.h"
#include "SnakeMonster.h"
#include "ImageManager.h"
#include "Image.h"
#include "CameraManager.h"
#include "CollisionManager.h"
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

    snakeCollider = new BoxCollider(
        { 20.0f , 15.0f },     // Offset
        { 85.0f, 80.0f },  // 
        CollisionMaskType::MONSTER,this
    );

   // Pos = { (WINSIZE_X / 2), (WINSIZE_Y / 2)};
    Pos = { 300, 250 };
    moveSpeed = 100.0f;
    monsterState = MonsterState::IDLE;

    currFrame = { 0,0 };
    moveFrameInfo = { {0,0},{5,0} }; 
    attackFrameInfo = { { 0,1 }, { 5,1 } };

    // attackMoveInfo;
    currFrameInfo = { { 0,0 }, {0, 0} };

    return S_OK;
}

void SnakeMonster::Release()
{
}

void SnakeMonster::Update(float TimeDelta)
{
    monsterState = MonsterState::MOVE;
    FrameUpdate(TimeDelta);
    Move();
}

void SnakeMonster::FrameUpdate(float TimeDelta)
{
    elipsedTime += TimeDelta;
    if (monsterState == MonsterState::MOVE && elipsedTime > 0.1f)
    {
       // currFrame.x = moveFrameInfo.startFrame.x;

        currFrame.x++;

        if (currFrame.x > moveFrameInfo.endFrame.x)
        {
            currFrame.x = moveFrameInfo.startFrame.x;
        }
        
        currFrame.y = moveFrameInfo.startFrame.y;
        elipsedTime = 0;
        //curFrameIndexX++;
        //if (curFrameIndexX > endFrameIndexX)
        //{
        //    curFrameIndexX = elipsedTime = 0;
        //}
        
    }

    //curFrameIndexX = int(elipsedTime);

    
}

void SnakeMonster::Move()
{
    float time = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    Pos.x += moveSpeed * time;
    // 벽이 있으면 다시 거꾸로 
}

void SnakeMonster::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (snakeImage)
    {
        if(monsterState == MonsterState::MOVE)
            snakeImage->FrameRender(renderTarget, pos.x, pos.y, currFrame.x, currFrame.y);
    }
}
