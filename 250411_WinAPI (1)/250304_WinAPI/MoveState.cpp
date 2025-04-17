#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    this->character = character;
 
    if (character->GetIsLookDownLocked()) ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
    else ChangeSubState(SubState::MOVE_ALONE);
}

void MoveState::Update()
{
    KeyManager* km = KeyManager::GetInstance();

    bool isJump = km->IsOnceKeyDown(VK_SPACE);
    bool inAir = character->GetIsinAir();

    bool isLeft = km->IsStayKeyDown(VK_LEFT);
    bool isRight = km->IsStayKeyDown(VK_RIGHT);
    bool isDown = km->IsStayKeyDown(VK_DOWN);
    bool isDownUp = km->IsOnceKeyUp(VK_DOWN);
    bool isShift = km->IsStayKeyDown(VK_SHIFT);

    // 좌우 키에서 손을 뗐을 때
    if (km->IsOnceKeyUp(VK_LEFT) || km->IsOnceKeyUp(VK_RIGHT))
    {
        if (currentSubState == SubState::MOVE_LOOKDOWN_LOOP)
            character->SetIsLookDownLocked(true);

        character->SetXVelocity(0.0f); // dir 제거 → velocity.x = 0
        character->ChangeState(&Character::idleState);
        return;
    }

    float speed = character->GetSpeed();
    float vx = 0.f;

    if (isLeft)  vx = -speed;
    if (isRight) vx = speed;

    // 공중 상태일 때
    if (inAir)
    {
        if (vx != 0.f)
            character->SetXVelocity(vx);
    }
    else
    {
        if (vx != 0.f)
        {
            // 하강 애니메이션이 끝나면 ALONE 상태로 복귀
            if (currentSubState == SubState::MOVE_LOOKDOWN_RELEASE)
            {
                if (character->GetCurrAnimEnd())
                    ChangeSubState(SubState::MOVE_ALONE);
            }
            else
            {
                if (isDown)
                {
                    if (!character->GetIsLookDownLocked() && currentSubState != SubState::MOVE_LOOKDOWN_LOOP)
                    {
                        ChangeSubState(SubState::MOVE_LOOKDOWN_START);
                        if (character->GetCurrAnimEnd())
                            character->SetIsLookDownLocked(true);
                    }
                    else ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
                }
                else if (isDownUp)
                {
                    ChangeSubState(SubState::MOVE_LOOKDOWN_RELEASE);
                    character->SetIsLookDownLocked(false);
                }
                else
                {
                    ChangeSubState(SubState::MOVE_ALONE);
                }
            }

            // 속도 조정
            if (isShift || isDown) character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
            else                   character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);

            character->SetXVelocity(vx); // 방향 → 속도 적용
        }
    }
}



void MoveState::ChangeSubState(SubState newSubState)
{
    if (newSubState == SubState::NONE)
    {
        OutputDebugStringA("[Warning] Attempted to change to SubState::NONE!\n");
        return;
    }

    // 공중에 있을 땐 상태는 바꾸되 애니메이션은 바꾸지 않는다
    if (character->GetIsinAir() && character->GetCurrFrameInfo().startFrame.y == 9)
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
    case SubState::NONE:                  
    default:                              return "NONE"; break;
    }
}
