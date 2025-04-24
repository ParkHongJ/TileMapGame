#include "pch.h"
#include "Character.h"
#include "ImageManager.h"
#include "CollisionManager.h"
#include "Image.h"
#include "CharacterState.h"
#include "CameraManager.h"
#include "ObjectManager.h"
#include "Collider.h"

// Add JunYong
#include "PlayerStatus.h"
#include "Bomb.h"
#include "Whip.h"
#include "RopeController.h"


IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);

HRESULT Character::Init()
{
    playerFaintEffect = playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");
    objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };

    state =  &Character::idleState;
    state->Enter(this);

    objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
  
    SetPos({ 200 / 2,0});

    // Stat

    health = 3;
    velocity = { 0.0f, 0.0f };
    gravity = 1000.f; 
    maxFallSpeed = 800.f; 
    jumpPower = 500.0f;


	// Animation
	frameTime = 0.0f;
	currFrameInd = { 0,0 };
	currFrameInfo = { { 0,0 }, {0, 0} };

    // Time lvalues
    currLockTime = 0.0f;
    lookUpLockTime = 1.0f;
    lookDownLockTime = 1.0f;
    
    currfaintTime = 0.0f;
    currFaintFrameInd = {0, 13} ;
    currFaintFrameInfo = { {0, 13},{11, 13}, AnimationMode::Loop };
    maxFaintTime = 5.0f;

	// Collision
	colliderSize = { 30.0f, 40.0f };
	colliderOffsetY = 0.f;
	collider = new BoxCollider(
		{ 0.0f , colliderOffsetY },     // Offset
		{ colliderSize.x, colliderSize.y },  // 
		CollisionMaskType::PLAYER, this
	);

	isTouchingLeft = false;
	isTouchingRight = false;
	isTouchingTop = false;
	isTouchingBottom = false;

	bottomHitDist = 10000.0f;

	// Interaction
	leftHandPos = { Pos.x - colliderSize.x / 2, Pos.y };
	rightHandPos = { Pos.x + colliderSize.x / 2, Pos.y };
	targetHangOnPos = { 0.f, 0.f };
	interActionPQ = {};
	interactionRadius = 1.f;
	interactionOffset = 30.f;

	// settings
	speed = 300.f;
	attackSpeed = 3.0f;
	attackRate = 0.3f;


    faintBounceTime = 0;

    // boolean
    isFlip = false;
    isAttacking = false;
    isCrouching = false;
    isLookUpLocked = false;
    isLookDownLocked = false;
    isFaint = false;
    isMovingAuto = false;

	InitAnimationMap();

	// Add Junyong
	playerStatus = new PlayerStatus();
	holdItem = nullptr;
	backItem = nullptr;
	preHoldItem = nullptr;

	whip = new Whip();
	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, whip);

    ObjectManager::GetInstance()->SetPlayer(this);
    objectRenderId = RENDER_PLAYER;

	return S_OK;
}


void Character::InitCharacter(string input)
{
    playerFaintEffect = playerImage = ImageManager::GetInstance()->FindImage(input);
    whip->SetImage(input);
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
	{ {0, 1}, {2, 1}, AnimationMode::FreezeAtX };

	animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_STOP)}] =
	{ {2, 1}, {2, 1}, AnimationMode::Hold };

    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_LOOKDOWN_RELEASE)}] =
    { {2, 1}, {4, 1}, AnimationMode::Hold };

	animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_FALL_ALMOST)}] =
	{ {0, 3}, {7, 3}, AnimationMode::Loop };

    
    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_FALL_FROM_HEIGHT)}] =
    { {0, 2}, {3, 2}, AnimationMode::Hold }; // �������� �߶� & ���� �ǰ�


    animationMap[{IDLESTATE, static_cast<int>(IdleState::SubState::IDLE_FAINT)}] =
    { {9, 0}, {9, 0}, AnimationMode::Hold }; // ���� (������ ���帲) 1������



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

	animationMap[{MOVESTATE, static_cast<int>(MoveState::SubState::MOVE_HANGON_AUTO)}] =
	{ {7, 2}, {10, 2}, AnimationMode::Hold };

    // ATTACK
    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_WHIP)}] =
    { {0, 4}, {5, 4}, AnimationMode::Hold };

    animationMap[{ATTACKSTATE, static_cast<int>(AttackState::SubState::ATTACK_ITEM_THROW)}] =
    { {6, 4},  {10, 4}, AnimationMode::Hold };

	// INTERACTION

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_CLIMB_LADDER)}] =
    { {0, 6},  {5, 7}, AnimationMode::Loop };

	animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_CLIMB_ROPE)}] =
	{ {0, 7},  {9, 7}, AnimationMode::Loop };

	animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_HANGON_TILE)}] =
	{ {8, 3},  {11, 3}, AnimationMode::Hold };

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_PUSH_TILE)}] =
    { {6, 6},  {11, 6}, AnimationMode::Loop };

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_ENTER_TUNNEL)}] =
    { {0, 5},  {5, 5}, AnimationMode::Loop };

    animationMap[{INTERACTIONSTATE, static_cast<int>(InteractionState::SubState::INTERACTION_EXIT_TUNNEL)}] =
    { {6, 5},  {11, 5}, AnimationMode::Loop };

    // SubAnim (���� �����ؾ� �� �ʿ䰡 �ִ� �ִϸ��̼�)

    animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}] =
    { {0, 9}, {3, 9}, AnimationMode::FreezeAtX }; // ��� ������
    animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}] =
    { {4, 9}, {7, 9}, AnimationMode::FreezeAtX }; // �ϰ� ������
    //
    //animationMap[{IDLESTATE, static_cast<int>(SubAnim::HURT_BIRD)}] =
    //{ {0, 13}, {11, 13}, AnimationMode::Loop }; // �� �Ӹ����� ����ٴ�
}

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

