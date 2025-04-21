#include "pch.h"
#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character) {
    this->character = character;

    InputIntent input = character->GetCurrInputIntent();

    if (input.moveDown) ChangeSubState(SubState::IDLE_LOOKDOWN_START);
    else if (input.moveUp) ChangeSubState(SubState::IDLE_LOOKUP_START);
    else if (character->IsAirborne()) ChangeSubState(SubState::IDLE_ONAIR);
    else ChangeSubState(SubState::IDLE_ALONE);
   
}

void IdleState::Update() {

    InputIntent input = character->GetCurrInputIntent();

    /*if (character->GetFallFromHeight())
    {
        ChangeSubState(SubState::IDLE_FALL_FROM_HEIGHT);
        return;
    }*/




    if (character->IsAirborne()) {
        ChangeSubState(SubState::IDLE_ONAIR);
        return;
    }

    if (character->CheckAlmostFall()) {
        ChangeSubState(SubState::IDLE_FALL_ALMOST);
        character->SetAnimationFrameInfo(IDLESTATE, static_cast<int>(SubState::IDLE_FALL_ALMOST));
        return;
    }
    

    // TOOD : 아이템을 들고 있는 IDLE 상태


    if (input.moveUp) {
        if (!character->GetIsLookUpLocked())
            ChangeSubState(SubState::IDLE_LOOKUP_START);
        else
            ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    }
    else if (input.moveUpReleased) {
        ChangeSubState(SubState::IDLE_LOOKUP_RELEASE);
    }

    if(input.moveDown)
    {
        if (character->GetIsCrouching())
        {
            ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
            return;
        }

        // 이미 START 상태면 애니메이션 끝날 때까지 대기
        if (currentSubState == SubState::IDLE_LOOKDOWN_START)
        {
            if (character->GetCurrAnimEnd())
            {
                ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
            }
            return;
        }

        // STOP 상태면 그대로 유지
        if (currentSubState == SubState::IDLE_LOOKDOWN_STOP)
        {
            return;
        }

        // 기본 진입은 START 상태로
        if (!character->GetIsCrouching())
        {
            ChangeSubState(SubState::IDLE_LOOKDOWN_START);
            return;
        }
    }
    else if (input.moveDownReleased)
    {
        ChangeSubState(SubState::IDLE_LOOKDOWN_RELEASE);
    }
 

    if ((currentSubState == SubState::IDLE_LOOKDOWN_RELEASE ||
        currentSubState == SubState::IDLE_LOOKUP_RELEASE) &&
        character->GetCurrAnimEnd()) 
    {
        ChangeSubState(SubState::IDLE_ALONE);
    }

    if (character->IsAirborne())
    {
        ChangeSubState(SubState::IDLE_ONAIR);
    }
    else
    {
        ChangeSubState(SubState::IDLE_ALONE);
    }

}

void IdleState::ChangeSubState(SubState newSubState) {
    if (newSubState == SubState::NONE) {
        OutputDebugStringA("[Warning] Attempted to change to SubState::NONE!\n");
        return;
    }
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(IDLESTATE, static_cast<unsigned int>(newSubState));
}

void IdleState::Exit() {
    character->SetFrameTime(0.0f);
}

const char* IdleState::GetSubStateName() const {
    switch (currentSubState) {
    case SubState::IDLE_ALONE:            return "IDLE_ALONE";
    case SubState::IDLE_LOOKUP_START:     return "IDLE_LOOKUP_START";
    case SubState::IDLE_LOOKUP_STOP:      return "IDLE_LOOKUP_STOP";
    case SubState::IDLE_LOOKUP_RELEASE:   return "IDLE_LOOKUP_RELEASE";
    case SubState::IDLE_LOOKDOWN_START:   return "IDLE_LOOKDOWN_START";
    case SubState::IDLE_LOOKDOWN_STOP:    return "IDLE_LOOKDOWN_STOP";
    case SubState::IDLE_LOOKDOWN_RELEASE: return "IDLE_LOOKDOWN_RELEASE";
    case SubState::IDLE_ONPET:            return "IDLE_ONPET";
    case SubState::IDLE_ONPET_LOOKUP:     return "IDLE_ONPET_LOOKUP";
    case SubState::IDLE_ONPET_LOOKDOWN:   return "IDLE_ONPET_LOOKDOWN";
    case SubState::IDLE_FALL_ALMOST:      return "IDLE_FALL_ALMOST";
    case SubState::IDLE_FALL_FROM_HEIGHT:             return "IDLE_HURT";
    case SubState::IDLE_DIE:              return "IDLE_DIE";
    case SubState::IDLE_ONAIR:              return "IDLE_ONAIR";
    case SubState::NONE: default:         return "NONE";
    }
}