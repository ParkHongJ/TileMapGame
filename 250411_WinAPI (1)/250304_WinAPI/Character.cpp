#include "pch.h"
#include "Character.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CharacterState.h"
#include "CameraManager.h"
#include "Collider.h"

// Add JunYong
#include "PlayerStatus.h"
#include "Bomb.h"


IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);

HRESULT Character::Init()
{
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");

    state = &Character::idleState;
    state->Enter(this);
  
    SetPos({ WINSIZE_X / 2,0 });
    SetPos({ 200 / 2,0});

    velocity = { 0.0f, 0.0f };


    gravity = 1000.f; // 중력 가속도 (픽셀/sec^2)
    maxFallSpeed = 800.f; // 최대 낙하 속도
    jumpPower = 500.0f;


    // Render
    frameTime = 0.0f;
    currFrameInd = { 0,0 };
    currFrameInfo = { { 0,0 }, {0, 0} };
    

    colliderSize = { 30.0f, 50.0f };

    leftHandColliderSize = rightHandColliderSize = { 20.0f , 10.f };
 
    colliderOffsetY = 25.f;

    collider = new BoxCollider(
        { 0.0f , colliderOffsetY },     // Offset
        { colliderSize.x, colliderSize.y },  // 
        this
    );

    leftHandCollider = new BoxCollider(
        { -colliderSize.x, -10.0f },
        {   leftHandColliderSize.x, leftHandColliderSize.y},
        this
    );


    rightHandCollider = new BoxCollider(
        { colliderSize.x ,-10.f },
        { rightHandColliderSize.x, rightHandColliderSize.y },
        this
    );

    


    // settings
    speed = 200.f;
    attackSpeed = 3.0f;
    attackRate = 0.3f;

    // boolean
    isFlip = false;
    isInAir = false;
    isAttacking = false;

    jumpPressed = false;
    attackPressed = false;
    isLookUpLocked = false;

    currLockTime = 0.0f;
    lookUpLockTime = 1.0f;
    lookDownLockTime = 1.0f;


    isTouchingLeft = false;
    isTouchingRight = false;
    isTouchingTop = false;
    isTouchingBottom = false;


    isOnLadder = false;
    isOnRope = false;
    isHangOn = false;

    interActionPQ = {};
    interactionRadius = 25.f;
    interactionOffset = 40.f;

    InitAnimationMap();

    // Add Junyong
    playerStatus = new PlayerStatus();

    return S_OK;
}

void Character::Release()
{
    if (playerImage)
    {
        ImageManager::GetInstance()->DeleteImage("Tae_Player");
        playerImage = nullptr;

    }

    // Add JunYong
    if (playerStatus)
    {
        delete playerStatus;
        playerStatus = nullptr;
    }
}

void Character::Update(float TimeDelta)
{
    CheckCollision();
    isInAir = !isTouchingBottom;
     
    auto km = KeyManager::GetInstance();
    if (km->IsOnceKeyDown('Z') && isTouchingBottom)
    {
        SetYVelocity(-GetJumpPower()); 
        SetIsInAir(true);
    }

    // Add JunYong
    if (km->IsOnceKeyDown('F'))
    {
        if (0 < playerStatus->GetBombCount())
        {
            if (isTouchingBottom)
            {
                Bomb* temp = new Bomb();
                ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
                FPOINT offset = { 100,0 };
                temp->SetPos(Pos + offset);
            }

            else
            {
                Bomb* temp = new Bomb();
                ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
                FPOINT offset = { 100,0 };
                temp->SetPos(Pos + offset);
            }

            playerStatus->MinusBombCount();
        }

    }

    char debug[128];
    sprintf_s(debug, "islookdownlocked  : %d islookuplocked: %d\n", isLookDownLocked,isLookUpLocked);
    OutputDebugStringA(debug);
    
   
    // 상태 전이 판단
    HandleTransitions();

    // 현재 상태 업데이트
    if (state)
        state->Update();


    if (state == &idleState) HandleIdleLogic();
    else if (state == &moveState) HandleMoveLogic();
    else if (state == &attackState) HandleAttackLogic();
    else if (state == &interactionState) HandleInteractionLogic();

    // 공중 점프 애니메이션 처리
    if(!isHangOn)
        HandleAirAnimation();

    PlayAnimation();
    ApplyGravity(TimeDelta);


    Move();


    // Camera에 정보 전달
    auto cm = CameraManager::GetInstance();
    cm->SetTargetPos(Pos);
    cm->SetLookingState(isLookUpLocked, isLookDownLocked);

}