void Character::Release()
{
	if (playerImage)
	{
		// ImageManager::GetInstance()->DeleteImage("Tae_Player"); �ٸ������� ������ �־ �ּ��߾
		playerImage = nullptr;

    }
    // Add JunYong
    if (playerStatus)
    {
        delete playerStatus;
        playerStatus = nullptr;
    }
}

void Character::HandleInput()
{
	KeyManager* km = KeyManager::GetInstance();

	currInput = {};
	currInput.moveLeft = km->IsStayKeyDown(VK_LEFT);
	currInput.moveLeftReleased = km->IsOnceKeyUp(VK_LEFT);

	currInput.moveRight = km->IsStayKeyDown(VK_RIGHT);
	currInput.moveRightReleased = km->IsOnceKeyUp(VK_RIGHT);

	currInput.moveUp = km->IsStayKeyDown(VK_UP);
	currInput.moveUpReleased = km->IsOnceKeyUp(VK_UP);

	currInput.moveDown = km->IsStayKeyDown(VK_DOWN);
	currInput.moveDownReleased = km->IsOnceKeyUp(VK_DOWN);

    currInput.jump = km->IsOnceKeyDown('Z');
    currInput.attack = km->IsOnceKeyDown('X');
    currInput.interact = km->IsOnceKeyDown('A');
    currInput.bomb = km->IsOnceKeyDown('C');
    currInput.shift = km->IsStayKeyDown(VK_SHIFT);
}


void Character::Update(float TimeDelta)
{

	// Camera�� ���� ����
    JunUpdate(TimeDelta);

	auto cm = CameraManager::GetInstance();
	cm->SetTargetPos(Pos);
	cm->SetLookingState(isLookUpLocked, isLookDownLocked);

    // �ڵ� �̵� (�� �Ŵ޸���)
    if (isMovingAuto)
    {
        HandleMoveLogic();

		if (state)
			state->Update();

        PlayAnimation();

        return;
    }

	HandleInput();

    // ���� ���� �Ǵ�
    if (!isMovingAuto)
        HandleTransitions();

    // ���� ���� ������Ʈ
    if (state)
        state->Update();

    if (state == &interactionState) HandleInteractionLogic();
    else if (state == &attackState) HandleAttackLogic();
    else if (state == &moveState) HandleMoveLogic();
    else if (state == &idleState) HandleIdleLogic();

    // Collision �˻� �� ���߿� �ִ��� �Ǵ�
    CheckTileCollision();

    if (!isMovingAuto)
        Move();

    if (!(state == &interactionState &&
        interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_HANGON_TILE))
    {

        ApplyGravity(TimeDelta);
    }

    

   
    // ���� ���� �ִϸ��̼� ó��
    if (IsAirborne() && !isFallFromHeight && state != &attackState)
    {
        HandleAirAnimation();
    }

	PlayAnimation();
}

