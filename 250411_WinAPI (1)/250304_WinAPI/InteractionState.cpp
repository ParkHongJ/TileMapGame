#include "InteractionState.h"
#include "Character.h"

void InteractionState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void InteractionState::Update(Character* character, float deltaTime)
{
    // 상태 변경 로직이 있다면 여기에
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
