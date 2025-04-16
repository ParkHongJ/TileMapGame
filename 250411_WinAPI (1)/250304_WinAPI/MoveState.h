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
    Character* character;

public:
    MoveState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update(float TimeDelta) override;
    virtual void UpdateAnimation( float TimeDelta) override;
    void ChangeSubState(SubState newSubState);

    virtual void Exit() override;
    virtual const char* GetSubStateName() const override;
};
