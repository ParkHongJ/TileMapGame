#pragma once
#include "config.h"

class Character;

class CharacterState {
public:
    virtual void Enter(Character* character) = 0;
    virtual void Update() = 0;
    virtual void Exit() = 0;
    virtual const char* GetSubStateName() const = 0;

  
    virtual ~CharacterState() {};
};

