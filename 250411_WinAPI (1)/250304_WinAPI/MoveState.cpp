#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void MoveState::Update(Character* character, float TimeDelta)
{
    // 예: 입력 받아 서브상태 전환 처리 가능
    // if (character->IsLookingDown()) { currentSubState = SubState::MOVE_LOOKDOWN; }

    KeyManager* km = KeyManager::GetInstance();


    if (!km->IsOnceKeyUp(VK_LEFT)    ||
        !km->IsOnceKeyUp(VK_RIGHT)   || 
        character->GetVelocitySize() <= 0)
    {
        character->ChangeState(&Character::idleState);
        return;
    }
    
    // 공중에 있을 시 == 점프 모션

    if (character->GetYVelocity() > 0)
    {
        currentSubState = SubState::MOVE_ONAIR;

        if (km->IsStayKeyDown(VK_LEFT)) character->Move(-1, TimeDelta);
        else if (km->IsStayKeyDown(VK_RIGHT)) character->Move(1, TimeDelta);
    }
    else
    {
        if (km->IsStayKeyDown(VK_LEFT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {

                currentSubState = SubState::MOVE_LOOKDOWN;
                character->Move(-1, TimeDelta);
            }
            else
            {
                currentSubState = SubState::MOVE_ALONE;
                character->Move(-1, TimeDelta);
            }
        }
        else if (km->IsStayKeyDown(VK_RIGHT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {

                currentSubState = SubState::MOVE_LOOKDOWN;
                character->Move(1, TimeDelta);
            }
            else
            {
                currentSubState = SubState::MOVE_ALONE;
                character->Move(1, TimeDelta);
            }
        }

        // 엎드려서 이동

        if (km->IsStayKeyDown(VK_DOWN))
        {
            if (km->IsStayKeyDown(VK_LEFT))
            {
                currentSubState = SubState::MOVE_LOOKDOWN;
                character->Move(-1, TimeDelta);
               
            }
            else if (km->IsStayKeyDown(VK_RIGHT))
            {
                currentSubState = SubState::MOVE_LOOKDOWN;
                character->Move(1, TimeDelta);
              
            }
        }
    }


    UpdateAnimation(character);
}

void MoveState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(MOVESTATE, static_cast<int>(currentSubState)); 
}

void MoveState::Exit(Character* character)
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
    
}
