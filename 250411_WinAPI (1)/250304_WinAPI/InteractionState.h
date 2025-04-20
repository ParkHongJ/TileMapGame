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
        INTERACTION_HANGON_TILE,
        INTERACTION_ENTER_TUNNEL,
        INTERACTION_EXIT_TUNNEL,
        INTERACTION_EXIT_LEVEL,
        INTERACTION_HOLD_ITEM,
        INTERACTION_PUSH_TILE
    };

private:
    SubState currentSubState;
    Character* character;

public:
    InteractionState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update() override;
    void ChangeSubState(SubState newSubState);
    virtual void Exit() override;
    virtual const char* GetSubStateName() const override;
    SubState GetCurrentSubState() const { return currentSubState; }
};
