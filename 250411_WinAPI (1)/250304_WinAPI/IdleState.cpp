#include "IdleState.h"
#include "Character.h"
#include "KeyManager.h"

void IdleState::Enter(Character* character)
{
		UpdateAnimation(character);
}

void IdleState::Update(Character* character, float deltaTime)
{

    // 입력 등을 받아서 서브상태 변경
    // 예시: 시선이 아래면 서브상태 변경
    KeyManager* km = KeyManager::GetInstance();

    if (km->IsStayKeyDown(VK_RIGHT) ||
        km->IsStayKeyDown(VK_LEFT) ||
        km->IsStayKeyDown(VK_UP) ||
        km->IsStayKeyDown(VK_DOWN) 
        )
    {
        character->ChageState();
    }
        
        
        



        UpdateAnimation(character);
    
}

void IdleState::UpdateAnimation(Character* character)
{
    character->PlayAnimation(0,static_cast<int>(currentSubState));
}

void IdleState::Exit(Character* character)
{
    // 1. 애니메이션 정리
    //character->
    character->SetFrameTime(0.0f);

    // 2. 사운드 정리
    //character->

    // 4. 서브 상태 초기화
    //currentSubState = SubState::IDLE_ALONE;
}
