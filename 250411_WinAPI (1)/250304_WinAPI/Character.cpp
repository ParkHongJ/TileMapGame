#include "Character.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CharacterState.h"
#include "CameraManager.h"
#include "Collider.h"


IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);

HRESULT Character::Init()
{
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");

    state = &Character::idleState;
    state->Enter(this);
  
    SetPos({ WINSIZE_X / 2,0});

    dir = { 0.0f, 0.0f };
    velocity = { 0.0f, 0.0f };


    gravity = 1000.f; // �߷� ���ӵ� (�ȼ�/sec^2)
    maxFallSpeed = 800.f; // �ִ� ���� �ӵ�
    jumpPower = 500.0f;


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

    colliderSize = 60.f;
    colliderOffset = 25.f;
    yellowCollider = new BoxCollider(
        { 0.0f , colliderOffset },     // Offset
        { colliderSize, colliderSize },  // 
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


    InitAnimationMap();

    return S_OK;
}

void Character::Release()
{
    if (playerImage)
    {
        ImageManager::GetInstance()->DeleteImage("Tae_Player");
        playerImage = nullptr;

    }
}

void Character::Update(float TimeDelta)
{
    CheckCollision();
    isInAir = !isTouchingBottom;

    KeyManager* km = KeyManager::GetInstance();


     PlayAnimation();
   
    // ���� ���� �Ǵ�
    HandleTransitions();

    // ���� ���� ������Ʈ
    if (state)
        state->Update();


    if (state == &idleState) HandleIdleLogic();
    else if (state == &moveState) HandleMoveLogic();
    else if (state == &attackState) HandleAttackLogic();


    // ���� ���� �ִϸ��̼� ó��
    HandleAirAnimation();

    ApplyGravity(TimeDelta);

    Move();
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

    // 0. ���� (���� ���̴��� ���� ����)
    if (isJump && isTouchingBottom)
    {
        velocity.y = -jumpPower;
        isInAir = true;
    }

    
    // 1. ���� 
    char buf[128];
    sprintf_s(buf, "[�����] isAttacking ����� �� %s\n", isAttacking ? "true" : "false");
    OutputDebugStringA(buf);

    if (isAttacking) return;

     if (isAttack && !isAttacking)
    {
        isAttacking = true;

        ChangeState(&attackState);

        return;
    }
    // 2. �̵�
    if (isAnyDirection && !isInAir)
    {
        ChangeState(&moveState);
        return;
    }

    // 3. ���� �ƴ� ���� idle
    if (!isAnyDirection && !isInAir)
    {
        ChangeState(&idleState);
    }

    // ���� 4. ��ȣ�ۿ� �Է� ������ ����� ���⿡ �߰�
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
        // �����ϸ� ���� ������
        if (state)
            state->Enter(this);
    }
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
        // ���� �ִϸ��̼� ���� ����, ���� ���� �ʿ� �� �߰�
        break;

    case IdleState::SubState::IDLE_LOOKUP_RELEASE:
        // ������ �ִϸ��̼� �Ϸ� �� alone���� �Ѿ�� ������ IdleState�� ����
        currLockTime = 0.0f;
        isLookUpLocked = false;
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_START:
        currLockTime += deltaTime;
        if (GetCurrAnimEnd() && currLockTime > lookDownLockTime)
            isLookDownLocked = true;
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_STOP:
        // ���� �ִϸ��̼� ���� ����
        break;

    case IdleState::SubState::IDLE_LOOKDOWN_RELEASE:
        // �ִϸ��̼��� ������ IDLE_ALONE ���� ��ȯ��
        currLockTime = 0.0f;
        isLookDownLocked = false;
        break;

    case IdleState::SubState::IDLE_ALONE:
        // �⺻ idle ����, Ư���� ���� ����
        break;

    case IdleState::SubState::IDLE_ONAIR:
        // ���߿� ���� �� ���� ó�� �ʿ� �� ���⿡
        break;

    case IdleState::SubState::IDLE_ONPET:
    case IdleState::SubState::IDLE_ONPET_LOOKUP:
    case IdleState::SubState::IDLE_ONPET_LOOKDOWN:
    case IdleState::SubState::IDLE_FALL_ALMOST:
    case IdleState::SubState::IDLE_HURT:
    case IdleState::SubState::IDLE_DIE:
        // ���� �̱��� �����̰ų� �ܺο��� ó�� �ʿ�
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


    // ���� ������¿� ���� ó��
    switch (move->GetCurrentSubState()) {
    case MoveState::SubState::MOVE_LOOKDOWN_START:
       /* currLockTime += deltaTime;
        if (GetCurrAnimEnd() && currLockTime > lookDownLockTime)
            isLookDownLocked = true;*/
        break;

    case MoveState::SubState::MOVE_LOOKDOWN_RELEASE:
     /*   isLookDownLocked = false;
        currLockTime = 0.0f;*/
        if (GetCurrAnimEnd())
            move->ChangeSubState(MoveState::SubState::MOVE_ALONE);
        break;

    case MoveState::SubState::MOVE_LOOKDOWN_LOOP:
    case MoveState::SubState::MOVE_ALONE:
        break;

    default:
        break;
    }
}


