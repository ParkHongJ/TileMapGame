#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
		UpdateAnimation(character);
}

void IdleState::Update(Character* character, float deltaTime)
{

    // �Է� ���� �޾Ƽ� ������� ����
    // ����: �ü��� �Ʒ��� ������� ����
    KeyManager* km = KeyManager::GetInstance();

    if (km->IsStayKeyDown(VK_RIGHT) ||
        km->IsStayKeyDown(VK_LEFT) ||
        km->IsStayKeyDown(VK_UP) ||
        km->IsStayKeyDown(VK_DOWN) 
        )
    {
        character->ChageState();
    }
        
        
        



        UpdateAnimation(character);
    
}

void IdleState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(0,static_cast<int>(currentSubState));
}

void IdleState::Exit(Character* character)
{
    // 1. �ִϸ��̼� ����
    //character->
    character->SetFrameTime(0.0f);

    // 2. ���� ����
    //character->

    // 4. ���� ���� �ʱ�ȭ
    //currentSubState = SubState::IDLE_ALONE;
}
