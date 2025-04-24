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

    if (character->GetIsDead())
    {
        ChangeSubState(SubState::INTERACTION_IS_DEAD);
        return;
    }
    if (character->GetOnNeedle())
    {
        ChangeSubState(SubState::INTERACTION_ON_NIDDLE);
    }
    else if (character->GetIsFaint())
    {
        ChangeSubState(SubState::INTERACTION_ON_DAMAGE);
        return;
    }
    else if (character->GetIsEnteringGate())
    {

        ChangeSubState(SubState::INTERACTION_ENTER_TUNNEL);
        return;
    }
    else if (character->GetIsExitingGate())
    {
        ChangeSubState(SubState::INTERACTION_EXIT_TUNNEL);
        return;
    }
    else if (character->CheckHangOn())
    {
        ChangeSubState(SubState::INTERACTION_HANGON_TILE);
        return;
    }
    else if (character->CheckCanClimbLadder())
    {
        ChangeSubState(SubState::INTERACTION_CLIMB_LADDER);
        return;
    }
    else if (character->CheckCanClimbRope())
    {
        ChangeSubState(SubState::INTERACTION_CLIMB_ROPE);
        return;
    }
    else if (character->CheckCanPushTile())
    {
        ChangeSubState(SubState::INTERACTION_PUSH_TILE);
        return;
    }
    /*else if (character->)
    {


        return;
    }
    */



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
    case SubState::INTERACTION_ON_NIDDLE: return "INTERACTION_ON_NIDDLE";
    case SubState::INTERACTION_ON_DAMAGE: return "INTERACTION_ON_DAMAGE";
    case SubState::INTERACTION_IS_DEAD: return "INTERACTION_IS_DEAD";
    }
}
