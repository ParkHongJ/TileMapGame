#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
		UpdateAnimation(character);
}

void IdleState::Update(Character* character, float TimeDelta)
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
            currentSubState = IdleState::SubState::IDLE_ALONE;
        }

        if (currentSubState == IdleState::SubState::IDLE_LOOKUP_RELEASE)
        {
            currentSubState = IdleState::SubState::IDLE_ALONE;
        }
    }


    if (km->IsStayKeyDown(VK_UP))
    {

        //조건에 따라 상태 분기  / 펫 아직 구현 안함
        
        if (!character->GetIsLookUpPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_START;
            character->LookUp(TimeDelta);
        }
        else
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_STOP;
            // 프레임 검사 함수
        }
        

    }
    else if (km->IsOnceKeyUp(VK_UP))
    {
        if (character->GetIsLookUpPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_RELEASE;
            //프레임 검사 함수
        }
    }
    else if (km->IsStayKeyDown(VK_DOWN))
    {

        if (!character->GetIsLookUpPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKDOWN_START;
            character->LookDown(TimeDelta);
        }
        else
        {
            currentSubState = IdleState::SubState::IDLE_LOOKDOWN_STOP;
            // 프레임 검사 함수
        }

    }
    else if (km->IsOnceKeyUp(VK_DOWN))
    {
        if (character->GetIsLookDownPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKDOWN_RELEASE;
            //프레임 검사 함수
        }
    }

    UpdateAnimation(character);
    
}

void IdleState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(IDLESTATE, static_cast<int>(currentSubState));
}

void IdleState::Exit(Character* character)
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
}
