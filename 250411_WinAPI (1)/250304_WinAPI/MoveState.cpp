#include "MoveState.h"
#include "Character.h"

void MoveState::Enter(Character* character)
{
    this->character = character;
 
    if (character->GetIsLookDownPaused()) ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
    else ChangeSubState(SubState::MOVE_ALONE);


}

void MoveState::Update()
{
    KeyManager* km = KeyManager::GetInstance();


    if (km->IsOnceKeyUp(VK_LEFT)    ||
        km->IsOnceKeyUp(VK_RIGHT)   )
    {
        if(currentSubState == SubState::MOVE_LOOKDOWN_LOOP)
           character->SetIsLookDownPaused(true);

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

        if (km->IsStayKeyDown(VK_LEFT))
        {
            if (currentSubState == SubState::MOVE_LOOKDOWN_RELEASE)
            {
                if (character->GetCurrAnimEnd()) ChangeSubState(SubState::MOVE_ALONE);
            }
            else
            {
                if (km->IsStayKeyDown(VK_DOWN))
                {

                    character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);

                    if (!character->GetIsLookDownPaused() &&
                        // currentSubState != SubState::MOVE_LOOKDOWN_START &&
                        currentSubState != SubState::MOVE_LOOKDOWN_LOOP)
                    {
                        ChangeSubState(SubState::MOVE_LOOKDOWN_START);
                        character->LookDown();
                    }
                    else
                    {

                        character->SetIsLookDownPaused(true);
                        ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);

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
            }
            

            character->Move(-1);

        }
        else  if (km->IsStayKeyDown(VK_RIGHT))
        {
            if (currentSubState == SubState::MOVE_LOOKDOWN_RELEASE)
            {
                if (character->GetCurrAnimEnd()) ChangeSubState(SubState::MOVE_ALONE);
            }
            else
            {
                if (km->IsStayKeyDown(VK_DOWN))
                {

                    character->SetSpeed(CHARACTER_MOVE_SLOW_SPEED);

                    if (!character->GetIsLookDownPaused() &&
                        //currentSubState != SubState::MOVE_LOOKDOWN_START &&
                        currentSubState != SubState::MOVE_LOOKDOWN_LOOP)
                    {
                        ChangeSubState(SubState::MOVE_LOOKDOWN_START);
                        character->LookDown();
                    }
                    else
                    {
                        character->SetIsLookDownPaused(true);
                        ChangeSubState(SubState::MOVE_LOOKDOWN_LOOP);
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
    if (newSubState == SubState::NONE)
    {
        OutputDebugStringA("[Warning] Attempted to change to SubState::NONE!\n");
        return;
    }

    currentSubState = newSubState;
    character->SetAnimationFrameInfo(MOVESTATE, static_cast<int>(newSubState));
}

void MoveState::Exit()
{
    //currentSubState = SubState::NONE;
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
