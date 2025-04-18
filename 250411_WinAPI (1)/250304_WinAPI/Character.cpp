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

    dir = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };


    // Render
    frameTime = 0.0f;
    currFrameInd = { 0,0 };
    currFrameInfo = { { 0,0 }, {0, 0} };
    
    jumpFrameInfo = { {0, 9}, {7, 9} };
    attackFrameInfo = { {10, 12}, {15,12} };
    ropeFrameInfo = { {0, 12}, {9,12} };

    //colliderRect = {-40, -40, 40, 40};
    //colliderRect = {-10, -10, 10, 10};
    //colliderRect = { -10, -20, 10, 20 }; // width: 20, height: 40

    yellowCollider = new BoxCollider(
        { 0.0f , 0.0f },     // Offset
        { 50.0f, 180.0f },  // 
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

    // HFSM
    if (state)
        state->Update();


    // ---- 변경된 애니메이션 처리 로직 시작 ----

    const int AIR_ANIM_ROW = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y;
    const auto& frame = currFrameInfo;

    // 점프 중이면
    if (isInAir)
    {
        // 현재 프레임이 공중 프레임이 아닐 경우 (점프 직후)
        if (frame.startFrame.y != animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y)
        {
            if (velocity.y < 0)
                SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP));
            else
                SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN));
        }
        // 이미 JUMP_UP 상태에서 낙하로 전환되는 시점
        else if (frame.startFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x && 
                 frame.endFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y && 
                 velocity.y >= 0)
        {
            SetAnimationFrameInfo(ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN));
        }
    }
    else if (frame.startFrame.y == AIR_ANIM_ROW)
    {
        // 착지한 경우에만 상태 재진입 (프레임 초기화)
        if (state)
            state->Enter(this);
    }

    // ---- 변경된 애니메이션 처리 로직 끝 ----



    PlayAnimation();

    Move();

    ApplyGravity(TimeDelta);

    auto cm = CameraManager::GetInstance();
    cm->SetTargetPos(Pos);

    cm->SetLookingState(isLookUpLocked, isLookDownLocked);

}


void Character::InitAnimationMap()
{
     // IDLE
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_ALONE)}] =
    { {0, 0}, {0, 0}, AnimationMode::Hold };

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
    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_ALONE)}] =
    { {0, 4}, {5, 4}, AnimationMode::Hold };



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

    if (frameTime < ANIMATION_FRAME_TIME) return;
   
    frameTime = 0.f;

    // 공중 애니메이션일 경우
    if (currFrameInfo.startFrame.y == 9)
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
        Pos.y += velocity.y * TimeDelta;
        velocity.y += gravity * TimeDelta;

        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;
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
    float debugTime = 0.0f;

    // Collider 기준 100x100일 때
    FPOINT leftTop = { Pos.x - 45.f, Pos.y - 30.f };
    FPOINT rightTop = { Pos.x + 45.f, Pos.y - 30.f };
    FPOINT leftBottom = { Pos.x - 30.f, Pos.y + 45.f };
    FPOINT rightBottom = { Pos.x + 30.f, Pos.y + 45.f };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;


    isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime);

    isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime);

    isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, true, debugTime);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, true, debugTime) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, true, debugTime);
}

void Character::Move()
{
    // 이동 방향에 따라 충돌 여부 확인
    if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight))
        return;

    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    if (velocity.x > 0)
        isFlip = false;
    else if (velocity.x < 0)
        isFlip = true;

    Pos.x += velocity.x * TimeDelta;
}
