#include "pch.h"
#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    this->character = character;
 

}

void MoveState::Update() {
    InputIntent input = character->GetCurrInputIntent();
    

    if (character->GetIsMovingAuto())
    {
        ChangeSubState(SubState::MOVE_HANGON_AUTO);
        return;
    }


    // 공중 상태에서는 서브 상태 변경 없음
    if (character->GetIsInAir())
    {
        ChangeSubState(SubState::MOVE_ONAIR);
        return;
    }


    // 아래 입력에 따른 서브 상태 변경

    if (input.moveDown)
    {
        if (character->GetIsCrouching()) 
        {
            ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
            return;
        }

        if (currentSubState == SubState::MOVE_LOOKDOWN_START)
        {
            // START 상태일 땐 상태 유지. 애니메이션 끝나면 LOOP로 전이
            if (character->GetCurrAnimEnd())
                ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
            return;
        }

        if (currentSubState == SubState::MOVE_LOOKDOWN_LOOP)
            return;

        if (!character->GetIsCrouching())
        {
            ChangeSubState(SubState::MOVE_LOOKDOWN_START);
            return;
        }
    }
    else if (input.moveDownReleased) {
        ChangeSubState(SubState::MOVE_LOOKDOWN_RELEASE);
    }
    else
        ChangeSubState(SubState::MOVE_ALONE);

}


void MoveState::ChangeSubState(SubState newSubState)
{
    if (newSubState == SubState::NONE)
    {
        OutputDebugStringA("[Warning] Attempted to change to SubState::NONE!\n");
        return;
    }

    // 공중에 있을 땐 상태는 바꾸되 애니메이션은 바꾸지 않는다
    if (character->GetIsInAir() && character->GetCurrFrameInfo().startFrame.y == 9)
    {
        currentSubState = newSubState;
        return;
    }

    currentSubState = newSubState;
    character->SetAnimationFrameInfo(MOVESTATE, static_cast<int>(newSubState));
}


void MoveState::Exit()
{
    character->SetFrameTime(0.0f);
}

const char* MoveState::GetSubStateName() const
{
    switch (currentSubState)
    {
    case SubState::MOVE_ALONE:            return "MOVE_ALONE"; break;
    case SubState::MOVE_LOOKDOWN_START:   return "MOVE_LOOKDOWN_START"; break;
    case SubState::MOVE_LOOKDOWN_LOOP:    return "MOVE_LOOKDOWN_LOOP"; break;
    case SubState::MOVE_LOOKDOWN_RELEASE: return "MOVE_LOOKDOWN_RELEASE"; break;
    case SubState::MOVE_SLOW:             return "MOVE_SLOW"; break;
    case SubState::MOVE_ONPET:            return "MOVE_ONPET"; break;
    case SubState::MOVE_ONPET_LOOKDOWN:   return "MOVE_ONPET_LOOKDOWN"; break;
    case SubState::MOVE_ONAIR:            return "MOVE_ONAIR"; break;
    case SubState::MOVE_ONAIR_ONPET:      return "MOVE_ONAIR_ONPET"; break;
    case SubState::MOVE_HANGON_AUTO:             return "MOVE_AUTO"; break;
    case SubState::NONE:                  
    default:                              return "NONE"; break;
    }
}
