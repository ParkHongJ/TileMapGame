#include "pch.h"
#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    this->character = character;
}

void AttackState::Update()
{

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
    case SubState::ATTACK_WHIP_START:        return "ATTACK_WHIP_START";
    case SubState::ATTACK_WHIP_END:           return "ATTACK_WHIP_END";
    case SubState::ATTACK_ITEM_THROW:     return "ATTACK_ITEM_THROW";

    }

}
