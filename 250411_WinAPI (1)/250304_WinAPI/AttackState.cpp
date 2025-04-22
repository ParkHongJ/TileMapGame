#include "pch.h"
#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    this->character = character;
    if (character->GetIsAttacking()) ChangeSubState(SubState::ATTACK_WHIP);
}

void AttackState::Update()
{
    if (character->GetIsAttacking())
    {
        ChangeSubState(SubState::ATTACK_WHIP);
        return;
    }

}

void AttackState::ChangeSubState(SubState newSubState)
{

    currentSubState = newSubState;
    character->SetAnimationFrameInfo(ATTACKSTATE, static_cast<int>(newSubState));
}

void AttackState::Exit()
{
    character->SetFrameTime(0.0f);
}

const char* AttackState::GetSubStateName() const
{
    switch (currentSubState) {
    case SubState::ATTACK_WHIP:                 return "ATTACK_WHIP";
    case SubState::ATTACK_ITEM_THROW:     return "ATTACK_ITEM_THROW";

    }

}
