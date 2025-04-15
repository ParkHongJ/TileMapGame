#pragma once
#include "CharacterState.h"
class Character;

class IdleState : public CharacterState
{
public:
    enum class SubState {
        IDLE_ALONE,
        IDLE_ONPET,
        IDLE_ONPET_LOOKDOWN,
        IDLE_ONPET_LOOKUP,
        IDLE_FALL_ALMOST,
        IDLE_HURT,
        IDLE_DIE
    };
private:
    SubState currentSubState;
	
public:
    IdleState(SubState initialSubState = SubState::IDLE_ALONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    

    virtual void Update(Character* character, float deltaTime) override;

    virtual void UpdateAnimation(Character* character) override;

    virtual void Exit(Character* character) override;


};