void Character::HandleTransitions()
{
    auto km = KeyManager::GetInstance();
 
    if (isFaint || isFallFromHeight)
    {
        ChangeState(&idleState);
        return;
    }

    // [3] ��ȣ�ۿ�
    CheckInterAction();

    if (currInput.jump && !IsAirborne())
    {
        
        Jump();
        return;
    }

    // [1] �Ŵ޸� ����
    if (state == &interactionState &&
        interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_HANGON_TILE)
    {
        if (currInput.jump)
        {
            velocity.y = -jumpPower;
            ChangeState(&idleState);
            return;
        }

       
        // �Ŵ޸� �߿��� ���� �����ϵ��� ���� ó�� �߰�
        if (currInput.attack && !isAttacking)
        {
            isAttacking = true;
            ChangeState(&attackState);
            return;
        }

        return; // ����/���� �� �Է� ����
    }

    // ���߿����� ���� �����ϰ� ����ó��
       if (IsAirborne() && currInput.attack && !isAttacking)
        {
            isAttacking = true;
            ChangeState(&attackState);
            return;
        }

    // [2] ���߿��� �Ŵ޸� �� �ִ��� �˻�
    if (IsAirborne() && !isAttacking &&
        !(state == &interactionState) && !(interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_LADDER)
        ) {
        HangOnTile();
    }

    if (isAttacking)
    {
        if (GetCurrAnimEnd())
        {
            isAttacking = false;
        }
        else
        {
            return; // ���� ���� ���̸� �ٸ� ���� ���� �� ��
        }
    }

    // [4] ����
    if (currInput.attack)
    {
        isAttacking = true;
        ChangeState(&attackState);
        return;
    }

    

    // [5] �̵�
    if (!(state == &interactionState) &&
        (currInput.moveLeft || currInput.moveRight))
    {
        ChangeState(&moveState);
        return;
    }

    // [6] �⺻ ����
    if (!(state == &interactionState))
        ChangeState(&idleState);

    // [7] ����ó��
    if (IsAirborne() && !isAttacking &&
        !(state == &interactionState) && !(interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_LADDER)
        ) {
        HangOnTile();
    }
}


void Character::HandleIdleLogic() {
	IdleState* idle = dynamic_cast<IdleState*>(state);

    if (!idle) return;

	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
    interActionPQ = {};

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
		isCrouching = true;
		break;

	case IdleState::SubState::IDLE_LOOKDOWN_RELEASE:
		currLockTime = 0.0f;
		isLookDownLocked = false;
		isCrouching = false;
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
    case IdleState::SubState::IDLE_FALL_FROM_HEIGHT:
      
        if (!IsAirborne())
        {
         
            if (faintBounceTime == 0)
            {
                velocity.y = -200.f;
          
                if(currFrameInd.x <= 1)
                currFrameInd.x++;
            }
             else if (faintBounceTime == 1)
            {
                velocity.y = -70.f;
           
                if (currFrameInd.x <= 3)
                currFrameInd.x++;


            }
            else
            {
                isFallFromHeight = false;
                velocity.y = 0.f;
                isFaint = true;
             }
            faintBounceTime++;
        
        }
        
        break;
    case IdleState::SubState::IDLE_FAINT:
       
        frameTime += deltaTime;

        currfaintTime += deltaTime;

        if (currfaintTime >= maxFaintTime)
        {
            currfaintTime = 0.f;
            frameTime = 0.f;
            isFaint = false;
            ChangeState(&idleState);
        }

        if (frameTime < 0.075f)
        {
            return;
        }

        frameTime = 0.f;
       
        currFaintFrameInd.x++;

        if (currFaintFrameInd.x > currFaintFrameInfo.endFrame.x)
            currFaintFrameInd.x = currFaintFrameInfo.startFrame.x;
        break;

        
    case IdleState::SubState::IDLE_DIE:
        break;
    }
}

