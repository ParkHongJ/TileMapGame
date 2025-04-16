#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
    this->character = character;
}

void IdleState::Update(float TimeDelta)
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
            ChangeSubState(IdleState::SubState::IDLE_ALONE);
        }

        if (currentSubState == IdleState::SubState::IDLE_LOOKUP_RELEASE)
        {
            ChangeSubState(IdleState::SubState::IDLE_ALONE);
        }
    }


    if (km->IsStayKeyDown(VK_UP))
    {

        //���ǿ� ���� ���� �б�  / �� ���� ���� ����
        
        if (!character->GetIsLookUpPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_START);
            character->LookUp(TimeDelta);
        }
        else
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_STOP);

            // ������ �˻� �Լ�
        }
        

    }
    else if (km->IsOnceKeyUp(VK_UP))
    {
        if (character->GetIsLookUpPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKUP_RELEASE);

            //������ �˻� �Լ�
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
            
            // ������ �˻� �Լ�
        }

    }
    else if (km->IsOnceKeyUp(VK_DOWN))
    {
        if (character->GetIsLookDownPaused())
        {
            ChangeSubState(IdleState::SubState::IDLE_LOOKDOWN_RELEASE);

            //������ �˻� �Լ�
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
