#pragma once
#include "CharacterState.h"
class Character;

class MoveState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        MOVE_ALONE,
        MOVE_LOOKDOWN_START,
        MOVE_LOOKDOWN_LOOP,
        MOVE_LOOKDOWN_RELEASE,
        MOVE_SLOW,
        MOVE_ONPET,
        MOVE_ONPET_LOOKDOWN,
        MOVE_ONAIR,
        MOVE_ONAIR_ONPET,
        MOVE_HANGON_AUTO,
        MOVE_ON_ROPE,
        MOVE_ON_LADDER
    };

private:
    SubState currentSubState;
    Character* character;

public:
    MoveState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    virtual void Update( ) override;
    void ChangeSubState(SubState newSubState);

    virtual void Exit() override;
    virtual const char* GetSubStateName() const override;
    SubState GetCurrentSubState() const { return currentSubState; }
};