void Character::HandleMoveLogic() {
	MoveState* move = dynamic_cast<MoveState*>(state);
	if (!move) return;

	float deltaTime = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	currLockTime = 0.0f;
	isLookDownLocked = false;
	isLookUpLocked = false;

    interActionPQ = {};

	// ���� ������¿� ���� ó��
	switch (move->GetCurrentSubState()) {
	case MoveState::SubState::MOVE_LOOKDOWN_START:

		break;

	case MoveState::SubState::MOVE_LOOKDOWN_RELEASE:
		isCrouching = false;
		break;
	case MoveState::SubState::MOVE_LOOKDOWN_LOOP:
		isCrouching = true;
		if (CheckAlmostFall())
		{
			isMovingAuto = true;

            targetHangOnPos = GetHangOnTargetPos();  // ��ǥ ��ġ ���
            //velocity = { 0.f, 0.f }; // �ߴ�
        }
        break;

		break;
	case MoveState::SubState::MOVE_ALONE:
		break;
	case MoveState::SubState::MOVE_HANGON_AUTO:
	{
		float dx = targetHangOnPos.x - Pos.x;
		float dy = targetHangOnPos.y - Pos.y;

		// X�� ���� ����
		if (fabs(dx) > TOLERANCE)
		{
			float moveX = CHARACTER_MOVE_SLOW_SPEED * deltaTime * (dx > 0 ? 1.0f : -1.0f);
			Pos.x += moveX;

			// ��ǥ�� ����ģ ��� ��ġ ����
			if ((dx > 0 && Pos.x > targetHangOnPos.x) || (dx < 0 && Pos.x < targetHangOnPos.x))
				Pos.x = targetHangOnPos.x;
		}
		// Y�� ����
		else if (fabs(dy) > TOLERANCE)
		{
			float moveY = CHARACTER_MOVE_SLOW_SPEED * deltaTime * (dy > 0 ? 1.0f : -1.0f);
			Pos.y += moveY;

            if ((dy > 0 && Pos.y > targetHangOnPos.y) || (dy < 0 && Pos.y < targetHangOnPos.y))
                Pos.y = targetHangOnPos.y;
        }
        else
        {
            // ��ǥ ���� ��
            isFlip = !isFlip;
            isMovingAuto = false;
            isCrouching = false;
            ChangeState(&interactionState);
            
        }
        break;
    }
    }
}

void Character::HandleAttackLogic() {
	AttackState* attack = dynamic_cast<AttackState*>(state);
	if (!attack) return;
    interActionPQ = {};

    switch (attack->GetCurrentSubState()) {
    case AttackState::SubState::ATTACK_WHIP:
        // TODO : �ִϸ��̼� �����Ӻ��� collision �˻�, ä�� ������ render
        whip->SetHoldItemPos(Pos, isFlip);
        whip->Use(&currFrameInd.x);
        
        if (( GetCurrAnimEnd()) )
        {
            isAttacking = false;
            ChangeState(&idleState);
        }
        break;

	case AttackState::SubState::ATTACK_ITEM_THROW:

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
        MoveY();
        //if (!MoveY()) 
            //ChangeState(&moveState);
        break;
    case InteractionState::SubState::INTERACTION_HANGON_TILE:
        break;
    case InteractionState::SubState::INTERACTION_PUSH_TILE:
        if (isFlip)
        {
            if (!CheckCanPushTile() || currInput.moveLeftReleased)
            {
                pushDelay = maxPushDelay;
                ChangeState(&idleState);
            }
        }
        else
        {
            if (!CheckCanPushTile() || currInput.moveRightReleased)
            {
                pushDelay = maxPushDelay;
                ChangeState(&idleState);
            }
        }
        //Move();
        break;

	}
}

void Character::Jump()
{
    faintBounceTime = 0;
    isFaint = false;
    isFallFromHeight = false;
    velocity.y = -jumpPower;
    isCrouching = false;
    isTouchingBottom = false;
    ChangeState(&idleState);
    return;
}


void Character::HangOnTile()
{
    if (CheckHangOn())
    {
        velocity.y = 0.f;
        ChangeState(&interactionState);
        return;
    }
}


void Character::OnDamage()
{


}


void Character::HandleAirAnimation()
{
    const int AIR_ANIM_ROW = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y;
    const auto& frame = currFrameInfo;

    

    if (IsAirborne() && !isAttacking)
    {
        if (frame.startFrame.y != AIR_ANIM_ROW)
        {
            if (velocity.y < 0)
            {
                currFrameInd = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame;
                frameTime = 0.f;

                currFrameInfo = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}];
            }
            else
            {
                currFrameInd = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame;
                frameTime = 0.f;

                currFrameInfo = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}];
            }
        }
        else if (frame.startFrame.x == animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x&&
            frame.endFrame.x == animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y&&
            velocity.y >= 0)
        {
            currFrameInd = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame;
            frameTime = 0.f;

            currFrameInfo = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}];
        }
    }
    else if (frame.startFrame.y == AIR_ANIM_ROW)
    {
        // �����ϸ� ���� ������
        if (state)
            state->Enter(this);
    }
}

