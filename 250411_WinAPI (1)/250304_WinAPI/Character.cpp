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
    currMaxFrameInd = { 9,0 };

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
        state->Update(this, TimeDelta);
    

    //HandleInput(prevState, TimeDelta);

   
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


//bool Character::ShouldResetAnimation(PlayerState prevState, PlayerState newState)
//{
//    if ((prevState == PlayerState::LOOKUP_START && state == PlayerState::LOOKUP_RELEASE) ||
//        (prevState == PlayerState::LOOKDOWN_START && state == PlayerState::LOOKDOWN_RELEASE))
//        return true;
//    else return false;
//
//}


void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{
   /* {
        TCHAR buf[256];
        wsprintf(buf, TEXT("Render Frame: %d, %d"), currFrameInd.x, currFrameInd.y);
        TextOut(renderTarget, 20, 100, buf, wcslen(buf));
    }
    {
        TCHAR buf[256]; 
        wsprintf(buf, TEXT("PlayerState: %s"), PlayerStateToString(state));
        TextOut(renderTarget, 20, 120, buf, wcslen(buf));
    }*/

    if (state)
    {
        char debugText[128];
        sprintf_s(debugText, "CurrentState: %s\n", state->GetStateName());
        OutputDebugStringA(debugText);  // ANSI 버전
    }

    //playerImage->FrameRender(renderTarget, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y);//, isFlip, true);
    playerImage->Render(renderTarget, Pos.x, Pos.y);
}

//
//void Character::HandleInput(PlayerState prevState, float TimeDelta)
//{
//    KeyManager* km = KeyManager::GetInstance();
//
//    bool hasInput = false;
//    dir = { 0.0f , 0.0f };
//
//#pragma region MOVESECTION
//
//    if (km->IsStayKeyDown(VK_RIGHT))
//    {
//        dir.x += 1;
//        isFlip = false;
//
//        if (km->IsStayKeyDown(VK_DOWN))
//        {
//            state = PlayerState::LOOKDOWN_MOVE;
//            speed = 100.f;
//        }
//        else
//        {
//            state = PlayerState::MOVE;
//            speed = 200.f;
//        }
//
//        Pos.x += speed * dir.x * TimeDelta;
//        hasInput = true;
//    }
//    else if (km->IsStayKeyDown(VK_LEFT))
//    {
//        dir.x += -1;
//        isFlip = true;
//
//        if (km->IsStayKeyDown(VK_DOWN))
//        {
//            state = PlayerState::LOOKDOWN_MOVE;
//            speed = 100.f;
//        }
//        else
//        {
//            state = PlayerState::MOVE;
//            speed = 200.f;
//        }
//
//        Pos.x += speed * dir.x * TimeDelta;
//        hasInput = true;
//    }
//    
//    if ((km->IsOnceKeyUp(VK_LEFT) || km->IsOnceKeyUp(VK_RIGHT)) && km->IsStayKeyDown(VK_DOWN))
//    {
//        switch (prevState)
//        {
//        case PlayerState::LOOKDOWN_MOVE:
//            state = PlayerState::LOOKDOWN_IDLE;
//            break;
//        }
//      //  return;
//    }
//
//    if (km->IsStayKeyDown(VK_DOWN) && state == PlayerState::LOOKDOWN_IDLE)
//    {
//        
//    }
//  
//
//
//#pragma endregion
//
//#pragma region JUMP
//
//    if (km->IsOnceKeyDown(VK_SPACE))
//    {
//        jumpPressed = true;
//        hasInput = true;
//    }
//
//#pragma endregion
//
//#pragma region LOOKUP & LOOKDOWN
//
//    if (state != PlayerState::MOVE)
//    {
//        // LOOK UP
//        if (km->IsStayKeyDown(VK_UP))
//        {
//            if (state != PlayerState::LOOKUP_START &&
//                state != PlayerState::LOOKUP_RELEASE)
//            {
//                state = PlayerState::LOOKUP_START;
//                hasInput = true;
//            }
//        }
//
//        if (state == PlayerState::LOOKUP_START && km->IsOnceKeyUp(VK_UP))
//        {
//            state = PlayerState::LOOKUP_RELEASE;
//            isLookUpPaused = false;
//        }
//
//        // LOOK DOWN
//        if (km->IsStayKeyDown(VK_DOWN))
//        {
//            if (state != PlayerState::LOOKDOWN_START &&
//                state != PlayerState::LOOKDOWN_RELEASE &&
//                state != PlayerState::LOOKDOWN_MOVE)
//            {
//                state = PlayerState ::LOOKDOWN_START;
//                hasInput = true;
//            }
//        }
//
//        if (km->IsOnceKeyUp(VK_DOWN))
//        {
//            switch (prevState)
//            {
//            case PlayerState::LOOKDOWN_MOVE:
//            case PlayerState::LOOKDOWN_IDLE:
//                state = PlayerState::LOOKDOWN_RELEASE;
//                break;
//            }
//        }
//
//        if (state == PlayerState::LOOKDOWN_START && km->IsOnceKeyUp(VK_DOWN))
//        {
//            state = PlayerState::LOOKDOWN_RELEASE;
//            isLookDownPaused = false;
//        }
//    }
//
//#pragma endregion
//
//    if (!hasInput &&
//        state != PlayerState::LOOKUP_START &&
//        state != PlayerState::LOOKUP_RELEASE &&
//        state != PlayerState::LOOKDOWN_START &&
//        state != PlayerState::LOOKDOWN_RELEASE &&
//        state != PlayerState::LOOKDOWN_MOVE)
//    {
//        state = PlayerState::IDLE;
//    }
//}

