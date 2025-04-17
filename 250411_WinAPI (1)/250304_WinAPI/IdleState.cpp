#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"
#include "TimerManager.h"

void IdleState::Enter(Character* character)
{
    this->character = character;
    //currentSubState = SubState::IDLE_ALONE;
    //character->SetAnimationFrameInfo(IDLESTATE, static_cast<int>(currentSubState));
    //character->SetFrameTime(0.f);


    if (character->GetIsLookDownPaused()) ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
    else if (character->GetIsLookUpPaused()) ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    else ChangeSubState(SubState::IDLE_ALONE);


}

void IdleState::Update()
{
    // 입력 등을 받아서 서브상태 변경
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

        //조건에 따라 상태 분기  / 펫 아직 구현 안함
        
        if (!character->GetIsLookUpPaused())
        {
            if (currentSubState != SubState::IDLE_LOOKUP_START)
                ChangeSubState(SubState::IDLE_LOOKUP_START);

            character->LookUp();

        }
        else
        {
            //if (currentSubState != SubState::IDLE_LOOKUP_STOP)
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
            //if (currentSubState != SubState::IDLE_LOOKDOWN_STOP)
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
    // 조건에 따라 엄청난 분기를 할 예정

    UpdateAnimation();
    
}

void IdleState::UpdateAnimation()
{
    character->PlayAnimation();
}

void IdleState::ChangeSubState( SubState newSubState)
{
    //if (currentSubState == newSubState) return;
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
    case SubState::IDLE_ALONE: return "IDLE_ALONE";
    case SubState::IDLE_LOOKUP_START: return "IDLE_LOOKUP_START";
    case SubState::IDLE_LOOKUP_STOP: return "IDLE_LOOKUP_STOP";
    case SubState::IDLE_LOOKUP_RELEASE: return "IDLE_LOOKUP_RELEASE";
    case SubState::IDLE_LOOKDOWN_START: return "IDLE_LOOKDOWN_START";
    case SubState::IDLE_LOOKDOWN_STOP: return "IDLE_LOOKDOWN_STOP";
    case SubState::IDLE_LOOKDOWN_RELEASE: return "IDLE_LOOKDOWN_RELEASE";
    case SubState::IDLE_ONPET: return "IDLE_ONPET";
    case SubState::IDLE_ONPET_LOOKUP: return "IDLE_ONPET_LOOKUP";
    case SubState::IDLE_ONPET_LOOKDOWN: return "IDLE_ONPET_LOOKDOWN";
    case SubState::IDLE_FALL_ALMOST: return "IDLE_FALL_ALMOST";
    case SubState::IDLE_HURT: return "IDLE_HURT";
    case SubState::IDLE_DIE: return "IDLE_DIE";
    case SubState::NONE: default: return "NONE";
    }
}

