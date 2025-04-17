#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    this->character = character;
}

void AttackState::Update()
{
    // ���� �� �Է��̳� ���� ��ȭ ���� ����
    UpdateAnimation();
}

void AttackState::UpdateAnimation()
{
    character->PlayAnimation(); 
}

void AttackState::ChangeSubState(SubState newSubState)
{
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(ATTACKSTATE, static_cast<int>(newSubState));
}

void AttackState::Exit()
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
}
