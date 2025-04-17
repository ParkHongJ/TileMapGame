#include "Character.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CharacterState.h"


IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);

HRESULT Character::Init()
{
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");

    state = &Character::idleState;
    state->Enter(this);
  
    SetPos({ WINSIZE_X / 2,0});

    dir = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };


    // Render
    frameTime = 0.0f;
    currFrameInd = { 0,0 };
    currFrameInfo = { { 0,0 }, {0, 0} };
    
    /*jumpFrameInfo = { {0, 9}, {7, 9} };
    attackFrameInfo = { {10, 12}, {15,12} };
    ropeFrameInfo = { {0, 12}, {9,12} };*/

    //colliderRect = {-40, -40, 40, 40};
    //colliderRect = {-10, -10, 10, 10};
    //colliderRect = { -10, -20, 10, 20 }; // width: 20, height: 40

    // settings
    speed = 200.f;
    attackSpeed = 3.0f;
    attackRate = 0.3f;

    // boolean
    isFlip = false;
    isInAir = false;
    isAttacking = false;

    jumpPressed = false;
    attackPressed = false;
    isLookUpLocked = false;

    InitAnimationMap();

    return S_OK;
}

void Character::Release()
{
    if (playerImage)
    {
        ImageManager::GetInstance()->DeleteImage("Tae_Player");
        playerImage = nullptr;

    }
}

void Character::Update(float TimeDelta)
{
    // HFSM
    if (state) 
        state->Update();
    

    // 충돌 방향 검사
    CheckCollision();

    // Gravity
    isInAir = !isTouchingBottom;
    ApplyGravity(TimeDelta);
    
    // Animation

    PlayAnimation();
   
}

void Character::InitAnimationMap()
{
     // IDLE
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_ALONE)}] =
    { {0, 0}, {0, 0}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_START)}] =
    { {0, 8}, {3, 8}, AnimationMode::FreezeAtX };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_STOP)}] =
    { {3, 8}, {3, 8}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_RELEASE)}] =
    { {4, 8}, {6, 8}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_START)}] =
    { {0, 1}, {2, 1}, AnimationMode::FreezeAtX};

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_STOP)}] =
    { {2, 1}, {2, 1}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };

    // MOVE
    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_ALONE)}] =
    { {1, 0}, {8, 0}, AnimationMode::Loop };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_LOOP)}] =
    { {5, 1}, {11, 1}, AnimationMode::Loop };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_START)}] =
    { {0, 1}, {2, 1}, AnimationMode::FreezeAtX };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };

    // ATTACK
    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_ALONE)}] =
    { {0, 4}, {5, 4}, AnimationMode::Hold };



}
//
//void Character::SetAnimationRange(PlayerState state)
//{
//    switch (state)
//    {
//        case PlayerState::IDLE:                  currFrameInfo = { {0, 0},  {0, 0} };            break;
//        case PlayerState::MOVE:                  currFrameInfo = { {1, 0},  {9, 0} };            break;
//        case PlayerState::LOOKUP_START:
//        case PlayerState::LOOKUP_RELEASE:        currFrameInfo = { {0, 8},  {6, 8} };            break;
//        case PlayerState::ONPET_LOOKUP_START:     currFrameInfo = { {8, 8},  {14, 8} };           break;
//        case PlayerState::LOOKDOWN_START:
//        case PlayerState::LOOKDOWN_RELEASE:      currFrameInfo = { {0, 1},  {4, 1} };            break;
//        case PlayerState::LOOKDOWN_MOVE:         currFrameInfo = { {5, 1},  {11, 1}};            break;
//        case PlayerState::LOOKDOWN_IDLE:         currFrameInfo = { {2, 1},  {2, 1} };            break;
//        case PlayerState::CLIMB_LADDER:          currFrameInfo = { {0, 6},  {5, 7} };            break;
//        case PlayerState::CLIMB_ROPE:            currFrameInfo = { {0, 7},  {9, 7} };            break;
//        case PlayerState::ON_NOTTAMEDPET:        currFrameInfo = { {4, 11}, {9, 11}};            break;
//        case PlayerState::ONPET_IDLE:            currFrameInfo = { {7, 8},  {7, 8} };            break;
//        case PlayerState::ALMOST_FALL:           currFrameInfo = { {0, 3},  {7, 3} };            break;
//        case PlayerState::ATTACK:                currFrameInfo = { {0, 4},  {5, 4} };            break;
//        case PlayerState::HANG:                  currFrameInfo = { {8, 3},  {11, 3}};            break;
//        case PlayerState::PUSH:                  currFrameInfo = { {6, 6},  {11, 6}};            break;
//        case PlayerState::DIE:                   currFrameInfo = { {9, 0},  {9, 0} };            break;
//        case PlayerState::FALL:                  currFrameInfo = { {0, 2},  {3, 2} };            break;
//        case PlayerState::FALL_STUNEFFECT:       currFrameInfo = { {0, 13}, {11, 13} };          break;
//        case PlayerState::ENTER_TUNNEL:          currFrameInfo = { {0, 5},  {5, 5} };            break;
//        case PlayerState::EXIT_TUNNEL:           currFrameInfo = { {6, 5},  {11, 5}};            break;
//        
//    default:
//        //currFrameInfo = { {0, 0}, {0, 0} };
//        break;
//    }
//}

