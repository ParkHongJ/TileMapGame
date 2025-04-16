#pragma once
#include "CharacterState.h"
class Character;

class MoveState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        MOVE_ALONE,
        MOVE_LOOKDOWN,
        MOVE_ONPET,
        MOVE_ONPET_LOOKDOWN,
        MOVE_ONAIR,
        MOVE_ONAIR_ONPET
    };

private:
    SubState currentSubState;

public:
    MoveState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update(Character* character, float TimeDelta) override;
    virtual void UpdateAnimation(Character* character) override;
    virtual void Exit(Character* character) override;
    virtual const char* GetStateName() const override { return "MoveState"; }
};
