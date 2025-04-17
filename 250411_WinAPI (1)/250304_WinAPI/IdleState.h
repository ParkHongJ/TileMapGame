#pragma once
#include "CharacterState.h"
class Character;

class IdleState : public CharacterState
{
public:
    enum class SubState {
        NONE,
        IDLE_ALONE,
        IDLE_LOOKUP_START,
        IDLE_LOOKUP_STOP,
        IDLE_LOOKUP_RELEASE,
        IDLE_LOOKDOWN_START,
        IDLE_LOOKDOWN_STOP,
        IDLE_LOOKDOWN_RELEASE,
        IDLE_ONPET,
        IDLE_ONPET_LOOKUP,
        IDLE_ONPET_LOOKDOWN,
        IDLE_FALL_ALMOST,
        IDLE_HURT,
        IDLE_DIE
    };
private:
    SubState currentSubState;
    Character* character;
	
public:
    IdleState(SubState initialSubState = SubState::NONE)
        : currentSubState(initialSubState) {
    }

    virtual void Enter(Character* character) override;
    

    virtual void Update() override;

  
    void ChangeSubState(SubState newSubState);

    virtual void Exit() override;



    virtual const char* GetSubStateName() const override;



};

