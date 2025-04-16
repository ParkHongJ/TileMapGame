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

public:
    AttackState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update(Character* character, float deltaTime) override;
    virtual void UpdateAnimation(Character* character) override;
    virtual void Exit(Character* character) override;
    virtual const char* GetStateName() const override { return "AttackState"; }
};
