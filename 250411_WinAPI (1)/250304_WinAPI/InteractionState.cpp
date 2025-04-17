#include "InteractionState.h"
#include "Character.h"

void InteractionState::Enter(Character* character)
{
    this->character = character;
}

void InteractionState::Update()
{
    // 상태 변경 로직이 있다면 여기에
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