bool Character::CheckAlmostFall()
{
    float edgeCheckOffset = 3.f; // ĳ���� �� ��ġ ���� ���� ������ �˻�

	FPOINT footLeft;
	FPOINT footRight;

    if (isFlip)
    {
        footLeft = { Pos.x - 5.f , Pos.y + colliderSize.y / 2 + colliderOffsetY };
        footRight = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
    }
    else
    {
        footLeft = { Pos.x + 5.f , Pos.y + colliderSize.y / 2 + colliderOffsetY };
        footRight = { Pos.x + colliderSize.x / 2 , Pos.y + colliderSize.y / 2 + colliderOffsetY };
    
    }
    
    RaycastHit hitLeft, hitRight;

    bool isGroundLeft = CollisionManager::GetInstance()->RaycastType({ footLeft, {0.f, 1.f} }, 10.f, hitLeft, CollisionMaskType::TILE,true , 1.0f );
    bool isGroundRight = CollisionManager::GetInstance()->RaycastType({ footRight, {0.f, 1.f} }, 10.f, hitRight,CollisionMaskType::TILE, true , 1.0f);
    
    if (isGroundLeft == false && isGroundRight == false) return true;
    else return false;
}

bool Character::CheckHangOn()
{
    float maxHangDist = 8.0f;
    float debugTime = 1.0f;
    bool debugDraw = true;

	RaycastHit hitLeft, hitRight;
	bool leftHang = false, rightHang = false;

    leftHandPos = { Pos.x - colliderSize.x - 10.f , Pos.y - colliderSize.y / 2 + 5.f };
    rightHandPos = { Pos.x + colliderSize.x + 10.f , Pos.y - colliderSize.y /2 + 5.f };

    Ray leftRay = { leftHandPos, { 0.f, 1.f } };
    if (CollisionManager::GetInstance()->RaycastType(leftRay, maxHangDist, hitLeft, CollisionMaskType::TILE,debugDraw, debugTime))
    {
        if (hitLeft.hit && isFlip)
        {
            // ���� Ÿ�Ϻ��� ���� ���� ���� �Ŵ޸�
            if (leftHandPos.y < hitLeft.point.y - 1.0f) // -1.0f�� ����
                leftHang = true;
        }
    }

    Ray rightRay = { rightHandPos, { 0.f, 1.f } };
    if (CollisionManager::GetInstance()->RaycastType(rightRay, maxHangDist, hitRight,CollisionMaskType::TILE, debugDraw, debugTime))
    {
        if (hitRight.hit && !isFlip)
        {
            if (rightHandPos.y < hitRight.point.y - 1.0f)
                rightHang = true;
        }
    }

	return leftHang || rightHang;
}

bool Character::CheckCanPushTile()
{
    float maxPushDist = 2.0f;
    float debugTime = 1.0f;
    bool debugDraw = true;

    RaycastHit hitLeft, hitRight;
    Ray leftRay = { {Pos.x - 64.f, Pos.y }, {-1.0f, 0.f} };
    Ray rightRay = { {Pos.x + 64.f, Pos.y}, {1.0f, 0.f} };

    if (isFlip)
    {   
        //ȫ�� ����
        if (CollisionManager::GetInstance()->RaycastType(leftRay, maxPushDist, hitLeft, CollisionMaskType::TILE, true, 1.0f))
        {
            GameObject* hitObject = hitLeft.collider->GetOwner();
            if (hitObject->GetObjectName() == OBJECTNAME::HEAVYBLOCK)
            {
                interactionObject = hitObject;
                return true;
            }
        }
    }
    else
    {
        if (CollisionManager::GetInstance()->RaycastType(rightRay, maxPushDist, hitRight, CollisionMaskType::TILE, true, 1.0f))
        {
            GameObject* hitObject = hitRight.collider->GetOwner();
            if (hitObject->GetObjectName() == OBJECTNAME::HEAVYBLOCK)
            {
                interactionObject = hitObject;
                return true;
            }
        }
    }
    return false;
}

bool Character::CheckCanClimbLadder()
{
    OutputDebugStringA("==================��ٸ� �˻���=========================");

    CollisionManager::GetInstance()->GetInteractObjectsInCircle(this, interactionRadius, interActionPQ);

    if (!interActionPQ.empty())
    {
        if (interActionPQ.top().second->GetObjectName() == OBJECTNAME::LADDER || interActionPQ.top().second->GetObjectName() == OBJECTNAME::ROPE)
        {
            return true; 
        }
    }

    return false;
}