void Character::HandleTransitions()
{
    KeyManager* km = KeyManager::GetInstance();
    bool isLeft = km->IsStayKeyDown(VK_LEFT);
    bool isRight = km->IsStayKeyDown(VK_RIGHT);
    bool isAnyDirection = isLeft || isRight;
    bool isAttack = km->IsOnceKeyDown('X');
    bool isJump = km->IsOnceKeyDown('Z');
    bool isInter = km->IsOnceKeyDown('A');
    bool isUp = km->IsStayKeyDown(VK_UP);

    if (isInAir)
    {

        if (CheckHangOn())
        {
            isHangOn = true;
            velocity = { 0.0f,0.0f };
            ChangeState(&interactionState);

            return;
        }

    }


    if (isUp || isOnLadder || isOnRope)
    {
        
        OutputDebugStringA("==================올라탈 곳 검사중=========================");
        CollisionManager::GetInstance()->GetInteractObjectsInCircle(this, interactionRadius, interActionPQ);

        if (!interActionPQ.empty())
        {
            // 사다리, 로프 pos 로 플레이어 위치 조정

            /*if (interActionPQ.top() == 사다리 || interActionPQ.top() == 로프)
            {
                isOnLadder = true;
                isOnRope = true;

                ChangeState(&interactionState);
                return;
            }
            else if (interActionPQ.top() == 상인)
            {

            }
            else if (interActionPQ.top() == )
            {

            }*/

        }
    }



    // 0. 점프 (공격 중이더라도 점프 가능)
    if (isJump && isTouchingBottom)
    {
        velocity.y = -jumpPower;
        isInAir = true;
        isOnLadder = false;
        isOnRope = false;

        interActionPQ = {};
    }

    
    // 공격 
    char buf[128];
    sprintf_s(buf, "  isAttacking 변경됨 → %s\n", isAttacking ? "true" : "false");
    OutputDebugStringA(buf);

    if (isAttacking) return;

     if (isAttack && !isAttacking)
    {
        isAttacking = true;

        ChangeState(&attackState);

        return;
    }

    // 이동
    if (isAnyDirection && !isInAir)
    {
        ChangeState(&moveState);
        return;
    }

    // 공중 아닐 때만 idle
    if (!isAnyDirection && !isInAir)
    {
        ChangeState(&idleState);
    }

}

void Character::HandleAirAnimation()
{
    const int AIR_ANIM_ROW = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y;
    const auto& frame = currFrameInfo;

    if (isInAir && !isAttacking)
    {
        if (frame.startFrame.y != AIR_ANIM_ROW)
        {
            if (velocity.y < 0)
                SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP));
            else
                SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN));
        }
        else if (frame.startFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x&&
            frame.endFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y&&
            velocity.y >= 0)
        {
            SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN));
        }
    }
    else if (frame.startFrame.y == AIR_ANIM_ROW)
    {
        // 착지하면 상태 재진입
        if (state)
            state->Enter(this);
    }
}



