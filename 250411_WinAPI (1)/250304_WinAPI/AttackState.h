#pragma once
#include "CharacterState.h"
class Character;

class AttackState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        ATTACK_WHIP,
        ATTACK_ITEM_THROW,
    };

private:
    SubState currentSubState;
    Character* character;

public:
    AttackState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update() override;
    void ChangeSubState(SubState newSubState);
    virtual void Exit() override;
    virtual const char* GetSubStateName() const override;
    SubState GetCurrentSubState() const { return currentSubState; }

};
