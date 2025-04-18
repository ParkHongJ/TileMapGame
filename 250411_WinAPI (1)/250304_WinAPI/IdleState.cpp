#include "pch.h"
#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character) {
    this->character = character;

    if (character->GetIsLookDownLocked()) ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
    else if (character->GetIsLookUpLocked()) ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    else ChangeSubState(SubState::IDLE_ALONE);
}

void IdleState::Update() {
    KeyManager* km = KeyManager::GetInstance();

    if (km->IsStayKeyDown(VK_LEFT) || km->IsStayKeyDown(VK_RIGHT)) {
        character->ChangeState(&Character::moveState);
        character->SetIsLookUpLocked(false);
        character->SetIsLookDownLocked(false);
        return;
    }

    if (character->GetIsInAir()) {
        ChangeSubState(SubState::IDLE_ONAIR);
        return;
    }

    if (currentSubState == SubState::IDLE_ALONE) {
        if (character->CheckAlmostFall()) {
            ChangeSubState(SubState::IDLE_FALL_ALMOST);
            character->SetAnimationFrameInfo(IDLESTATE, static_cast<int>(SubState::IDLE_FALL_ALMOST));
            return;
        }
    }

    if (km->IsStayKeyDown(VK_UP)) {
        if (!character->GetIsLookUpLocked())
            ChangeSubState(SubState::IDLE_LOOKUP_START);
        else
            ChangeSubState(SubState::IDLE_LOOKUP_STOP);
    }
    else if (km->IsOnceKeyUp(VK_UP)) {
        ChangeSubState(SubState::IDLE_LOOKUP_RELEASE);
    }

    if (km->IsStayKeyDown(VK_DOWN)) {
        if (!character->GetIsLookDownLocked())
            ChangeSubState(SubState::IDLE_LOOKDOWN_START);
        else
            ChangeSubState(SubState::IDLE_LOOKDOWN_STOP);
    }
    else if (km->IsOnceKeyUp(VK_DOWN)) {
        ChangeSubState(SubState::IDLE_LOOKDOWN_RELEASE);
    }

    if ((currentSubState == SubState::IDLE_LOOKDOWN_RELEASE ||
        currentSubState == SubState::IDLE_LOOKUP_RELEASE) &&
        character->GetCurrFrameInd().x >= character->GetCurrFrameInfo().endFrame.x) {
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
    case SubState::IDLE_HURT:             return "IDLE_HURT";
    case SubState::IDLE_DIE:              return "IDLE_DIE";
    case SubState::IDLE_ONAIR:              return "IDLE_ONAIR";
    case SubState::NONE: default:         return "NONE";
    }
}