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
#include "Whip.h"


IdleState Character::idleState(IdleState::SubState::NONE);
MoveState Character::moveState(MoveState::SubState::NONE);
AttackState Character::attackState(AttackState::SubState::NONE);
InteractionState Character::interactionState(InteractionState::SubState::NONE);

HRESULT Character::Init()
{
	playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");

	state = &Character::idleState;
	state->Enter(this);

	SetPos({ 200 / 2,0 });

	velocity = { 0.0f, 0.0f };
	gravity = 1000.f;
	maxFallSpeed = 800.f;
	jumpPower = 500.0f;

	// Animation
	frameTime = 0.0f;
	currFrameInd = { 0,0 };
	currFrameInfo = { { 0,0 }, {0, 0} };

	// Camera
	currLockTime = 0.0f;
	lookUpLockTime = 1.0f;
	lookDownLockTime = 1.0f;

	// Collision
	colliderSize = { 30.0f, 40.0f };
	colliderOffsetY = 25.f;
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
	interactionRadius = 25.f;
	interactionOffset = 40.f;

	// settings
	speed = 300.f;
	attackSpeed = 3.0f;
	attackRate = 0.3f;

	// boolean
	isFlip = false;
	isInAir = false;
	isAttacking = false;
	isCrouching = false;
	isLookUpLocked = false;
	isLookDownLocked = false;

	isOnLadder = false;
	isOnRope = false;
	isHanging = false;
	isMovingAuto = false;


	InitAnimationMap();

	// Add Junyong
	playerStatus = new PlayerStatus();
	holdItem = nullptr;
	backItem = nullptr;
	preHoldItem = nullptr;

	objectRenderId = RENDER_PLAYER;

	whip = new Whip();
	ObjectManager::GetInstance()->AddObject(RENDER_HOLD, whip);
	return S_OK;
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

void Character::Release()
{
	if (playerImage)
	{
		// ImageManager::GetInstance()->DeleteImage("Tae_Player"); 다른데서도 쓰던거 있어서 주석했어여
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
	currInput.shift = km->IsStayKeyDown(VK_SHIFT);
}


void Character::Update(float TimeDelta)
{

	// Camera에 정보 전달
	auto cm = CameraManager::GetInstance();
	cm->SetTargetPos(Pos);
	cm->SetLookingState(isLookUpLocked, isLookDownLocked);


	if (isMovingAuto)
	{
		HandleMoveLogic();

		if (state)
			state->Update();



		PlayAnimation();

		return;
	}


	HandleInput();


	if (!isMovingAuto)
		Move();

	if (!isHanging)
		ApplyGravity(TimeDelta);


	CheckTileCollision();

	isInAir = !isTouchingBottom;


	// 바닥 가까이일 때만 착지로 판단
	if (isTouchingBottom && velocity.y > 0 && bottomHitDist < 2.0f)
	{
		// 바닥 위로 위치 보정
		Pos.y += bottomHitDist - colliderOffsetY + colliderSize.y / 2.f;
		velocity.y = 0.f;
		isInAir = false;
		//return;
	}


	JunUpdate(TimeDelta);

	// 상태 전이 판단
	if (!isMovingAuto)
		HandleTransitions();

	// 현재 상태 업데이트
	if (state)
		state->Update();



	if (state == &interactionState) HandleInteractionLogic();
	else if (state == &attackState) HandleAttackLogic();
	else if (state == &moveState) HandleMoveLogic();
	else if (state == &idleState) HandleIdleLogic();



	// 공중 점프 애니메이션 처리
	if (!isHanging)
		HandleAirAnimation();

	PlayAnimation();
}


void Character::HandleTransitions()
{

	if (currInput.jump && !isInAir)
	{
		Jump();
	}

	// [1] 매달림 상태에서는 점프만 허용
	if (isHanging)
	{
		if (currInput.jump)
		{
			velocity.y = -jumpPower;
			isInAir = true;
			isHanging = false; // 매달림 해제
			isOnLadder = false;
			isOnRope = false;
			ChangeState(&idleState);
		}

		ChangeState(&interactionState);
		return;

	}

	// [2] 공중에서 매달릴 수 있는지 검사
	if (isInAir)
	{
		HangOnTile();
	}

	if (currInput.moveUp || isOnLadder || isOnRope)
	{

		OutputDebugStringA("==================올라탈 곳 검사중=========================");
		CollisionManager::GetInstance()->GetInteractObjectsInCircle(this, interactionRadius, interActionPQ);

		if (!interActionPQ.empty())
		{
			// 사다리, 로프 pos 로 플레이어 위치 조정
			OBJECTNAME temp = interActionPQ.top().second->GetObjectName();
			if (interActionPQ.top().second->GetObjectName() == OBJECTNAME::LADDER || interActionPQ.top().second->GetObjectName() == OBJECTNAME::ROPE)
			{
				isOnLadder = true;
				isOnRope = true;

				ChangeState(&interactionState);
				return;
			}
			//else if (interActionPQ.top() == 상인)
			//{

			//}
			//else if (interActionPQ.top() == )
			//{

			//}

		}
	}


	// 공격 
	char buf[128];
	sprintf_s(buf, "  isAttacking 변경됨 → %s\n", isAttacking ? "true" : "false");
	OutputDebugStringA(buf);

	if (isAttacking) return;

	if (currInput.attack && !isAttacking)
	{
		isAttacking = true;

		ChangeState(&attackState);

		return;
	}

	// 이동
	if ((currInput.moveLeft || currInput.moveRight))
	{
		ChangeState(&moveState);
		return;
	}

	// 유후 상태
	ChangeState(&idleState);

	if (isInAir)
	{
		HangOnTile();
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

	currLockTime = 0.0f;
	isLookDownLocked = false;
	isLookUpLocked = false;

	// 현재 서브상태에 따른 처리
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

			targetHangOnPos = GetHangOnTargetPos();  // 목표 위치 계산
			velocity = { 0.f, 0.f }; // 중단

			// 여기서는 상태 전이 보류 (이동 완료 후 시도할 것)

		}
		break;

		break;
	case MoveState::SubState::MOVE_ALONE:
		break;
	case MoveState::SubState::MOVE_HANGON_AUTO:
	{
		float dx = targetHangOnPos.x - Pos.x;
		float dy = targetHangOnPos.y - Pos.y;

		// X축 먼저 정렬
		if (fabs(dx) > TOLERANCE)
		{
			float moveX = CHARACTER_MOVE_SLOW_SPEED * deltaTime * (dx > 0 ? 1.0f : -1.0f);
			Pos.x += moveX;

			// 목표를 지나친 경우 위치 보정
			if ((dx > 0 && Pos.x > targetHangOnPos.x) || (dx < 0 && Pos.x < targetHangOnPos.x))
				Pos.x = targetHangOnPos.x;
		}
		// Y축 정렬
		else if (fabs(dy) > TOLERANCE)
		{
			float moveY = CHARACTER_MOVE_SLOW_SPEED * deltaTime * (dy > 0 ? 1.0f : -1.0f);
			Pos.y += moveY;

			if ((dy > 0 && Pos.y > targetHangOnPos.y) || (dy < 0 && Pos.y < targetHangOnPos.y))
				Pos.y = targetHangOnPos.y;
		}
		else
		{
			// 목표 도착 시
			isFlip = !isFlip;
			isMovingAuto = false;
			isHanging = true;
			isCrouching = false;
			isInAir = true;
			ChangeState(&interactionState);
		}

		break;
	}

	}



}


void Character::HandleAttackLogic() {
	AttackState* attack = dynamic_cast<AttackState*>(state);
	if (!attack) return;

	switch (attack->GetCurrentSubState()) {
	case AttackState::SubState::ATTACK_WHIP:
		whip->SetHoldItemPos(Pos, isFlip);
		whip->Use(&currFrameInd.x);
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
	case InteractionState::SubState::INTERACTION_HANGON_TILE:


		break;

	}
}

void Character::Jump()
{
	velocity.y = -jumpPower;
	isInAir = true;
	isHanging = false; // 매달림 해제
	isOnLadder = false;
	isCrouching = false;
	isOnRope = false;
	ChangeState(&idleState);
	return;
}

void Character::HangOnTile()
{
	if (CheckHangOn())
	{
		isHanging = true;
		velocity = { 0.0f, 0.0f };
		ChangeState(&interactionState);
		return;
	}
	else
		isHanging = false;
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
			{

				currFrameInd = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame;
				frameTime = 0.f;

				currFrameInfo = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}];
			}
			else
			{
				currFrameInd = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame;
				frameTime = 0.f;

				currFrameInfo = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}];
			}
		}
		else if (frame.startFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x&&
			frame.endFrame.x == animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.y&&
			velocity.y >= 0)
		{
			currFrameInd = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame;
			frameTime = 0.f;

			currFrameInfo = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}];
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

	bool isGroundLeft = CollisionManager::GetInstance()->RaycastType({ footLeft, {0.f, 1.f} }, 10.f, hitLeft, CollisionMaskType::TILE, false, 1.0f);
	bool isGroundRight = CollisionManager::GetInstance()->RaycastType({ footRight, {0.f, 1.f} }, 10.f, hitRight, CollisionMaskType::TILE, false, 1.0f);

	// 둘 중 하나만 떠 있으면 가장자리
	return (isGroundLeft ^ isGroundRight); // XOR
}

