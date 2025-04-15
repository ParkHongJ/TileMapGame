#pragma once
#include "config.h"

class Character;

class CharacterState {
public:
    virtual void Enter(Character* character) = 0;
    virtual void Update(Character* character, float deltaTime) = 0;
    virtual void UpdateAnimation(Character* character) = 0;
    virtual void Exit(Character* character) = 0;
    virtual ~CharacterState() {};
};

