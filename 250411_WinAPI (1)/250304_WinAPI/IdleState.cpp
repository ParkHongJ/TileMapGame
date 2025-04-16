#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
    this->character = character;
}

void IdleState::Update(float TimeDelta)
{
    // 입력 등을 받아서 서브상태 변경
    KeyManager* km = KeyManager::GetInstance();

    if (km->IsStayKeyDown(VK_RIGHT) || km->IsStayKeyDown(VK_LEFT))
    {
        character->ChangeState(&Character::moveState);
        return;
    }

    if (!character->GetIsLookUpPaused())
    {
        if (currentSubState == IdleState::SubState::IDLE_LOOKDOWN_RELEASE)
        {
            ChangeSubState(IdleState::SubState::IDLE_ALONE);
        }

        if (currentSubState == IdleState::SubState::IDLE_LOOKUP_RELEASE)
        {
            ChangeSubState(IdleState::SubState::IDLE_ALONE);
        }
    }


    if (km->IsStayKeyDown(VK_UP))
    {

        //조건에 따라 상태 분기  / 펫 아직 구현 안함
        
        if (!character->GetIsLookUpPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_START);
            character->LookUp(TimeDelta);
        }
        else
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_STOP);

            // 프레임 검사 함수
        }
        

    }
    else if (km->IsOnceKeyUp(VK_UP))
    {
        if (character->GetIsLookUpPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_RELEASE);

            //프레임 검사 함수
        }
    }
    else if (km->IsStayKeyDown(VK_DOWN))
    {

        if (!character->GetIsLookUpPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKDOWN_START);
            character->LookDown(TimeDelta);
        }
        else
        {

            ChangeSubState( IdleState::SubState::IDLE_LOOKDOWN_STOP);
            
            // 프레임 검사 함수
        }

    }
    else if (km->IsOnceKeyUp(VK_DOWN))
    {
        if (character->GetIsLookDownPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKDOWN_RELEASE);

            //프레임 검사 함수
        }
    }

    UpdateAnimation(TimeDelta);
    
}

void IdleState::UpdateAnimation(float TimeDelta)
{
    character->PlayAnimation(TimeDelta);
}

void IdleState::ChangeSubState( SubState newSubState)
{
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(IDLESTATE, static_cast<unsigned int>(newSubState));
}

void IdleState::Exit()
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
}
