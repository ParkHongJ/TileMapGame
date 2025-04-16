#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    this->character = character;

    Update(TimerManager::GetInstance()->GetDeltaTime(L"60Frame"));
}

void MoveState::Update(float TimeDelta)
{
    // 예: 입력 받아 서브상태 전환 처리 가능
    // if (character->IsLookingDown()) { currentSubState = SubState::MOVE_LOOKDOWN; }

    KeyManager* km = KeyManager::GetInstance();


    if (km->IsOnceKeyUp(VK_LEFT)    ||
        km->IsOnceKeyUp(VK_RIGHT)   )
    {
        character->ChangeState(&Character::idleState);
        return;
    }
    
    // 공중에 있을 시 == 점프 모션

    if (character->GetYVelocity() > 0)
    {
        ChangeSubState(SubState::MOVE_ONAIR);

        if (km->IsStayKeyDown(VK_LEFT)) character->Move(-1, TimeDelta);
        else if (km->IsStayKeyDown(VK_RIGHT)) character->Move(1, TimeDelta);
    }
    else
    {
        if (km->IsStayKeyDown(VK_LEFT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN);

                 character->Move(-1, TimeDelta);
            }
            else
            {
                ChangeSubState(SubState::MOVE_ALONE);

                character->Move(-1, TimeDelta);
            }
        }
        else if (km->IsStayKeyDown(VK_RIGHT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN);

                character->Move(1, TimeDelta);
            }
            else
            {
                ChangeSubState(SubState::MOVE_ALONE);

                 character->Move(1, TimeDelta);
            }
        }

        // 엎드려서 이동

        if (km->IsStayKeyDown(VK_DOWN))
        {
            if (km->IsStayKeyDown(VK_LEFT))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN);

                character->Move(-1, TimeDelta);
               
            }
            else if (km->IsStayKeyDown(VK_RIGHT))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN);

                character->Move(1, TimeDelta);
              
            }
        }
    }

    UpdateAnimation(TimeDelta);
}

void MoveState::UpdateAnimation(float TimeDelta)
{
    character->PlayAnimation(TimeDelta); 
}

void MoveState::ChangeSubState(SubState newSubState)
{
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(MOVESTATE, static_cast<int>(newSubState));
}

void MoveState::Exit()
{
    //currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
    
}

const char* MoveState::GetSubStateName() const
{
    switch (currentSubState)
    {
    case SubState::MOVE_ALONE: return "MOVE_ALONE";
    case SubState::MOVE_LOOKDOWN: return "MOVE_LOOKDOWN";
    case SubState::MOVE_ONPET: return "MOVE_ONPET";
    case SubState::MOVE_ONPET_LOOKDOWN: return "MOVE_ONPET_LOOKDOWN";
    case SubState::MOVE_ONAIR: return "MOVE_ONAIR";
    case SubState::MOVE_ONAIR_ONPET: return "MOVE_ONAIR_ONPET";
    case SubState::NONE: default: return "NONE";
    }
}