//bool Character::CheckCanClimbRope()
//{
//    OutputDebugStringA("==================��ٸ� �˻���=========================");
//
//    CollisionManager::GetInstance()->GetInteractObjectsInCircle(this, interactionRadius, interActionPQ);
//
//    if (!interActionPQ.empty())
//    {
//        if (interActionPQ.top().second->GetObjectName() == OBJECTNAME::ROPE)
//        {
//            return true;
//        }
//        return false;
//    }
//}

void Character::CheckInterAction()
{
    if (currInput.moveUp && !isMovingAuto)
    {
        // ��ٸ� ��ȣ�ۿ� �� ���� ����
        if (currInput.jump && !IsAirborne())
        {
            Jump();
            return;
        }
        if (!CheckHangOn())
        {
            if ((CheckCanClimbLadder()))
            {
                ChangeState(&interactionState);
                return;
            }
        /*    else if (CheckCanClimbRope())
            {
                ChangeState(&interactionState);
                return;
            }*/
            else
            {
                ChangeState(&idleState);
                return;
            }
        }        
    }
   /* else if (currInput.interact)
    {
        CollisionManager::GetInstance()->GetInteractObjectsInCircle(this, interactionRadius, interActionPQ);
        if (!interActionPQ.empty())
        {
            if (interActionPQ.top().second->GetObjectName() == OBJECTNAME::DOOR)
            {
            
                ChangeState(&interactionState);
                return;
            }
        }
    }*/

    if (isTouchingBottom)
    {
        if (CheckCanPushTile() && (currInput.moveLeft || currInput.moveRight))
        {
            float timeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");
            
            pushDelay -= timeDelta;
            
            // ��¥ �б�
            if (pushDelay > 0.f)
            {
                interactionObject = nullptr;
                return;
            }

            // TODO : Push tile from tilemap?
            
            if (interactionObject != nullptr)
            {
                FPOINT dir;
                float pushStrength = 30.f;

                if (isFlip)
                {
                    //����
                    dir = { -1.f,0.f };
                }
                else
                {
                    //������
                    dir = { 1.f,0.f };
                }

                if (interactionObject->OnPush(dir, pushStrength))
                {
                    Pos.x += dir.x * pushStrength * timeDelta;
                    interactionObject = nullptr;
                }
            }

            ChangeState(&interactionState);
            return;
        }
        else
        {
            pushDelay = maxPushDelay;
        }
    }
}

void Character::Detect(GameObject* obj)
{
	//if (obj == preHoldItem)
	//{
	//    return;
	//}

	int i = 5;
}

