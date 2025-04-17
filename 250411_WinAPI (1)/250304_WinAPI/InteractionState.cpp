#include "InteractionState.h"
#include "Character.h"

void InteractionState::Enter(Character* character)
{
    this->character = character;
}

void InteractionState::Update()
{
    // ���� ���� ������ �ִٸ� ���⿡
    UpdateAnimation();
}

void InteractionState::UpdateAnimation()
{
    character->PlayAnimation(); // stateId = 2 (Interaction)
}

void InteractionState::ChangeSubState(SubState newSubState)
{
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(INTERACTIONSTATE, static_cast<int>(newSubState));
}

void InteractionState::Exit()
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
}
