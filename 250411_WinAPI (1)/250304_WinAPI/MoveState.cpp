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


    // ���� ���¿����� ���� ���� ���� ����
    if (character->GetIsInAir())
    {
        ChangeSubState(SubState::MOVE_ONAIR);
        return;
    }


    // �Ʒ� �Է¿� ���� ���� ���� ����

    if (input.moveDown)
    {
        if (character->GetIsCrouching()) 
        {
            ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
            return;
        }

        if (currentSubState == SubState::MOVE_LOOKDOWN_START)
        {
            // START ������ �� ���� ����. �ִϸ��̼� ������ LOOP�� ����
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

    // ���߿� ���� �� ���´� �ٲٵ� �ִϸ��̼��� �ٲ��� �ʴ´�
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