void Character::JunUpdate(float TimeDelta)
{
	auto km = KeyManager::GetInstance();

	// Add JunYong
	if (km->IsOnceKeyDown('F'))
	{
		if (0 < playerStatus->GetBombCount())
		{
			FPOINT offset = { 50,0 };
			float angle = 25.f;
			if (true == isFlip)
			{
				offset.x *= -1;
				angle = 155.f;
			}

			if (currInput.moveDownReleased || currInput.moveDown)
			{
				Bomb* temp = new Bomb();
				ObjectManager::GetInstance()->AddObject(RENDER_HOLD, temp);
				temp->SetPos({ Pos.x /*+ offset.x*/, Pos.y + colliderOffsetY });
				temp->UnEquip(playerStatus);
				//temp->SetDrop(0.f, angle);
			}

			else
			{
				Bomb* temp = new Bomb();
				ObjectManager::GetInstance()->AddObject(RENDER_HOLD, temp);
				temp->SetPos(Pos + offset);
				temp->SetDrop(1000.f, angle, 0.3f, {0.f, 198.f});
				temp->UnEquip(playerStatus);
			}

			playerStatus->MinusBombCount();
		}

	}

	if (km->IsOnceKeyDown('L'))
	{
		if (holdItem)
		{
			FPOINT offset = { 50,0 };
			float angle = 25.f;
			if (true == isFlip)
			{
				offset.x *= -1;
				angle = 155.f;
			}

			holdItem->UnEquip();

			if (!currInput.moveDownReleased && !currInput.moveDown)
			{
				holdItem->SetDrop(600.f, angle);
			}

			else
			{
				holdItem->SetDrop(0.f, angle);
			}

			preHoldItem = holdItem;
			holdItem = nullptr;
		}

		else
		{
			vector<GameObject*> inCircleObjects;

			FPOINT center = Pos;
			CollisionManager::GetInstance()->GetObjectsInCircle(center, interactionRadius, &inCircleObjects);
			for (auto& iter : inCircleObjects)
			{
				if (nullptr != dynamic_cast<Item*>(iter)) // Test
					//if (OBJECTNAME::GUN == (iter)->GetObjectName()) // Test
				{
					holdItem = dynamic_cast<Item*>(iter);
					holdItem->Equip();
				}
			}
		}

	}

	if (km->IsOnceKeyDown('V'))
	{
		if (holdItem)
		{
			holdItem->Use();
		}
	}

    if (km->IsOnceKeyDown('I'))
    {
        if (0 < playerStatus->GetRopeCount())
        {
            RopeController* temp = new RopeController();
            ObjectManager::GetInstance()->AddObject(RENDER_HOLD, temp);
            temp->Shoot(Pos);
            playerStatus->MinusRopeCount();
        }
    }
    
	if (holdItem)
	{
		holdItem->SetHoldItemPos(Pos, isFlip);
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
    if (IsAirborne() && !isFallFromHeight && !isAttacking)
    {
        float vel = velocity.y;

        const int JUMP_UP_START = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x;
        const int JUMP_UP_END = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_UP)}].endFrame.x;
        const int JUMP_DOWN_START = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame.x;
        const int JUMP_DOWN_END = animationMap[{SUBSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].endFrame.x;

		float ratio = 0.f;

		if (vel < 0) // ��� ��
		{
			ratio = -vel / jumpPower;
			if (ratio < 0.f) ratio = 0.f;
			if (ratio > 1.f) ratio = 1.f;

			int frame = static_cast<int>(JUMP_UP_START + ratio * (JUMP_UP_END - JUMP_UP_START));
			currFrameInd.x = frame;
		}

		else // �ϰ� ��
		{
			ratio = vel / jumpPower;
			if (ratio < 0.f) ratio = 0.f;
			if (ratio > 1.f) ratio = 1.f;

            int frame = static_cast<int>(JUMP_DOWN_START + ratio * (JUMP_DOWN_END - JUMP_DOWN_START));
            currFrameInd.x = frame;
        }
    }
    else
    {
        // ��ٸ� ��ȣ�ۿ�
        bool isClimbing =
            state == &interactionState &&
            (interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_LADDER ||
                interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_ROPE);

        bool hasVerticalInput = currInput.moveUp || currInput.moveDown;

        if (isClimbing && !hasVerticalInput )
            return; // �Է��� ������ ������ ���� �ߴ�

        if (isFallFromHeight) return;


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
}

void Character::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

    if (state)
    {
        char buf[256];
        sprintf_s(buf,
            "�� Render Frame: (%d,%d)\n�� State: %s  \n isFallFromHeight : %d isFaint: %d currfaintTime : %f Velocity : x = %f y = %f",
            currFrameInd.x, currFrameInd.y, state->GetSubStateName(), isFallFromHeight, isFaint,currfaintTime, velocity.x, velocity.y
            );

		OutputDebugStringA(buf);
	}

    if (playerImage)
    {
        playerImage->FrameRender(renderTarget, pos.x, pos.y, currFrameInd.x, currFrameInd.y, objectScale.x, objectScale.y, isFlip);
    }
    if (playerFaintEffect)
    {
        if (isFaint )
        {
            playerFaintEffect->FrameRender(renderTarget, pos.x, pos.y - 20.f, currFaintFrameInd.x, currFaintFrameInd.y, objectScale.x, objectScale.y );
        }
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

void Character::ApplyGravity(float TimeDelta)
{
    if (IsAirborne() && !isTouchingBottom)
    {
        float fallDist = velocity.y * TimeDelta;

        // �߷� ����
        Pos.y += fallDist;
        velocity.y += gravity * TimeDelta;
        if (velocity.y > maxFallSpeed)
            velocity.y = maxFallSpeed;
    }
    else if(!IsAirborne())
    {

        // ���� �Ǵ� �� ��ġ ����
        //if (isTouchingBottom && bottomHitDist < 2.0f)
        //{
        //    // �ٴ� ���� ��ġ ����
        //    Pos.y += bottomHitDist - colliderOffsetY + colliderSize.y / 2.f;

            // ��ġ ������ ���ÿ� ���� ����

            if (velocity.y > 1000.f)
            {
               isFallFromHeight = true;
               ChangeState(&idleState);
            }
        /*}*/

            velocity.y = 0.f;

    }
}


void Character::CheckTileCollision()
{
	float maxDist = 10.0f;
	float debugTime = 1.0f;

	// Collider ���� 
	FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
	FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
	FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
	FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };

	RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
	RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

	bool onDebug = false;

 // RayAll -> RayType : Ÿ�ϸ� ���� ��� �ϴ� �ڵ�� ����
    isTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE , onDebug, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::TILE, onDebug, debugTime);

    isTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, onDebug, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::TILE, onDebug, debugTime);

    isTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::TILE, onDebug, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::TILE, onDebug, debugTime);

    isTouchingBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, onDebug, debugTime) ||
        CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, CollisionMaskType::TILE, onDebug, debugTime);
    
     if (velocity.y < 0.f)
        isTouchingBottom = false;

    if (hitBottom1.hit && hitBottom2.hit)
        bottomHitDist = min(hitBottom1.distance, hitBottom2.distance);
    else if (hitBottom1.hit)
        bottomHitDist = hitBottom1.distance;
    else if (hitBottom2.hit)
        bottomHitDist = hitBottom2.distance;
}

