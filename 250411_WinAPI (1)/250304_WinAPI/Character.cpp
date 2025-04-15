#include "Character.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT Character::Init()
{
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");


    SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });

    state = PlayerState::IDLE;
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
    PlayerState prevState = state;

    HandleInput(prevState, TimeDelta);

    if (prevState != state)
    {
 
        bool skipReset = ShouldResetAnimation(prevState, state);

        if (!skipReset)
        {
          
            SetAnimationRange(state);

            frameTime = 0.0f;
            currFrameInd = currFrameInfo.startFrame;
        }
    }


    switch (state)
    {
        case PlayerState::IDLE:                 UpdateIdle();                         break;
        case PlayerState::MOVE:                 UpdateMove(TimeDelta);                break;
        case PlayerState::LOOKUP_START:         UpdateLookUp(TimeDelta);              break;
        case PlayerState::LOOKUP_RELEASE:       UpdateLookUp(TimeDelta);              break;
        case PlayerState::LOOKDOWN_START:       UpdateLookDown(TimeDelta);            break;
        case PlayerState::LOOKDOWN_RELEASE:     UpdateLookDown(TimeDelta);            break;
        case PlayerState::LOOKDOWN_MOVE:        UpdateLookDownMove(TimeDelta);        break;
        case PlayerState::LOOKDOWN_IDLE:        UpdateLookDownMoveStop(TimeDelta);    break;
        case PlayerState::ATTACK:               UpdateAttack(TimeDelta);              break;
        case PlayerState::HANG:                 UpdateHang(TimeDelta);                break;
        case PlayerState::HURT:                 UpdateHurt(TimeDelta);                break;
        case PlayerState::DIE:                  UpdateDie(TimeDelta);                 break;
        case PlayerState::THROW:                UpdateThrow(TimeDelta);               break;
        case PlayerState::HOLD:                 UpdateHold(TimeDelta);                break;
        case PlayerState::PUSH:                 UpdatePush(TimeDelta);                break;
        case PlayerState::EXIT:                 UpdateExit(TimeDelta);                break;
        default: break;
    }
}

