#include "Character.h"
#include "ImageManager.h"
#include "Image.h"

IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);


HRESULT Character::Init()
{
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");

    SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });

    state = nullptr;
    dir = { 0.0f, 0.0f };

    velocity = { 0.0f, 0.0f };

    currFrameInd = { 0,0 };

    frameTime = 0.0f;
    currFrameInfo.startFrame = { 0,0 };
    currFrameInfo.endFrame = { 0,0 };

    jumpFrameInfo = { {0, 9}, {7, 9} };
    attackFrameInfo = { {10, 12}, {15,12} };
    ropeFrameInfo = { {0, 12}, {9,12} };

 
    speed = 200.f;
    attackSpeed = 3.0f;
    attackRate = 0.3f;

    isFlip = false;
    isInAir = false;
    isAttacking = false;

    jumpPressed = false;
    attackPressed = false;
    isLookUpPaused = false;

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

    if (state) 
        state->Update(TimeDelta);
    

    // velocity 중력 관련 업데이트

    
   
}

void Character::InitAnimationMap()
{
    // IDLE
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_ALONE)}] = { {0, 0}, {0, 0} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_START)}] = { {0, 8}, {3, 8} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_STOP)}] = { {3, 8}, {3, 8} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_RELEASE)}] = { {4, 8}, {6, 8} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_START)}] = { {0, 1}, {2, 1} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_STOP)}] = { {2, 1}, {2, 1} };
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_RELEASE)}] = { {2, 1}, {4, 1} };

    // MOVE
    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_ALONE)}] = { {1, 0}, {9, 0} };
    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN)}] = { {5, 1}, {11, 1} };

    // ATTACK

    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_ALONE)}] = { {0, 4}, {5, 4} };

    // INTERACTION



}

void Character::SetAnimationRange(PlayerState state)
{
    switch (state)
    {
        case PlayerState::IDLE:                  currFrameInfo = { {0, 0},  {0, 0} };            break;
        case PlayerState::MOVE:                  currFrameInfo = { {1, 0},  {9, 0} };            break;
        case PlayerState::LOOKUP_START:
        case PlayerState::LOOKUP_RELEASE:        currFrameInfo = { {0, 8},  {6, 8} };            break;
        case PlayerState::ONPET_LOOKUP_START:     currFrameInfo = { {8, 8},  {14, 8} };           break;
        case PlayerState::LOOKDOWN_START:
        case PlayerState::LOOKDOWN_RELEASE:      currFrameInfo = { {0, 1},  {4, 1} };            break;
        case PlayerState::LOOKDOWN_MOVE:         currFrameInfo = { {5, 1},  {11, 1}};            break;
        case PlayerState::LOOKDOWN_IDLE:         currFrameInfo = { {2, 1},  {2, 1} };            break;
        case PlayerState::CLIMB_LADDER:          currFrameInfo = { {0, 6},  {5, 7} };            break;
        case PlayerState::CLIMB_ROPE:            currFrameInfo = { {0, 7},  {9, 7} };            break;
        case PlayerState::ON_NOTTAMEDPET:        currFrameInfo = { {4, 11}, {9, 11}};            break;
        case PlayerState::ONPET_IDLE:            currFrameInfo = { {7, 8},  {7, 8} };            break;
        case PlayerState::ALMOST_FALL:           currFrameInfo = { {0, 3},  {7, 3} };            break;
        case PlayerState::ATTACK:                currFrameInfo = { {0, 4},  {5, 4} };            break;
        case PlayerState::HANG:                  currFrameInfo = { {8, 3},  {11, 3}};            break;
        case PlayerState::PUSH:                  currFrameInfo = { {6, 6},  {11, 6}};            break;
        case PlayerState::DIE:                   currFrameInfo = { {9, 0},  {9, 0} };            break;
        case PlayerState::FALL:                  currFrameInfo = { {0, 2},  {3, 2} };            break;
        case PlayerState::FALL_STUNEFFECT:       currFrameInfo = { {0, 13}, {11, 13} };          break;
        case PlayerState::ENTER_TUNNEL:          currFrameInfo = { {0, 5},  {5, 5} };            break;
        case PlayerState::EXIT_TUNNEL:           currFrameInfo = { {6, 5},  {11, 5}};            break;
        
    default:
        //currFrameInfo = { {0, 0}, {0, 0} };
        break;
    }
}

void Character::SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState)
{
    auto key = std::make_pair(stateClassNum, subState);
    auto it = animationMap.find(key);
    if (it != animationMap.end())
    {
        currFrameInfo = it->second;
        currFrameInd = currFrameInfo.startFrame;
        frameTime = 0.0f;
    }
}


void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{

    if (state)
    {
        char debugText[128];
        sprintf_s(debugText, "CurrentState: %s\n", state->GetStateName());
        OutputDebugStringA(debugText);  // ANSI 버전
    }

    //playerImage->FrameRender(renderTarget, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y);
    //playerImage->Render(renderTarget, Pos.x, Pos.y);
}

void Character::PlayAnimation(float TimeDelta)
{

    frameTime += TimeDelta;
    
    if (frameTime >= ANIMATION_FRAME_TIME)
    {
        frameTime = 0.f;

        currFrameInd.x++;
        if (currFrameInd.x >= currFrameInfo.endFrame.x)
            currFrameInd.x = currFrameInfo.startFrame.x;

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

bool Character::GetIsLookUpPaused()
{
    return isLookUpPaused;
}

bool Character::GetIsLookDownPaused()
{
    return isLookDownPaused;
}

void Character::Move(int dirX, float timeDelta)
{
    isFlip = dirX > 0 ? false : true;
    Pos.x += speed * dir.x * timeDelta;
}

void Character::LookUp(float TimeDelta)
{ 

}

void Character::LookDown(float TimeDelta)
{

}

