#include "Character.h"
#include "ImageManager.h"
#include "Image.h"

HRESULT Character::Init()
{
    ImageManager::GetInstance()->AddImage("Tae_Player", TEXT("Image/Character/char_yellow.bmp"), 2048, 2048, 16, 16, true, RGB(255, 0, 255));
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");


    SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });

    state = PlayerState::IDLE;
    dir = { 0.0f, 0.0f };

    // causally walking & idle
    currFrameInd = { 0,0 };

    currFrameInfo.startFrame = { 0,0 };
    currFrameInfo.endFrame = { 0,0 };
    currMaxFrameInd = { 9,0 };
    frameTime = 0.0f;

    speed = 200.f;
    attackSpeed = 3.0f;
    attackRate = 0.3f;

    isFlip = false;
    isFalling = false;
    isAttakcing = false;

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

    HandleInput(TimeDelta);

    if (prevState != state)
    {
        frameTime = 0.0f;
        //currFrameInd = { 0,0 }; 
    }

    switch (state)
    {
        case PlayerState::IDLE:               UpdateIdle();               break;
        case PlayerState::MOVE:               UpdateMove(TimeDelta);      break;
        case PlayerState::LOOKUP_START:       UpdateLookUp(TimeDelta);    break;
        case PlayerState::LOOKUP_RELEASE:     UpdateLookUp(TimeDelta);    break;
        case PlayerState::LOOKDOWN_START:     UpdateLookDown(TimeDelta);  break;
        case PlayerState::LOOKDOWN_RELEASE:   UpdateLookDown(TimeDelta);  break;
        case PlayerState::JUMP:               UpdateJump(TimeDelta);      break;
        case PlayerState::FALL:               UpdateFall(TimeDelta);      break;
        case PlayerState::CLIMB:              UpdateClimb(TimeDelta);     break;
        case PlayerState::ATTACK:             UpdateAttack(TimeDelta);    break;
        case PlayerState::CROUCH:             UpdateCrouch(TimeDelta);    break;
        case PlayerState::HANG:               UpdateHang(TimeDelta);      break;
        case PlayerState::HURT:               UpdateHurt(TimeDelta);      break;
        case PlayerState::DIE:                UpdateDie(TimeDelta);       break;
        case PlayerState::THROW:              UpdateThrow(TimeDelta);     break;
        case PlayerState::HOLD:               UpdateHold(TimeDelta);      break;
        case PlayerState::PUSH:               UpdatePush(TimeDelta);      break;
        case PlayerState::EXIT:               UpdateExit(TimeDelta);      break;
        default: break;
    }
}

void Character::Render(HDC hdc)
{
    {
        TCHAR buf[256];
        wsprintf(buf, TEXT("Render Frame: %d, %d"), currFrameInd.x, currFrameInd.y);
        TextOut(hdc, 20, 100, buf, wcslen(buf));
    }
    {
        TCHAR buf[256]; 
        wsprintf(buf, TEXT("PlayerState: %s"), PlayerStateToString(state));
        TextOut(hdc, 20, 120, buf, wcslen(buf));
    }

    playerImage->FrameRender(hdc, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y, isFlip, true);

}

void Character::HandleInput(float TimeDelta)
{
    KeyManager* km = KeyManager::GetInstance();

    bool hasInput = false; // Ű �Է� ���� üũ
    dir = { 0.0f , 0.0f };

    if (km->IsStayKeyDown(VK_RIGHT))
    {
        state = PlayerState::MOVE;
        isFlip = false;
        dir.x += 1;
        Pos.x += speed * dir.x * TimeDelta;
        hasInput = true;
    }
    if (km->IsStayKeyDown(VK_LEFT))
    {
        state = PlayerState::MOVE;
        isFlip = true;
        dir.x += -1;
        Pos.x += speed * dir.x * TimeDelta;
        hasInput = true;
    }
    if (km->IsStayKeyDown(VK_UP))
    {
        if (state != PlayerState::LOOKUP_RELEASE)
        {
            state = PlayerState::LOOKUP_START;
            hasInput = true;
        }
    }
    if (state == PlayerState::LOOKUP_START && km->IsOnceKeyUp(VK_UP))
    {
        state = PlayerState::LOOKUP_RELEASE; 
        isLookUpPaused = false;
        return;
    }

    if (km->IsStayKeyDown(VK_DOWN))
    {
        state = PlayerState::LOOKDOWN_START;
        hasInput = true;
    }
    if (km->IsOnceKeyDown(VK_SPACE))
    {
        state = PlayerState::JUMP;
        jumpPressed = true;
        hasInput = true;
    }

    if (!hasInput && state != PlayerState::LOOKUP_START && state != PlayerState::LOOKUP_RELEASE)
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
    currFrameInfo.startFrame = { 1,0 };
    currFrameInfo.endFrame = { 9,0 };

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
    currFrameInfo.startFrame = { 0, 8 };
    currFrameInfo.endFrame = { 6, 8 };
    currFrameInd.y = currFrameInfo.startFrame.y;

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

         else if (state == PlayerState::LOOKUP_RELEASE)
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
}

void Character::UpdateJump(float TimeDelta)
{

}

void Character::UpdateFall(float TimeDelta)
{
}

void Character::UpdateClimb(float TimeDelta)
{
}

void Character::UpdateAttack(float TimeDelta)
{
}

void Character::UpdateCrouch(float TimeDelta)
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
        case PlayerState::JUMP:               return TEXT("JUMP");
        case PlayerState::FALL:               return TEXT("FALL");
        case PlayerState::CLIMB:              return TEXT("CLIMB");
        case PlayerState::ATTACK:             return TEXT("ATTACK");
        case PlayerState::CROUCH:             return TEXT("CROUCH");
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