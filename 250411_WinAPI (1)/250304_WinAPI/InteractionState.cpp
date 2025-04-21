#include "pch.h"
#include "InteractionState.h"
#include "Character.h"

void InteractionState::Enter(Character* character)
{
    this->character = character;
    character->CheckTileCollision();
    Update();
}

void InteractionState::Update()
{

    if (character->CheckHangOn())
    {
        ChangeSubState(SubState::INTERACTION_HANGON_TILE);
    }
    else if (character->CheckCanClimbLadder())
    {
        ChangeSubState(SubState::INTERACTION_CLIMB_LADDER);
    }
    else if (character->CheckCanClimbRope())
    {
        ChangeSubState(SubState::INTERACTION_CLIMB_ROPE);
    }
    else if (character->CheckCanPushTile())
    {
        ChangeSubState(SubState::INTERACTION_PUSH_TILE);
    }
    



}

void InteractionState::ChangeSubState(SubState newSubState)
{
    //if (currentSubState == newSubState) return;
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(INTERACTIONSTATE, static_cast<int>(newSubState));
}

void InteractionState::Exit()
{

    character->SetFrameTime(0.0f);
}

const char* InteractionState::GetSubStateName() const {
    switch (currentSubState) {
    case SubState::NONE:                     return "NONE";
    case SubState::INTERACTION_CLIMB_LADDER: return "INTERACTION_CLIMB_LADDER";
    case SubState::INTERACTION_CLIMB_ROPE:   return "INTERACTION_CLIMB_ROPE";
    case SubState::INTERACTION_ENTER_TUNNEL: return "INTERACTION_ENTER_TUNNEL";
    case SubState::INTERACTION_EXIT_TUNNEL:  return "INTERACTION_EXIT_TUNNEL";
    case SubState::INTERACTION_EXIT_LEVEL:   return "INTERACTION_EXIT_LEVEL";
    case SubState::INTERACTION_HOLD_ITEM:    return "INTERACTION_HOLD_ITEM";
    case SubState::INTERACTION_PUSH_TILE:    return "INTERACTION_PUSH_ITEM";
    case SubState::INTERACTION_HANGON_TILE:    return "INTERACTION_HANGON_TILE";
    }
}