bool Character::CheckAlmostFall()
{
    float edgeCheckOffset = 5.f; // 캐릭터 발 위치 기준 양쪽 끝에서 검사

    FPOINT footLeft;
    FPOINT footRight;

    if (isFlip)
    {
        footLeft = { Pos.x + colliderSize.x / 2 , Pos.y + colliderSize.y / 2 + colliderOffsetY };
        footRight = { Pos.x , Pos.y + colliderSize.y / 2 + colliderOffsetY };
    }
    else
    {
        footLeft = { Pos.x - colliderSize.x / 2 , Pos.y + colliderSize.y / 2 + colliderOffsetY };
        footRight = { Pos.x , Pos.y + colliderSize.y / 2 + colliderOffsetY };
    
    }
    
    RaycastHit hitLeft, hitRight;

    bool isGroundLeft = CollisionManager::GetInstance()->RaycastAll({ footLeft, {0.f, 1.f} }, 10.f, hitLeft, false , 1.0f, this);
    bool isGroundRight = CollisionManager::GetInstance()->RaycastAll({ footRight, {0.f, 1.f} }, 10.f, hitRight, false , 1.0f, this);

    // 둘 중 하나만 떠 있으면 가장자리
    return (isGroundLeft ^ isGroundRight); // XOR


}

bool Character::CheckHangOn()
{
    float maxHangDist = 4.0f;      // 매달릴 수 있는 거리
    float debugTime = 1.0f;        // 디버그 Ray 지속 시간
    bool debugDraw = true;

    RaycastHit hitLeft, hitRight;

    // 왼손 아래로 Ray 쏘기
    Ray leftRay = {
        leftHandCollider->GetWorldPos(),
        { 0.f, 1.f }
    };

    bool leftHang = CollisionManager::GetInstance()->RaycastAll(leftRay, maxHangDist, hitLeft, debugDraw, debugTime, this);

    // 오른손 아래로 Ray 쏘기
    Ray rightRay = {
        rightHandCollider->GetWorldPos(),
        { 0.f, 1.f }
    };

    bool rightHang = CollisionManager::GetInstance()->RaycastAll(rightRay, maxHangDist, hitRight, debugDraw, debugTime, this);

    // 둘 중 하나라도 맞으면 매달릴 수 있음
    return leftHang || rightHang;
}


void Character::HandleIdleLogic() {
    IdleState* idle = dynamic_cast<IdleState*>(state);
    
    if (!idle) return;


    float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
   
    switch (idle->GetCurrentSubState()) {
    case IdleState::SubState::IDLE_LOOKUP_START:
        currLockTime += deltaTime;
        if (GetCurrAnimEnd() && currLockTime > lookUpLockTime)
            isLookUpLocked = true;



        break;

    case IdleState::SubState::IDLE_LOOKUP_STOP:
        break;

    case IdleState::SubState::IDLE_LOOKUP_RELEASE:
        currLockTime = 0.0f;
        isLookUpLocked = false;
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_START:
        currLockTime += deltaTime;
        if (GetCurrAnimEnd() && currLockTime > lookDownLockTime)
            isLookDownLocked = true;
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_STOP:
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_RELEASE:
        currLockTime = 0.0f;
        isLookDownLocked = false;
        break;

    case IdleState::SubState::IDLE_ALONE:
        break;

    case IdleState::SubState::IDLE_ONAIR:
        break;

    case IdleState::SubState::IDLE_ONPET:
    case IdleState::SubState::IDLE_ONPET_LOOKUP:
    case IdleState::SubState::IDLE_ONPET_LOOKDOWN:
    case IdleState::SubState::IDLE_FALL_ALMOST:

        break;
    case IdleState::SubState::IDLE_HURT:
    case IdleState::SubState::IDLE_DIE:
        break;

    case IdleState::SubState::NONE:
    default:
        break;
    }

 
}

void Character::HandleMoveLogic() {
    MoveState* move = dynamic_cast<MoveState*>(state);
    if (!move) return;

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");


    // 현재 서브상태에 따른 처리
    switch (move->GetCurrentSubState()) {
    case MoveState::SubState::MOVE_LOOKDOWN_START:
        break;

    case MoveState::SubState::MOVE_LOOKDOWN_RELEASE:
        
        break;

    case MoveState::SubState::MOVE_LOOKDOWN_LOOP:
    case MoveState::SubState::MOVE_ALONE:
        break;
    }



}