bool Character::CheckHangOn()
{
	float maxHangDist = 7.0f;
	float debugTime = 1.0f;
	bool debugDraw = true;

	RaycastHit hitLeft, hitRight;
	bool leftHang = false, rightHang = false;

	leftHandPos = { Pos.x - colliderSize.x - 10.f , Pos.y - colliderSize.y / 2 + 10.f };
	rightHandPos = { Pos.x + colliderSize.x + 10.f , Pos.y - colliderSize.y / 2 + 10.f };

	Ray leftRay = { leftHandPos, { -0.1f, 1.f } };
	if (CollisionManager::GetInstance()->RaycastType(leftRay, maxHangDist, hitLeft, CollisionMaskType::TILE, debugDraw, debugTime))
	{
		if (hitLeft.hit && isFlip)
		{
			// 손이 타일보다 위에 있을 때만 매달림
			if (leftHandPos.y < hitLeft.point.y - 1.0f) // -1.0f는 여유
				leftHang = true;
		}
	}

	Ray rightRay = { rightHandPos, { 0.1f, 1.f } };
	if (CollisionManager::GetInstance()->RaycastType(rightRay, maxHangDist, hitRight, CollisionMaskType::TILE, debugDraw, debugTime))
	{
		if (hitRight.hit && !isFlip)
		{
			if (rightHandPos.y < hitRight.point.y - 1.0f)
				rightHang = true;
		}
	}

	return leftHang || rightHang;
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



	// 공중 애니메이션일 경우
	if (currFrameInfo.startFrame.y == 9 && isInAir)
	{
		float vel = velocity.y;

		const int JUMP_UP_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].startFrame.x;
		const int JUMP_UP_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_UP)}].endFrame.x;
		const int JUMP_DOWN_START = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].startFrame.x;
		const int JUMP_DOWN_END = animationMap[{ANIMSTATE, static_cast<int>(SubAnim::JUMP_DOWN)}].endFrame.x;

		float ratio = 0.f;

		if (vel < 0) // 상승 중
		{
			ratio = -vel / jumpPower;
			if (ratio < 0.f) ratio = 0.f;
			if (ratio > 1.f) ratio = 1.f;

			int frame = static_cast<int>(JUMP_UP_START + ratio * (JUMP_UP_END - JUMP_UP_START));
			currFrameInd.x = frame;
		}
		else // 하강 중
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
}

