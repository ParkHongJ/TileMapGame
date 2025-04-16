#pragma once
#include "CharacterState.h"
class Character;

class InteractionState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        INTERACTION_CLIMB_LADDER,
        INTERACTION_CLIMB_ROPE,
        INTERACTION_ENTER_TUNNEL,
        INTERACTION_EXIT_TUNNEL,
        INTERACTION_EXIT_LEVEL,
        INTERACTION_HOLD_ITEM,
        INTERACTION_PUSH_ITEM
    };

private:
    SubState currentSubState;

public:
    InteractionState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update(Character* character, float deltaTime) override;
    virtual void UpdateAnimation(Character* character) override;
    virtual void Exit(Character* character) override;
    virtual const char* GetStateName() const override { return "InteractionState"; }

};
