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

    if (km->IsStayKeyDown(VK_RIGHT) ||
        km->IsStayKeyDown(VK_LEFT) 
        )
    {
        character->ChangeState(&Character::moveState);
        return;
    }

    if (km->IsStayKeyDown(VK_UP))
    {

        //���ǿ� ���� ���� �б� ex ) ��
        currentSubState = IdleState::SubState::IDLE_LOOKUP;
        
        
        character->LookUp(TimeDelta);
    }
    else if (km->IsStayKeyDown(VK_DOWN))
    {
        currentSubState = IdleState::SubState::IDLE_LOOKDOWN;
        character->LookDown(TimeDelta);
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