void Character::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();

	if (state)
	{
		char buf[256];
		sprintf_s(buf,
			"▶ Render Frame: (%d,%d)\n▶ State: %s  \n ishangOn : %d isCrouching: %d Velocity : x = %f y = %f",
			currFrameInd.x, currFrameInd.y, state->GetSubStateName(), isHanging, isCrouching, velocity.x, velocity.y
		);

		OutputDebugStringA(buf);
	}

	if (playerImage)
	{
		playerImage->FrameRender(renderTarget, pos.x, pos.y, currFrameInd.x, currFrameInd.y, isFlip);
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

void Character::ApplyGravity(float TimeDelta)
{
	if (isInAir)
	{
		float fallDist = velocity.y * TimeDelta;



		// 중력 적용
		Pos.y += fallDist;
		velocity.y += gravity * TimeDelta;
		if (velocity.y > maxFallSpeed)
			velocity.y = maxFallSpeed;
	}

	else if (isOnLadder || isOnRope)
	{
		isInAir = false;
	}

	else
	{
		velocity.y = 0.f;
		isInAir = false;
	}
}


void Character::CheckTileCollision()
{
	float maxDist = 10.0f;
	float debugTime = 1.0f;

	// Collider 기준 
	FPOINT leftTop = { Pos.x - colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
	FPOINT rightTop = { Pos.x + colliderSize.x / 2, Pos.y - colliderSize.y / 2 + colliderOffsetY };
	FPOINT leftBottom = { Pos.x - colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };
	FPOINT rightBottom = { Pos.x + colliderSize.x / 2, Pos.y + colliderSize.y / 2 + colliderOffsetY };

	RaycastHit hitLeft1, hitLeft2, hitRight1, hitRight2;
	RaycastHit hitTop1, hitTop2, hitBottom1, hitBottom2;

	bool onDebug = false;

	//isTouchingLeft = CollisionManager::GetInstance()->RaycastAll({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, true, debugTime, this) ||
	//    CollisionManager::GetInstance()->RaycastAll({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, true, debugTime, this);

	//isTouchingRight = CollisionManager::GetInstance()->RaycastAll({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, true, debugTime, this) ||
	//    CollisionManager::GetInstance()->RaycastAll({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, true, debugTime, this);

	//isTouchingTop = CollisionManager::GetInstance()->RaycastAll({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, true, debugTime, this) ||
	//    CollisionManager::GetInstance()->RaycastAll({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, true, debugTime, this);

	//isTouchingBottom = CollisionManager::GetInstance()->RaycastAll({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, true, debugTime, this) ||
	//    CollisionManager::GetInstance()->RaycastAll({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, true, debugTime, this);

 // RayAll -> RayType : 타일만 레이 쏘게 하는 코드로 변경
	isTouchingLeft = CollisionManager::GetInstance()->RaycastType({ leftTop, {-1.f, 0.f} }, maxDist, hitLeft1, CollisionMaskType::TILE, true, debugTime) ||
		CollisionManager::GetInstance()->RaycastType({ leftBottom, {-1.f, 0.f} }, maxDist, hitLeft2, CollisionMaskType::TILE, true, debugTime);

	isTouchingRight = CollisionManager::GetInstance()->RaycastType({ rightTop, {1.f, 0.f} }, maxDist, hitRight1, CollisionMaskType::TILE, true, debugTime) ||
		CollisionManager::GetInstance()->RaycastType({ rightBottom, {1.f, 0.f} }, maxDist, hitRight2, CollisionMaskType::TILE, true, debugTime);

	isTouchingTop = CollisionManager::GetInstance()->RaycastType({ leftTop, {0.f, -1.f} }, maxDist, hitTop1, CollisionMaskType::TILE, true, debugTime) ||
		CollisionManager::GetInstance()->RaycastType({ rightTop, {0.f, -1.f} }, maxDist, hitTop2, CollisionMaskType::TILE, true, debugTime);

	isTouchingBottom = CollisionManager::GetInstance()->RaycastType({ leftBottom, {0.f, 1.f} }, maxDist, hitBottom1, CollisionMaskType::TILE, true, debugTime) ||
		CollisionManager::GetInstance()->RaycastType({ rightBottom, {0.f, 1.f} }, maxDist, hitBottom2, CollisionMaskType::TILE, true, debugTime);


	if (hitBottom1.hit && hitBottom2.hit)
		bottomHitDist = min(hitBottom1.distance, hitBottom2.distance);
	else if (hitBottom1.hit)
		bottomHitDist = hitBottom1.distance;
	else if (hitBottom2.hit)
		bottomHitDist = hitBottom2.distance;

}

void Character::Move()
{
	if (isHanging)
	{
		//velocity.x = 0.f;
		return;
	}


	float vx = 0.f;
	if (currInput.moveLeft)       vx = -GetSpeed();
	else if (currInput.moveRight) vx = GetSpeed();
	else                                  vx = 0.f;

	SetXVelocity(vx);

	if (!GetIsInAir()) {
		if (isCrouching || currInput.shift)
			SetSpeed(CHARACTER_MOVE_SLOW_SPEED);
		else
			SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED);
	}
	else {
		SetSpeed(CHARACTER_MOVE_DEFAULT_SPEED); // 공중에서도 기본 속도
	}


	float TimeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	float moveDist = velocity.x * TimeDelta;

	// 공중에서 벽에 부딪혔을 때 이동을 막음
	if (GetIsInAir()) {
		if ((velocity.x < 0 && isTouchingLeft) || (velocity.x > 0 && isTouchingRight)) {
			velocity.x = 0.f;  // 공중에서는 이동을 멈춤
			moveDist = 0.f;    // 이동량을 0으로 설정
		}
		if ((velocity.y < 0 && isTouchingTop)) {
			velocity.y = 0.f;  // 공중에서는 이동을 멈춤
			moveDist = 0.f;    // 이동량을 0으로 설정
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
	/* KeyManager* km = KeyManager::GetInstance();
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
	 }*/

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


bool Character::GetIsHangOn() const
{
	return isHanging;
}

void Character::SetIsHangOn(bool value)
{
	isHanging = value;
}
bool Character::GetIsMovingAuto() const
{
	return isMovingAuto;
}
void Character::SetIsMovingAuto(bool value)
{
	isMovingAuto = value;
}

FPOINT Character::GetHangOnTargetPos()
{
	if (isFlip)
		return { Pos.x - colliderSize.x , Pos.y + colliderSize.y + 20.f }; // 왼쪽 끝
	else
		return { Pos.x + colliderSize.x , Pos.y + colliderSize.y + 20.f }; // 오른쪽 끝
}