void Character::Move()
{
    if ((state == &interactionState &&
        interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_HANGON_TILE))
    {
        return;
    }

    if ((state == &idleState && idleState.GetCurrentSubState() == IdleState::SubState::IDLE_FAINT))
    {
        return;
    }
    if ((state == &interactionState &&
        interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_LADDER))
    {
        return;
    }


	float vx = 0.f;
	if (currInput.moveLeft)       vx = -GetSpeed();
	else if (currInput.moveRight) vx = GetSpeed();
	else                                  vx = 0.f;

	SetXVelocity(vx);

    if (!IsAirborne()) {
        if (isCrouching || currInput.shift || CheckCanPushTile())
            SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
        else
            SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);  
    }
    else {
        SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED); // ���߿����� �⺻ �ӵ�
    }

	float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	float moveDist = velocity.x * TimeDelta;

    // ���߿��� ���� �ε����� �� �̵��� ����
    if (IsAirborne()) {
        if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight) ) {
            velocity.x = 0.f;  // ���߿����� �̵��� ����
            moveDist = 0.f;    // �̵����� 0���� ����
        }
        if ( (velocity.y < 0 && isTouchingTop)  ) {
            velocity.y = 0.f;  // ���߿����� �̵��� ����
            moveDist = 0.f;    // �̵����� 0���� ����
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


// ��ٸ�, ����
bool Character::MoveY()
{
    float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

    float vy = 0.f;
    if (currInput.moveUp)       vy = -GetSpeed();
    else if (currInput.moveDown) vy = GetSpeed();
    else                                vy = 0.f;

	 if ((vy < 0 && isTouchingTop) || (vy > 0 && isTouchingBottom)) {
		 return false;
	 }

    if (CanGoY(vy)) {

        velocity.y = vy;
        Pos.y += velocity.y * TimeDelta;
        return true;
    }

	return false;
}

bool Character::CanGoY(float vy)
{
    // ��ٸ� & ������ ���� Ÿ���� �� �� �ִ��� �˻�
    vector<GameObject*> inCircleObjects;

    FPOINT center = Pos;
    center.y += vy < 0 ? -interactionOffset : interactionOffset;
    CollisionManager::GetInstance()->GetObjectsInCircle(center, interactionRadius, &inCircleObjects);

    //��ٸ� or �������� �˻�
    for (auto it : inCircleObjects)
    {
        if(it->GetObjectName() == OBJECTNAME::LADDER ||
           it->GetObjectName() == OBJECTNAME::ROPE )
        {
            Pos.x = it->GetPos().x; // ��ٸ� �߾� ����

            inCircleObjects.clear();
            inCircleObjects.shrink_to_fit();
            return true;
         }
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


bool Character::GetIsMovingAuto() const
{
	return isMovingAuto;
}


FPOINT Character::GetHangOnTargetPos()
{
	if (isFlip)
		return { Pos.x - colliderSize.x , Pos.y + colliderSize.y + 20.f }; // ���� ��
	else
		return { Pos.x + colliderSize.x , Pos.y + colliderSize.y + 20.f }; // ������ ��
}

bool Character::IsAirborne() const
{
    return !isTouchingBottom &&
        !(state == &interactionState &&
            (interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_HANGON_TILE ||
                interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_LADDER ||
                interactionState.GetCurrentSubState() == InteractionState::SubState::INTERACTION_CLIMB_ROPE));
}
