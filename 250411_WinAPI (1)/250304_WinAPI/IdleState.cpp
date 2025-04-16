#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
		UpdateAnimation(character);
}

void IdleState::Update(Character* character, float TimeDelta)
{

    // �Է� ���� �޾Ƽ� ������� ����
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

        //���ǿ� ���� ���� �б�  / �� ���� ���� ����
        
        if (!character->GetIsLookUpPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_START;
            character->LookUp(TimeDelta);
        }
        else
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_STOP;
            // ������ �˻� �Լ�
        }
        

    }
    else if (km->IsOnceKeyUp(VK_UP))
    {
        if (character->GetIsLookUpPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKUP_RELEASE;
            //������ �˻� �Լ�
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
            // ������ �˻� �Լ�
        }

    }
    else if (km->IsOnceKeyUp(VK_DOWN))
    {
        if (character->GetIsLookDownPaused())
        {
            currentSubState = IdleState::SubState::IDLE_LOOKDOWN_RELEASE;
            //������ �˻� �Լ�
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