void Character::HandleAttackLogic() {
    AttackState* attack = dynamic_cast<AttackState*>(state);
    if (!attack) return;

    switch (attack->GetCurrentSubState()) {
    case AttackState::SubState::ATTACK_WHIP:
        break;

    case AttackState::SubState::ATTACK_ITEM_THROW:
        break;

    default:
        break;
    }

    // �̵� �� �߷��� ��� �ݿ�
  
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



    animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}] =
    { {0, 9}, {3, 9}, AnimationMode::FreezeAtX }; // ��� ������
    animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}] =
    { {4, 9}, {7, 9}, AnimationMode::FreezeAtX }; // �ϰ� ������


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
        OutputDebugStringA(" �ִϸ��̼� ������ �ʱ�ȭ ȣ���\n");
        // ������ �ʱ�ȭ�� ��¥�� �ٲ� ��쿡�� �ϰԲ�
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
            "�� Render Frame: (%d,%d)\n�� State: %s  \n LookDownLocked : %d Speed: %f Velocity : x = %f y = %f",
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


    // ���� �ִϸ��̼��� ���
    if (currFrameInfo.startFrame.y == 9)
    {
        float vel = velocity.y;

        const int JUMP_UP_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x;
        const int JUMP_UP_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].endFrame.x;
        const int JUMP_DOWN_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame.x;
        const int JUMP_DOWN_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].endFrame.x;

        const float MAX_JUMP_VEL = jumpPower;

        float ratio = 0.f;

        if (vel < 0) // ��� ��
        {
            ratio = -vel / MAX_JUMP_VEL;
            if (ratio < 0.f) ratio = 0.f;
            if (ratio > 1.f) ratio = 1.f;

            int frame = static_cast<int>(JUMP_UP_START + ratio * (JUMP_UP_END - JUMP_UP_START));
            currFrameInd.x = frame;
        }
        else // �ϰ� ��
        {
            ratio = vel / MAX_JUMP_VEL;
            if (ratio < 0.f) ratio = 0.f;
            if (ratio > 1.f) ratio = 1.f;

            int frame = static_cast<int>(JUMP_DOWN_START + ratio * (JUMP_DOWN_END - JUMP_DOWN_START));
            currFrameInd.x = frame;
        }

        return;
    }

    // �Ϲ� �ִϸ��̼�
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
        return; // ���°� ������ �������� ����

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
            // �� ���� �����ϵ��� �Ÿ� ����
            Pos.y += bottomHitDist - colliderOffset - 0.1f;
            velocity.y = 0.f;
            isInAir = false;
            return;
        }

        Pos.y += fallDist;
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
    float debugTime = 1.0f;

    // Collider ���� 
    FPOINT leftTop = { Pos.x - colliderSize/2, Pos.y - colliderSize/2 + colliderOffset };
    FPOINT rightTop = { Pos.x + colliderSize/2, Pos.y - colliderSize/2+ colliderOffset };
    FPOINT leftBottom = { Pos.x - colliderSize/2, Pos.y + colliderSize/2 + colliderOffset };
    FPOINT rightBottom = { Pos.x + colliderSize/2, Pos.y + colliderSize/2 + colliderOffset };

    RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
    RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;


    isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, true, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime, this);

    isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, true, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime, this);

    isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, true, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, true, debugTime, this);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, true, debugTime, this) ||
        CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, true, debugTime, this);

   

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

    // ���߿��� ���� �ε����� �� �̵��� ����
    if (GetIsInAir()) {
        if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight) ) {
            velocity.x = 0.f;  // ���߿����� �̵��� ����
            moveDist = 0.f;    // �̵����� 0���� ����
        }
        if ( (velocity.y < 0 && isTouchingTop)  ) {
            velocity.y = 0.f;  // ���߿����� �̵��� ����
          //  moveDist = 0.f;    // �̵����� 0���� ����
        }
    }
    else {
        // ���󿡼��� �̵� ��ü�� ���� (���� �ε�����)
        if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight)) {
            return;  // ���� �ε������Ƿ� �̵��� ����
        }
    }

    // �̵� ���⿡ ���� ������
    if (velocity.x > 0)
        isFlip = false;
    else if (velocity.x < 0)
        isFlip = true;

    // �̵�
    Pos.x += moveDist;
}