void Character::HandleAttackLogic() {
    AttackState* attack = dynamic_cast<AttackState*>(state);
    if (!attack) return;

    switch (attack->GetCurrentSubState()) {
    case AttackState::SubState::ATTACK_WHIP:
        // TODO : 애니메이션 프레임별로 collision 검사, 채찍 아이템 render
        break;

    case AttackState::SubState::ATTACK_ITEM_THROW:

        break;

    default:
        break;
    }

  
}

void Character::HandleInteractionLogic()
{
    InteractionState* inter = dynamic_cast<InteractionState*>(state);
    if (!inter) return;

    switch (inter->GetCurrentSubState()) {
    case InteractionState::SubState::INTERACTION_CLIMB_LADDER:
    case InteractionState::SubState::INTERACTION_CLIMB_ROPE:
        if (!MoveY()) ChangeState(&moveState);
        break;

    default:
        break;
    }
}

void Character::InitAnimationMap()
{
     // IDLE
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_ALONE)}] =
    { {0, 0}, {0, 0}, AnimationMode::Loop };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_ONAIR)}] =
    { {0, 9}, {7, 9}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_START)}] =
    { {0, 8}, {3, 8}, AnimationMode::FreezeAtX };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_STOP)}] =
    { {3, 8}, {3, 8}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKUP_RELEASE)}] =
    { {4, 8}, {6, 8}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_START)}] =
    { {0, 1}, {2, 1}, AnimationMode::FreezeAtX};

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_STOP)}] =
    { {2, 1}, {2, 1}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_FALL_ALMOST)}] =
    { {0, 3}, {7, 3}, AnimationMode::Loop };




    // MOVE
    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_ALONE)}] =
    { {1, 0}, {8, 0}, AnimationMode::Loop };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_LOOP)}] =
    { {5, 1}, {11, 1}, AnimationMode::Loop };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_START)}] =
    { {0, 1}, {2, 1}, AnimationMode::FreezeAtX };

    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };
    
    animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_ONAIR)}] =
    { {0, 9}, {7, 9}, AnimationMode::Hold };

    // ATTACK
    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_WHIP)}] =
    { {0, 4}, {5, 4}, AnimationMode::Hold };

    // INTERACTION

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_CLIMB_LADDER)}] =
    { {0, 6},  {5, 7}, AnimationMode::Loop };


    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_CLIMB_ROPE)}] =
    { {0, 7},  {9, 7}, AnimationMode::Loop };

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_HANGON_TILE)}] =
    { {8, 3},  {11, 3}, AnimationMode::Hold };


    // JUMP 

    animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}] =
    { {0, 9}, {3, 9}, AnimationMode::FreezeAtX }; // 상승 프레임
    animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}] =
    { {4, 9}, {7, 9}, AnimationMode::FreezeAtX }; // 하강 프레임


}
//
//void Character::SetAnimationRange(PlayerState state)
//{
//    switch (state)
//    {
//        case PlayerState::IDLE:                  currFrameInfo = { {0, 0},  {0, 0} };            break;
//        case PlayerState::MOVE:                  currFrameInfo = { {1, 0},  {9, 0} };            break;
//        case PlayerState::LOOKUP_START:
//        case PlayerState::LOOKUP_RELEASE:        currFrameInfo = { {0, 8},  {6, 8} };            break;
//        case PlayerState::ONPET_LOOKUP_START:     currFrameInfo = { {8, 8},  {14, 8} };           break;
//        case PlayerState::LOOKDOWN_START:
//        case PlayerState::LOOKDOWN_RELEASE:      currFrameInfo = { {0, 1},  {4, 1} };            break;
//        case PlayerState::LOOKDOWN_MOVE:         currFrameInfo = { {5, 1},  {11, 1}};            break;
//        case PlayerState::LOOKDOWN_IDLE:         currFrameInfo = { {2, 1},  {2, 1} };            break;
//        case PlayerState::CLIMB_LADDER:          currFrameInfo = { {0, 6},  {5, 7} };            break;
//        case PlayerState::CLIMB_ROPE:            currFrameInfo = { {0, 7},  {9, 7} };            break;
//        case PlayerState::ON_NOTTAMEDPET:        currFrameInfo = { {4, 11}, {9, 11}};            break;
//        case PlayerState::ONPET_IDLE:            currFrameInfo = { {7, 8},  {7, 8} };            break;
//        case PlayerState::ALMOST_FALL:           currFrameInfo = { {0, 3},  {7, 3} };            break;
//        case PlayerState::ATTACK:                currFrameInfo = { {0, 4},  {5, 4} };            break;
//        case PlayerState::HANG:                  currFrameInfo = { {8, 3},  {11, 3}};            break;
//        case PlayerState::PUSH:                  currFrameInfo = { {6, 6},  {11, 6}};            break;
//        case PlayerState::DIE:                   currFrameInfo = { {9, 0},  {9, 0} };            break;
//        case PlayerState::FALL:                  currFrameInfo = { {0, 2},  {3, 2} };            break;
//        case PlayerState::FALL_STUNEFFECT:       currFrameInfo = { {0, 13}, {11, 13} };          break;
//        case PlayerState::ENTER_TUNNEL:          currFrameInfo = { {0, 5},  {5, 5} };            break;
//        case PlayerState::EXIT_TUNNEL:           currFrameInfo = { {6, 5},  {11, 5}};            break;
//        
//    default:
//        //currFrameInfo = { {0, 0}, {0, 0} };
//        break;
//    }
//}


