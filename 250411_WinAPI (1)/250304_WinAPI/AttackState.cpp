#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    this->character = character;
    Update(TimerManager::GetInstance()->GetDeltaTime(L"60Frame"));
}

void AttackState::Update(float TimeDelta)
{
    // 공격 중 입력이나 상태 변화 감지 가능
    UpdateAnimation(TimeDelta);
}

void AttackState::UpdateAnimation(float TimeDelta)
{
    character->PlayAnimation(TimeDelta); 
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
