#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"
#include "TimerManager.h"

void IdleState::Enter(Character* character)
{
    this->character = character;
 


    if (character->GetIsLookDownPaused()) ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
    else if (character->GetIsLookUpPaused()) ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    else ChangeSubState(SubState::IDLE_ALONE);


}

void IdleState::Update()
{
    // �Է� ���� �޾Ƽ� ������� ����
    KeyManager* km = KeyManager::GetInstance();

    if (km->IsStayKeyDown(VK_RIGHT) || km->IsStayKeyDown(VK_LEFT))
    {
        character->ChangeState(&Character::moveState);
        character->SetIsLookUpPaused(false);
        character->SetIsLookDownPaused(false);
       
        return;
    }


    if (km->IsStayKeyDown(VK_UP))
    {

        //���ǿ� ���� ���� �б�  / �� ���� ���� ����
        
        if (!character->GetIsLookUpPaused())
        {
            if (currentSubState != SubState::IDLE_LOOKUP_START)
                ChangeSubState(SubState::IDLE_LOOKUP_START);

            character->LookUp();

        }
        else
        {
            character->SetIsLookUpPaused(true);
            ChangeSubState(SubState::IDLE_LOOKUP_STOP);

        }
    }
    else if (km->IsOnceKeyUp(VK_UP))
    {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_RELEASE);
            character->SetIsLookUpPaused(false);

    }
    else if (km->IsStayKeyDown(VK_DOWN))
    {

        if (!character->GetIsLookDownPaused())
        {
            if (currentSubState != SubState::IDLE_LOOKDOWN_START)
                ChangeSubState(SubState::IDLE_LOOKDOWN_START);

            character->LookDown();
        }
        else
        {
            character->SetIsLookDownPaused(true);
            ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);

        }

    }
    else if (km->IsOnceKeyUp(VK_DOWN))
    {
            ChangeSubState(IdleState::SubState::IDLE_LOOKDOWN_RELEASE);
            character->SetIsLookDownPaused(false);
       
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
    //currentSubState = SubState::NONE;
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

