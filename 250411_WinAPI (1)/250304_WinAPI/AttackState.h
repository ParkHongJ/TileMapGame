#pragma once
#include "CharacterState.h"
class Character;

class AttackState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        ATTACK_ALONE,
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
    virtual void Update( float TimeDelta) override;
    virtual void UpdateAnimation( float TimeDelta) override;
    void ChangeSubState(SubState newSubState);
    virtual void Exit() override;
    virtual const char* GetStateName() const override { return "AttackState"; }
};
