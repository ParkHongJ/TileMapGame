#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void AttackState::Update(Character* character, float deltaTime)
{
    // 공격 중 입력이나 상태 변화 감지 가능
    UpdateAnimation(character);
}

void AttackState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(2, static_cast<int>(currentSubState)); // stateId = 3 (Attack)
}

void AttackState::Exit(Character* character)
{
    character->SetFrameTime(0.0f);
}
