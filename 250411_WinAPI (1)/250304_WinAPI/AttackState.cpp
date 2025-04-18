#include "pch.h"
#include "AttackState.h"
#include "Character.h"

void AttackState::Enter(Character* character)
{
    this->character = character;
    ChangeSubState(SubState::ATTACK_WHIP);
    character->SetIsAttacking(true);
}

void AttackState::Update()
{
    KeyManager* km = KeyManager::GetInstance();

    // 점프 입력은 공격 중에도 허용
    if (km->IsOnceKeyDown('Z') && !character->GetIsInAir())
    {
        character->SetYVelocity(-character->GetJumpPower());
        character->SetIsInAir(true);
      
    }


    // 아이템에 따라 분기

    //switch (character->GetCurrItem())
    //{

    //}

    switch (currentSubState)
    {
    case SubState::ATTACK_WHIP:
    case SubState::ATTACK_ITEM_THROW:
    {
        
        if (character->GetCurrAnimEnd()) {
            character->SetIsAttacking(false);

            if (character->GetIsInAir())
                character->SetAnimationFrameInfo(ANIMSTATE,
                    character->GetYVelocity() < 0
                    ? static_cast<int>(SubAnim::JUMP_UP)
                    : static_cast<int>(SubAnim::JUMP_DOWN));
            else
                character->ChangeState(&Character::idleState);
        }
        break;
    }

    default:
        break;
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
    character->SetIsAttacking(false);
}

const char* AttackState::GetSubStateName() const
{
    switch (currentSubState) {
    case SubState::ATTACK_WHIP:        return "ATTACK_WHIP";
    case SubState::ATTACK_ITEM_THROW:     return "ATTACK_ITEM_THROW";

    }

}