void Character::SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState)
{
    auto key = std::make_pair(stateClassNum, subState);
    auto it = animationMap.find(key);
    if (it != animationMap.end())
    {
        // 프레임 초기화는 진짜로 바뀐 경우에만 하게끔
        if (currFrameInfo.startFrame.x != it->second.startFrame.x ||
            currFrameInfo.startFrame.y != it->second.startFrame.y ||
            currFrameInfo.endFrame.x != it->second.endFrame.x ||
            currFrameInfo.endFrame.y != it->second.endFrame.y ||
            currFrameInfo.mode != it->second.mode)
        {
            currFrameInd = it->second.startFrame;
            frameTime = 0.f;

            currFrameInfo = it->second;
        }

    }
}


void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{
    if (state)
    {
        char buf[256];
        sprintf_s(buf,
            "▶ Render Frame: (%d,%d)\n▶ State: %s  \n LookDownLocked : %d Speed: %f Velocity : x = %f y = %f",
            currFrameInd.x, currFrameInd.y, state->GetSubStateName(), isLookDownLocked, speed, velocity.x, velocity.y
            );

        OutputDebugStringA(buf);
    }

    if (playerImage)
    {
        playerImage->FrameRender(renderTarget, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y, isFlip);
    }


}


void Character::PlayAnimation()
{
    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    frameTime += TimeDelta;
    
    if (frameTime >= ANIMATION_FRAME_TIME)
    {
        frameTime = 0.f;

        switch (currFrameInfo.mode)
        {
            case AnimationMode::Loop:
                currFrameInd.x++;
                if (currFrameInd.x > currFrameInfo.endFrame.x)
                    currFrameInd.x = currFrameInfo.startFrame.x;
                break;

            case AnimationMode::FreezeAtX:
            case AnimationMode::Hold:
                if (currFrameInd.x < currFrameInfo.endFrame.x)
                    currFrameInd.x++;
                break;
        }

    }

}

void Character::ChangeState(CharacterState* newState)
{
    if (state) state->Exit();
    state = newState;
    if (state) state->Enter(this);
}

bool Character::PressAnyKey(void)
{
    KeyManager* km = KeyManager::GetInstance();

    for (int key = 0; key < MAX_KEY_COUNT; ++key)
    {
        if (km->IsOnceKeyDown(key))
            return true;
    }
    return false;
}

float Character::GetVelocitySize()
{
    return sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

float Character::GetYVelocity()
{
    return velocity.y;
}

bool Character::GetIsLookUpLocked()
{
    return isLookUpLocked;
}

bool Character::GetIsLookDownLocked()
{
    return isLookDownLocked;
}

POINT Character::GetCurrFrameInd() const
{
    return currFrameInd;
}

FrameInfo Character::GetCurrFrameInfo() const
{
    return currFrameInfo;
}

bool Character::GetCurrAnimEnd()
{
    if (currFrameInd.x == currFrameInfo.endFrame.x) return true;
    else return false;
}

void Character::ApplyGravity(float TimeDelta)
{
    if (isInAir)
    {
        velocity.y += gravity * TimeDelta;
        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;

        Pos.y += velocity.y * TimeDelta;
    }
    else
    {
        velocity.y = 0.f; // 바닥에 있으면 낙하속도 초기화
    }
}

void Character::CheckCollision()
{
    float maxDist = 5.f;
    float debugTime = 3.0f;

    // Collider 기준 100x100일 때
    FPOINT leftTop = { Pos.x - 30.f, Pos.y - 30.f };
    FPOINT rightTop = { Pos.x + 30.f, Pos.y - 30.f };
    FPOINT leftBottom = { Pos.x - 30.f, Pos.y + 30.f };
    FPOINT rightBottom = { Pos.x + 30.f, Pos.y + 30.f };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;


    isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime);

    isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime);

    isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, true, debugTime);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, true, debugTime);
}

void Character::Move(int dirX)
{
    // 이동 방향에 따라 충돌 여부 확인
    if ((dirX < 0 && isTouchingLeft) || (dirX > 0 && isTouchingRight))
        return;

    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    isFlip = dirX > 0 ? false : true;
    Pos.x += speed * dirX * TimeDelta;
}