void Character::SetAnimationRange(PlayerState state)
{
    switch (state)
    {
        case PlayerState::IDLE:                  currFrameInfo = { {0, 0},  {0, 0} };            break;
        case PlayerState::MOVE:                  currFrameInfo = { {1, 0},  {9, 0} };            break;
        case PlayerState::LOOKUP_START:
        case PlayerState::LOOKUP_RELEASE:        currFrameInfo = { {0, 8},  {6, 8} };            break;
        case PlayerState::LOOKUP_ONTAMEDPET:     currFrameInfo = { {8, 8},  {14, 8} };           break;
        case PlayerState::LOOKDOWN_START:
        case PlayerState::LOOKDOWN_RELEASE:      currFrameInfo = { {0, 1},  {4, 1} };            break;
        case PlayerState::LOOKDOWN_MOVE:         currFrameInfo = { {5, 1},  {11, 1}};            break;
        case PlayerState::LOOKDOWN_IDLE:         currFrameInfo = { {2, 1},  {2, 1} };            break;
        case PlayerState::CLIMB_LADDER:          currFrameInfo = { {0, 6},  {5, 7} };            break;
        case PlayerState::CLIMB_ROPE:            currFrameInfo = { {0, 7},  {9, 7} };            break;
        case PlayerState::ON_NOTTAMEDPET:        currFrameInfo = { {4, 11}, {9, 11}};            break;
        case PlayerState::ON_TAMEDPET:           currFrameInfo = { {7, 8},  {7, 8} };            break;
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

bool Character::ShouldResetAnimation(PlayerState prevState, PlayerState newState)
{
    if ((prevState == PlayerState::LOOKUP_START && state == PlayerState::LOOKUP_RELEASE) ||
        (prevState == PlayerState::LOOKDOWN_START && state == PlayerState::LOOKDOWN_RELEASE))
        return true;
    else return false;

}


void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{
    /*{
        TCHAR buf[256];
        wsprintf(buf, TEXT("Render Frame: %d, %d"), currFrameInd.x, currFrameInd.y);
        TextOut(hdc, 20, 100, buf, wcslen(buf));
    }
    {
        TCHAR buf[256]; 
        wsprintf(buf, TEXT("PlayerState: %s"), PlayerStateToString(state));
        TextOut(hdc, 20, 120, buf, wcslen(buf));
    }*/

    //playerImage->FrameRender(renderTarget, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y);//, isFlip, true);
    playerImage->Render(renderTarget, Pos.x, Pos.y);
}

void Character::HandleInput(PlayerState prevState, float TimeDelta)
{
    KeyManager* km = KeyManager::GetInstance();

    bool hasInput = false;
    dir = { 0.0f , 0.0f };

#pragma region MOVESECTION

    if (km->IsStayKeyDown(VK_RIGHT))
    {
        dir.x += 1;
        isFlip = false;

        if (km->IsStayKeyDown(VK_DOWN))
        {
            state = PlayerState::LOOKDOWN_MOVE;
            speed = 100.f;
        }
        else
        {
            state = PlayerState::MOVE;
            speed = 200.f;
        }

        Pos.x += speed * dir.x * TimeDelta;
        hasInput = true;
    }
    else if (km->IsStayKeyDown(VK_LEFT))
    {
        dir.x += -1;
        isFlip = true;

        if (km->IsStayKeyDown(VK_DOWN))
        {
            state = PlayerState::LOOKDOWN_MOVE;
            speed = 100.f;
        }
        else
        {
            state = PlayerState::MOVE;
            speed = 200.f;
        }

        Pos.x += speed * dir.x * TimeDelta;
        hasInput = true;
    }
    
    if ((km->IsOnceKeyUp(VK_LEFT) || km->IsOnceKeyUp(VK_RIGHT)) && km->IsStayKeyDown(VK_DOWN))
    {
        switch (prevState)
        {
        case PlayerState::LOOKDOWN_MOVE:
            state = PlayerState::LOOKDOWN_IDLE;
            break;
        }
      //  return;
    }

    if (km->IsStayKeyDown(VK_DOWN) && state == PlayerState::LOOKDOWN_IDLE)
    {
        
    }
  


#pragma endregion

#pragma region JUMP

    if (km->IsOnceKeyDown(VK_SPACE))
    {
        jumpPressed = true;
        hasInput = true;
    }

#pragma endregion

#pragma region LOOKUP & LOOKDOWN

    if (state != PlayerState::MOVE)
    {
        // LOOK UP
        if (km->IsStayKeyDown(VK_UP))
        {
            if (state != PlayerState::LOOKUP_START &&
                state != PlayerState::LOOKUP_RELEASE)
            {
                state = PlayerState::LOOKUP_START;
                hasInput = true;
            }
        }

        if (state == PlayerState::LOOKUP_START && km->IsOnceKeyUp(VK_UP))
        {
            state = PlayerState::LOOKUP_RELEASE;
            isLookUpPaused = false;
        }

        // LOOK DOWN
        if (km->IsStayKeyDown(VK_DOWN))
        {
            if (state != PlayerState::LOOKDOWN_START &&
                state != PlayerState::LOOKDOWN_RELEASE &&
                state != PlayerState::LOOKDOWN_MOVE)
            {
                state = PlayerState ::LOOKDOWN_START;
                hasInput = true;
            }
        }

        if (km->IsOnceKeyUp(VK_DOWN))
        {
            switch (prevState)
            {
            case PlayerState::LOOKDOWN_MOVE:
            case PlayerState::LOOKDOWN_IDLE:
                state = PlayerState::LOOKDOWN_RELEASE;
                break;
            }
        }

        if (state == PlayerState::LOOKDOWN_START && km->IsOnceKeyUp(VK_DOWN))
        {
            state = PlayerState::LOOKDOWN_RELEASE;
            isLookDownPaused = false;
        }
    }

#pragma endregion

    if (!hasInput &&
        state != PlayerState::LOOKUP_START &&
        state != PlayerState::LOOKUP_RELEASE &&
        state != PlayerState::LOOKDOWN_START &&
        state != PlayerState::LOOKDOWN_RELEASE &&
        state != PlayerState::LOOKDOWN_MOVE)
    {
        state = PlayerState::IDLE;
    }
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

void Character::UpdateIdle()
{
    currFrameInd.x = 0;
    currFrameInd.y = 0;
}

void Character::UpdateMove(float TimeDelta)
{
    isLookUpPaused = false;
    isLookDownPaused = false;

    if (dir.x != 0)
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
    
}

void Character::UpdateLookUp(float TimeDelta)
{
    frameTime += TimeDelta;

    if (frameTime >= ANIMATION_FRAME_TIME)
    {
        frameTime = 0.f;

        if (state == PlayerState::LOOKUP_START)
        {
            if (!isLookUpPaused)
                currFrameInd.x++;

             if (currFrameInd.x == currFrameInfo.endFrame.x / 2)
                 isLookUpPaused = true;
        }

        if (state == PlayerState::LOOKUP_RELEASE)
        {
            currFrameInd.x++;

            if (currFrameInd.x >= currFrameInfo.endFrame.x)
            {
                currFrameInd.x = currFrameInfo.endFrame.x;
                state = PlayerState::IDLE;
            }
        }
    }
}

void Character::UpdateLookDown(float TimeDelta)
{
    frameTime += TimeDelta;


    if (frameTime >= ANIMATION_FRAME_TIME)
    {
        frameTime = 0.f;

        if (state == PlayerState::LOOKDOWN_START)
        {
            if (!isLookDownPaused)
                currFrameInd.x++;

            if (currFrameInd.x == currFrameInfo.endFrame.x / 2)
                isLookDownPaused = true;
        }

        if (state == PlayerState::LOOKDOWN_RELEASE)
        {
            
            currFrameInd.x++;

            if (currFrameInd.x >= currFrameInfo.endFrame.x)
            {
                currFrameInd.x = currFrameInfo.endFrame.x;
                state = PlayerState::IDLE;
            }
        }
    }
}

void Character::UpdateLookDownMove(float TimeDelta)
{
    
    if (dir.x != 0)
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
   
 
  
}


void Character::UpdateLookDownMoveStop(float TimeDelta)
{
    
    if (dir.x != 0) 
    {
        state = PlayerState::LOOKDOWN_MOVE;
    }
    else
        currFrameInd = currFrameInfo.startFrame;
}


void Character::UpdateClimb(float TimeDelta)
{
}

void Character::UpdateAttack(float TimeDelta)
{
}


void Character::UpdateHang(float TimeDelta)
{
}

void Character::UpdateHurt(float TimeDelta)
{
}

void Character::UpdateDie(float TimeDelta)
{
}

void Character::UpdateThrow(float TimeDelta)
{
}

void Character::UpdateHold(float TimeDelta)
{
}

void Character::UpdatePush(float TimeDelta)
{
}

void Character::UpdateExit(float TimeDelta)
{   
}

// for debug
const TCHAR* Character::PlayerStateToString(PlayerState state)
{
    
    switch (state)
    {
        case PlayerState::IDLE:               return TEXT("IDLE");
        case PlayerState::MOVE:               return TEXT("MOVE");
        case PlayerState::LOOKUP_START:       return TEXT("LOOKUP_START");
        case PlayerState::LOOKUP_RELEASE:     return TEXT("LOOKUP_RELEASE");
        case PlayerState::LOOKDOWN_START:     return TEXT("LOOKDOWN_START");
        case PlayerState::LOOKDOWN_RELEASE:   return TEXT("LOOKDOWN_RELEASE");
        case PlayerState::LOOKDOWN_MOVE:      return TEXT("LOOKDOWN_MOVE");
        case PlayerState::LOOKDOWN_IDLE:      return TEXT("LOOKDOWN_MOVESTOP");
        case PlayerState::CLIMB:              return TEXT("CLIMB");
        case PlayerState::ATTACK:             return TEXT("ATTACK");
        case PlayerState::HANG:               return TEXT("HANG");
        case PlayerState::HURT:               return TEXT("HURT");
        case PlayerState::DIE:                return TEXT("DIE");
        case PlayerState::THROW:              return TEXT("THROW");
        case PlayerState::HOLD:               return TEXT("HOLD");
        case PlayerState::PUSH:               return TEXT("PUSH");
        case PlayerState::EXIT:               return TEXT("EXIT");
        default:                              return TEXT("UNKNOWN");
    }
}