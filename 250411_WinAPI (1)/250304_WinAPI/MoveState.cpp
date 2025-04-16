#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void MoveState::Update(Character* character, float TimeDelta)
{
    // ��: �Է� �޾� ������� ��ȯ ó�� ����
    // if (character->IsLookingDown()) { currentSubState = SubState::MOVE_LOOKDOWN; }

    KeyManager* km = KeyManager::GetInstance();


    if (!km->IsOnceKeyUp(VK_LEFT)    ||
        !km->IsOnceKeyUp(VK_RIGHT)   || 
        character->GetVelocitySize() <= 0)
    {
        character->ChangeState(&Character::idleState);
        return;
    }
    
    // ���߿� ���� �� == ���� ���

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

        // ������� �̵�

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
