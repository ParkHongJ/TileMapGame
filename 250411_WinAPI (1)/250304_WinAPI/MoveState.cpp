#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    UpdateAnimation(character);
}

void MoveState::Update(Character* character, float deltaTime)
{
    // 예: 입력 받아 서브상태 전환 처리 가능
    // if (character->IsLookingDown()) { currentSubState = SubState::MOVE_LOOKDOWN; }

    UpdateAnimation(character);
}

void MoveState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(1, static_cast<int>(currentSubState)); 
}

void MoveState::Exit(Character* character)
{
    character->SetFrameTime(0.0f);
}