void Character::SetAnimationFrameInfo(unsigned int stateClassNum, unsigned int subState)
{
    auto key = std::make_pair(stateClassNum, subState);
    auto it = animationMap.find(key);
    if (it != animationMap.end())
    {
        OutputDebugStringA(" 애니메이션 프레임 초기화 호출됨\n");
        // 프레임 초기화는 진짜로 바뀐 경우에만 하게끔
        if (currFrameInfo.startFrame.x != it->second.startFrame.x ||
            currFrameInfo.startFrame.y != it->second.startFrame.y ||
            currFrameInfo.endFrame.x != it->second.endFrame.x ||
            currFrameInfo.endFrame.y != it->second.endFrame.y ||
            currFrameInfo.mode != it->second.mode)
        {
            currFrameInd = it->second.startFrame;
            frameTime = 0.f;

            currFrameInfo = it->second;
        }

    }
}


void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();
    if (state)
    {
        char buf[256];
        sprintf_s(buf,
            "▶ Render Frame: (%d,%d)\n▶ State: %s  \n LookDownLocked : %d Speed: %f Velocity : x = %f y = %f",
            currFrameInd.x, currFrameInd.y, state->GetSubStateName(), isLookDownLocked, speed, velocity.x, velocity.y
            );

        OutputDebugStringA(buf);
    }

    if (playerImage)
    {
        playerImage->FrameRender(renderTarget, pos.x, pos.y, currFrameInd.x, currFrameInd.y, isFlip);
    }



}


