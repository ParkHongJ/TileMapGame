#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    this->character = character;
  /*
    currentSubState = SubState::NONE;
    character->SetAnimationFrameInfo(IDLESTATE, static_cast<int>(currentSubState));
    character->SetFrameTime(0.f);*/

}

void MoveState::Update()
{
    // 예: 입력 받아 서브상태 전환 처리 가능
    // if (character->IsLookingDown()) { currentSubState = SubState::MOVE_LOOKDOWN; }

    KeyManager* km = KeyManager::GetInstance();


    if (km->IsOnceKeyUp(VK_LEFT)    ||
        km->IsOnceKeyUp(VK_RIGHT)   )
    {
        character->ChangeState(&Character::idleState);
        return;
    }
    
    // 공중에 있을 시 -> 점프 모션
    // 공중에 있어도 shift 누르면 감속 됌
    if (character->GetYVelocity() > 0)
    {
        ChangeSubState(SubState::MOVE_ONAIR);
        
        character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
        
        // 방향키 먼저

        if (km->IsStayKeyDown(VK_LEFT))
        {
            if (km->IsStayKeyDown(VK_SHIFT)) character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);          
            else character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
         
            character->Move(-1);
        }
        else if (km->IsStayKeyDown(VK_RIGHT)) 
        {
            if (km->IsStayKeyDown(VK_SHIFT)) character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
            else character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);

            character->Move(1);
        }

        // SHIFT 먼저

        if (km->IsStayKeyDown(VK_SHIFT))
        {
            character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
            if (km->IsStayKeyDown(VK_LEFT)) character->Move(-1);
            else if (km->IsStayKeyDown(VK_RIGHT)) character->Move(1);
        }

    }
    else // 땅에 붙어 있을 시
    {
        // 달리다 엎드리는 건 무조건 이동 방향키 먼저

        if (km->IsStayKeyDown(VK_LEFT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {

                character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);

                if (!character->GetIsLookDownPaused())
                {
                    if (currentSubState != SubState::MOVE_LOOKDOWN_START)
                        ChangeSubState(SubState::MOVE_LOOKDOWN_START);

                    character->LookDown();
                }
                else
                {

                    if (currentSubState != SubState::MOVE_LOOKDOWN_LOOP)
                    {
                        ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
                    
                    }
                }
               
            }
            else if (km->IsOnceKeyUp(VK_DOWN))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN_RELEASE);

                character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
                character->SetIsLookDownPaused(false);
            }
            else
            {
                ChangeSubState(SubState::MOVE_ALONE);

                character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
                
            }

            character->Move(-1);

        }
        else  if (km->IsStayKeyDown(VK_RIGHT))
        {
            if (km->IsStayKeyDown(VK_DOWN))
            {
                character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
                if (!character->GetIsLookDownPaused())
                {
                    if (currentSubState != SubState::MOVE_LOOKDOWN_START)
                        ChangeSubState(SubState::MOVE_LOOKDOWN_START);

                    character->LookDown();
                }
                else
                {

                    if (currentSubState != SubState::MOVE_LOOKDOWN_LOOP)
                    {
                        ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
                        
                    }
                }

            }
            else if (km->IsOnceKeyUp(VK_DOWN))
            {
                ChangeSubState(SubState::MOVE_LOOKDOWN_RELEASE);

                character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
                character->SetIsLookDownPaused(false);
            }
            else
            {
                ChangeSubState(SubState::MOVE_ALONE);

                character->SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
               
            }


            character->Move(1);

        }

      
    }

    UpdateAnimation();
}

void MoveState::UpdateAnimation()
{
    character->PlayAnimation(); 
}

void MoveState::ChangeSubState(SubState newSubState)
{
    currentSubState = newSubState;
    character->SetAnimationFrameInfo(MOVESTATE, static_cast<int>(newSubState));
}

void MoveState::Exit()
{
    currentSubState = SubState::NONE;
    character->SetFrameTime(0.0f);
    
}

const char* MoveState::GetSubStateName() const
{
    switch (currentSubState)
    {
    case SubState::MOVE_ALONE: return "MOVE_ALONE";
    case SubState::MOVE_LOOKDOWN_LOOP: return "MOVE_LOOKDOWN";
    case SubState::MOVE_ONPET: return "MOVE_ONPET";
    case SubState::MOVE_ONPET_LOOKDOWN: return "MOVE_ONPET_LOOKDOWN";
    case SubState::MOVE_ONAIR: return "MOVE_ONAIR";
    case SubState::MOVE_ONAIR_ONPET: return "MOVE_ONAIR_ONPET";
    case SubState::NONE: default: return "NONE";
    }
}
