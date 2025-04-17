#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"
#include "TimerManager.h"

void IdleState::Enter(Character* character)
{
    this->character = character;
 
    if (character->GetIsLookDownLocked()) ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
    else if (character->GetIsLookUpLocked()) ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    else ChangeSubState(SubState::IDLE_ALONE);
}

void IdleState::Update()
{
    KeyManager* km = KeyManager::GetInstance();

    bool isLeft = km->IsStayKeyDown(VK_LEFT);
    bool isRight = km->IsStayKeyDown(VK_RIGHT);
    bool isUp = km->IsStayKeyDown(VK_UP);
    bool isDown = km->IsStayKeyDown(VK_DOWN);
    bool isUpReleased = km->IsOnceKeyUp(VK_UP);
    bool isDownReleased = km->IsOnceKeyUp(VK_DOWN);

    if (isLeft || isRight)
    {
        character->ChangeState(&Character::moveState);
        character->SetIsLookUpLocked(false);
        character->SetIsLookDownLocked(false);
        return;
    }

    if (isUp)
    {
        if (!character->GetIsLookUpLocked())
        {
            if (currentSubState != SubState::IDLE_LOOKUP_START)
                ChangeSubState(SubState::IDLE_LOOKUP_START);

            if (character->GetCurrAnimEnd())
                character->SetIsLookUpLocked(true);
        }
        else ChangeSubState(SubState::IDLE_LOOKUP_STOP);
   
    }
    else if (isUpReleased)
    {
        character->SetIsLookUpLocked(false);
        ChangeSubState(SubState::IDLE_LOOKUP_RELEASE);
    }
    else if (isDown)
    {
        if (!character->GetIsLookDownLocked())
        {
            if (currentSubState != SubState::IDLE_LOOKDOWN_START)
                ChangeSubState(SubState::IDLE_LOOKDOWN_START);

            if (character->GetCurrAnimEnd())
                character->SetIsLookDownLocked(true);
        }
        else ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
        
    }
    else if (isDownReleased)
    {
        ChangeSubState(SubState::IDLE_LOOKDOWN_RELEASE);
        character->SetIsLookDownLocked(false);
    }

    if ((currentSubState == SubState::IDLE_LOOKDOWN_RELEASE ||
        currentSubState == SubState::IDLE_LOOKUP_RELEASE) &&
        character->GetCurrFrameInd().x >= character->GetCurrFrameInfo().endFrame.x)
    {
        ChangeSubState(SubState::IDLE_ALONE);
    }

    UpdateAnimation();
}

void IdleState::UpdateAnimation()
{
    character->PlayAnimation();
}

void IdleState::ChangeSubState( SubState newSubState)
{
    if (newSubState == SubState::NONE)
    {
        OutputDebugStringA("[Warning] Attempted to change to SubState::NONE!\n");
        return;
    }
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(IDLESTATE, static_cast<unsigned int>(newSubState));
}

void IdleState::Exit()
{
    character->SetFrameTime(0.0f);
}

const char* IdleState::GetSubStateName() const
{
    switch (currentSubState)
    {
    case SubState::IDLE_ALONE:            return "IDLE_ALONE"; break;
    case SubState::IDLE_LOOKUP_START:     return "IDLE_LOOKUP_START"; break;
    case SubState::IDLE_LOOKUP_STOP:      return "IDLE_LOOKUP_STOP"; break;
    case SubState::IDLE_LOOKUP_RELEASE:   return "IDLE_LOOKUP_RELEASE"; break;
    case SubState::IDLE_LOOKDOWN_START:   return "IDLE_LOOKDOWN_START"; break;
    case SubState::IDLE_LOOKDOWN_STOP:    return "IDLE_LOOKDOWN_STOP"; break;
    case SubState::IDLE_LOOKDOWN_RELEASE: return "IDLE_LOOKDOWN_RELEASE"; break;
    case SubState::IDLE_ONPET:            return "IDLE_ONPET"; break;
    case SubState::IDLE_ONPET_LOOKUP:     return "IDLE_ONPET_LOOKUP"; break;
    case SubState::IDLE_ONPET_LOOKDOWN:   return "IDLE_ONPET_LOOKDOWN"; break;
    case SubState::IDLE_FALL_ALMOST:      return "IDLE_FALL_ALMOST"; break;
    case SubState::IDLE_HURT:             return "IDLE_HURT"; break;
    case SubState::IDLE_DIE:              return "IDLE_DIE"; break;
    case SubState::NONE:                  
    default:                              return "NONE"; break;
    }
}

