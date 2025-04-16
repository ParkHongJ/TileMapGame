#include "InteractionState.h"
#include "Character.h"

void InteractionState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void InteractionState::Update(Character* character, float deltaTime)
{
    // ���� ���� ������ �ִٸ� ���⿡
    UpdateAnimation(character);
}

void InteractionState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(INTERACTIONSTATE, static_cast<int>(currentSubState)); // stateId = 2 (Interaction)
}

void InteractionState::Exit(Character* character)
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
}