void Character::PlayAnimation()
{
    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    frameTime += TimeDelta;

    if (frameTime < ANIMATION_FRAME_TIME)
    {
        return;
    }
   
    frameTime = 0.f;


    if (isHangOn)
    {
        switch (currFrameInfo.mode)
        {
        case AnimationMode::Loop:
            currFrameInd.x++;
            if (currFrameInd.x > currFrameInfo.endFrame.x)
                currFrameInd.x = currFrameInfo.startFrame.x;
            break;

        case AnimationMode::FreezeAtX:
        case AnimationMode::Hold:
            if (currFrameInd.x < currFrameInfo.endFrame.x)
                currFrameInd.x++;
            break;
        }
    }

    // 공중 애니메이션일 경우
    if (currFrameInfo.startFrame.y == 9 && isInAir)
    {
        float vel = velocity.y;

        const int JUMP_UP_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x;
        const int JUMP_UP_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].endFrame.x;
        const int JUMP_DOWN_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame.x;
        const int JUMP_DOWN_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].endFrame.x;

        const float MAX_JUMP_VEL = jumpPower;

        float ratio = 0.f;

        if (vel < 0) // 상승 중
        {
            ratio = -vel / MAX_JUMP_VEL;
            if (ratio < 0.f) ratio = 0.f;
            if (ratio > 1.f) ratio = 1.f;

            int frame = static_cast<int>(JUMP_UP_START + ratio * (JUMP_UP_END - JUMP_UP_START));
            currFrameInd.x = frame;
        }
        else // 하강 중
        {
            ratio = vel / MAX_JUMP_VEL;
            if (ratio < 0.f) ratio = 0.f;
            if (ratio > 1.f) ratio = 1.f;

            int frame = static_cast<int>(JUMP_DOWN_START + ratio * (JUMP_DOWN_END - JUMP_DOWN_START));
            currFrameInd.x = frame;
        }

        return;
    }

    // 일반 애니메이션
    switch (currFrameInfo.mode)
    {
        case AnimationMode::Loop:
            currFrameInd.x++;
            if (currFrameInd.x > currFrameInfo.endFrame.x)
                currFrameInd.x = currFrameInfo.startFrame.x;
            break;

        case AnimationMode::FreezeAtX:
        case AnimationMode::Hold:
            if (currFrameInd.x < currFrameInfo.endFrame.x)
                currFrameInd.x++;
        break;
    }

}

void Character::ChangeState(CharacterState* newState)
{
    if (state == newState)
        return; // 상태가 같으면 전이하지 않음

    if (state) state->Exit();
    state = newState;
    if (state) state->Enter(this);
}


bool Character::PressAnyKey(void)
{
    KeyManager* km = KeyManager::GetInstance();

    for (int key = 0; key < MAX_KEY_COUNT; ++key)
    {
        if (km->IsOnceKeyDown(key))
            return true;
    }
    return false;
}

float Character::GetVelocitySize()
{
    return sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
}

float Character::GetYVelocity()
{
    return velocity.y;
}

bool Character::GetIsLookUpLocked()
{
    return isLookUpLocked;
}

bool Character::GetIsLookDownLocked()
{
    return isLookDownLocked;
}

POINT Character::GetCurrFrameInd() const
{
    return currFrameInd;
}

FrameInfo Character::GetCurrFrameInfo() const
{
    return currFrameInfo;
}

bool Character::GetCurrAnimEnd()
{
    if (currFrameInd.x == currFrameInfo.endFrame.x) return true;
    else return false;
}


void Character::ApplyGravity(float TimeDelta)
{
    if (isInAir)
    {
        float fallDist = velocity.y * TimeDelta;

        if (isTouchingBottom && fallDist > bottomHitDist)
        {
            // 땅 위에 착지하도록 거리 보정
            Pos.y += bottomHitDist - colliderOffsetY - 0.1f;
            velocity.y = 0.f;
            isInAir = false;
            return;
        }

        Pos.y += fallDist;
        velocity.y += gravity * TimeDelta;
        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;
    }
    else if (isOnLadder || isOnRope)
    {
        // 사다리, 밧줄 시에는 중력 적용 X
        isInAir = false;
    }
    else
    {
        velocity.y = 0.f;
        isInAir = false;
    }
}