void Character::PlayAnimation(unsigned int stateClassNum, unsigned int subState)
{

    // map 사용

    auto key = std::make_pair(stateClassNum, subState);
    auto it = animationMap.find(key);
    if (it != animationMap.end())
    {
        currFrameInfo = it->second;
        currFrameInd = currFrameInfo.startFrame;
        frameTime = 0.0f;
    }






}

void Character::ChangeState(CharacterState* newState)
{
    if (state) state->Exit(this);
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
    // 특정 프레임 도달 시 불리언 변경? 
}

void Character::LookDown(float TimeDelta)
{
    // 특정 프레임 도다 ㄹ시 불리언 변경 ? 
}


//
//void Character::UpdateIdle()
//{
//    currFrameInd.x = 0;
//    currFrameInd.y = 0;
//}
//
//void Character::UpdateMove(float TimeDelta)
//{
//    isLookUpPaused = false;
//    isLookDownPaused = false;
//
//    if (dir.x != 0)
//    {
//        frameTime += TimeDelta;
//
//        if (frameTime >= ANIMATION_FRAME_TIME)
//        {
//            frameTime = 0.f;
//
//            currFrameInd.x++;
//            if (currFrameInd.x >= currFrameInfo.endFrame.x)
//                currFrameInd.x = currFrameInfo.startFrame.x;
//        }
//    }
//    
//}
//
//void Character::UpdateLookUp(float TimeDelta)
//{
//    frameTime += TimeDelta;
//
//    if (frameTime >= ANIMATION_FRAME_TIME)
//    {
//        frameTime = 0.f;
//
//        if (state == PlayerState::LOOKUP_START)
//        {
//            if (!isLookUpPaused)
//                currFrameInd.x++;
//
//             if (currFrameInd.x == currFrameInfo.endFrame.x / 2)
//                 isLookUpPaused = true;
//        }
//
//        if (state == PlayerState::LOOKUP_RELEASE)
//        {
//            currFrameInd.x++;
//
//            if (currFrameInd.x >= currFrameInfo.endFrame.x)
//            {
//                currFrameInd.x = currFrameInfo.endFrame.x;
//                state = PlayerState::IDLE;
//            }
//        }
//    }
//}
//
//void Character::UpdateLookDown(float TimeDelta)
//{
//    frameTime += TimeDelta;
//
//
//    if (frameTime >= ANIMATION_FRAME_TIME)
//    {
//        frameTime = 0.f;
//
//        if (state == PlayerState::LOOKDOWN_START)
//        {
//            if (!isLookDownPaused)
//                currFrameInd.x++;
//
//            if (currFrameInd.x == currFrameInfo.endFrame.x / 2)
//                isLookDownPaused = true;
//        }
//
//        if (state == PlayerState::LOOKDOWN_RELEASE)
//        {
//            
//            currFrameInd.x++;
//
//            if (currFrameInd.x >= currFrameInfo.endFrame.x)
//            {
//                currFrameInd.x = currFrameInfo.endFrame.x;
//                state = PlayerState::IDLE;
//            }
//        }
//    }
//}
//
//void Character::UpdateLookDownMove(float TimeDelta)
//{
//    
//    if (dir.x != 0)
//    {
//        frameTime += TimeDelta;
//
//        if (frameTime >= ANIMATION_FRAME_TIME)
//        {
//            frameTime = 0.f;
//
//            currFrameInd.x++;
//            if (currFrameInd.x >= currFrameInfo.endFrame.x)
//                currFrameInd.x = currFrameInfo.startFrame.x;
//        }
//    }
//   
// 
//  
//}
//
//
//void Character::UpdateLookDownIdle(float TimeDelta)
//{
//    
//    if (dir.x != 0) 
//    {
//        state = PlayerState::LOOKDOWN_MOVE;
//    }
//    else
//        currFrameInd = currFrameInfo.startFrame;
//}
//
//