void Character::CheckCollision()
{
    float maxDist = 10.0f;
    float debugTime = 1.0f;

    // Collider 기준 
    FPOINT leftTop = { Pos.x - colliderSize.x/2, Pos.y - colliderSize.y/2 + colliderOffsetY };
    FPOINT rightTop = { Pos.x + colliderSize.x/2, Pos.y - colliderSize.y/2+ colliderOffsetY };
    FPOINT leftBottom = { Pos.x - colliderSize.x/2, Pos.y + colliderSize.y/2 + colliderOffsetY };
    FPOINT rightBottom = { Pos.x + colliderSize.x/2, Pos.y + colliderSize.y/2 + colliderOffsetY };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

    bool onDebug = false;

    isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, onDebug, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, onDebug, debugTime, this);

    isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, onDebug, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, onDebug, debugTime, this);

    isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, onDebug, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, onDebug, debugTime, this);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, onDebug, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, onDebug, debugTime, this);

   

    if (hitBottom1.hit && hitBottom2.hit)
        bottomHitDist = min(hitBottom1.distance, hitBottom2.distance);
    else if (hitBottom1.hit)
        bottomHitDist = hitBottom1.distance;
    else if (hitBottom2.hit)
        bottomHitDist = hitBottom2.distance;


   /* if (isTouchingLeft || isTouchingRight || isTouchingTop|| isTouchingBottom)
    {
        GameObject* obj = hitLeft1.collider->GetOwner();
        int a = 10;
    }*/
}

void Character::Move()
{
    KeyManager* km = KeyManager::GetInstance();

    float vx = 0.f;
    if (km->IsStayKeyDown(VK_LEFT))       vx = -GetSpeed();
    else if (km->IsStayKeyDown(VK_RIGHT)) vx = GetSpeed();
    else                                  vx = 0.f;

    SetXVelocity(vx);

    if (!GetIsInAir()) {
        if (km->IsStayKeyDown(VK_DOWN) || km->IsStayKeyDown(VK_SHIFT))
            SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
        else
            SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
    }

    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    float moveDist = velocity.x * TimeDelta;

    // 공중에서 벽에 부딪혔을 때 이동을 막음
    if (GetIsInAir()) {
        if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight) ) {
            velocity.x = 0.f;  // 공중에서는 이동을 멈춤
            moveDist = 0.f;    // 이동량을 0으로 설정
        }
        if ( (velocity.y < 0 && isTouchingTop)  ) {
            velocity.y = 0.f;  // 공중에서는 이동을 멈춤
          //  moveDist = 0.f;    // 이동량을 0으로 설정
        }
    }
    else {
        // 지상에서는 이동 자체를 막음 (벽에 부딪히면)
        if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight)) {
            return;  // 벽에 부딪혔으므로 이동을 막음
        }
    }

    // 이동 방향에 따라 뒤집기
    if (velocity.x > 0)
        isFlip = false;
    else if (velocity.x < 0)
        isFlip = true;

    // 이동
    Pos.x += moveDist;
}


// 사다리, 밧줄
bool Character::MoveY()
{
    KeyManager* km = KeyManager::GetInstance();
    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    float vy = 0.f;
    if (km->IsStayKeyDown(VK_UP))       vy = -GetSpeed();
    else if (km->IsStayKeyDown(VK_DOWN)) vy = GetSpeed();
    else                                  vy = 0.f;


    if ((vy < 0 && isTouchingTop) || (vy > 0 && isTouchingBottom)) {
        return false;
    }

    if (CanGoY(vy)) {
        SetYVelocity(vy);
        Pos.y += velocity.y * TimeDelta;
        return true;
    }

    return false;
}

bool Character::CanGoY(float vy)
{
    // 사다리 & 밧줄의 다음 타일을 갈 수 있느냐 검사


    vector<GameObject*> inCircleObjects;

    FPOINT center = Pos;
    center.y += vy < 0 ? -interactionOffset : interactionOffset;
    CollisionManager::GetInstance()->GetObjectsInCircle(center, interactionRadius, &inCircleObjects);


    //사다리 or 밧줄인지 검사
   /* for (auto it : inCircleObjects)
    {
        if(*it  == )
        {


            inCircleObjects.clear();
            inCircleObjects.shrink_to_fit();
            return true;
         }
    }
*/


    return false;

}
// Source (Character.cpp)에 정의
bool Character::GetIsHangOn() const
{
    return isHangOn;
}

void Character::SetIsHangOn(bool value)
{
    isHangOn = value;
}
