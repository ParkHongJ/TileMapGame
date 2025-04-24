#include "pch.h"
#include "Monster.h"
#include "Character.h"
#include "CollisionManager.h"

Monster::Monster()
{
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE , GAME_TILE_SIZE / ATLAS_TILE_SIZE };
}

Monster::~Monster()
{
}

HRESULT Monster::Init()
{
	monsterState = MonsterState::IDLE;
	player = new Character();
	damage = 0;
	monsterHP = 0;
	dir = { -1,-1 };
	moveSpeed = 0.0f;

	startFrameIndexX = 0;
	startFrameIndexY = 0;

	endFrameIndexX = 0;
	endFrameIndexY = 0;

	curFrameIndexX = 0;
	curFrameIndexY = 0;

	elipsedTime = 0.0f;
	frameSpeed = 0.0f;
	meetPlayer = false;
	isAttack = false;
	isDamaged = false;
	isFlip = false;

	isTileTouchingLeft = false;
	isTileTouchingRight = false;
	isTileTouchingTop = false;
	isTileTouchingLeftBottom = false;
	isTileTouchingRightBottom = false;
	isTileTouchingLeftCenter = false;
	isTileTouchingRightCenter = false;

	hasBottomTile = true;

	isPlayerTouchingLeft = false;
	isPlayerTouchingRight = false;
	isPlayerTouchingTop = false;
	isPlayerTouchingBottom = false;
	isPlayerTouchingCenterTop = false;

	isItemTouchingLeft = false;
	isItemTouchingRight = false;
	isItemTouchingTop = false;
	isItemTouchingBottom = false;

	meetPlayerLeft = false;
	meetPlayerRight = false;

	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
	return S_OK;
}

void Monster::Release()
{
	if (player)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
}

void Monster::Update(float TimeDelta)
{
	Move();
	FrameUpdate(TimeDelta);
}

void Monster::FrameUpdate(float TimeDelta)
{
}

void Monster::CheckTileCollision()
{
}

void Monster::CheckPlayerCollision()
{
}

void Monster::CheckItemCollision()
{
}

void Monster::MeetPlayer(float TimeDelta)
{
}

void Monster::Move()
{
}

void Monster::ApplyGravity(float TimeDelta)
{
}

void Monster::Render(ID2D1RenderTarget* renderTarget)
{
	
}

void Monster::Damaged()
{
	monsterHP -= 1;
}

void Monster::MoveJumpStart(float speed, float angle, float mass, FPOINT gravity)
{
	this->mass = mass;
	this->gravity = gravity;

	velocity =
	{
		//cosf(angleRad) * speed,  // 135도 (왼쪽 위)
		//-sinf(angleRad) * speed

		cosf(DEG_TO_RAD(angle)) * speed,  // 135도 (왼쪽 위)
		-sinf(DEG_TO_RAD(angle)) * speed
	};

	acceleration = { 0, 0 };  // 가속도
	totalForce = { 0.f,0.f };

	useGravity = true;
	bPhysics = true;
}

void Monster::MoveJump(float TimeDelta)
{
	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force 제한
		ClampVector(totalForce, 450.f);

		acceleration = totalForce / mass;
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = { velocity.x * TimeDelta, velocity.y * TimeDelta };
		FPOINT nextPos = { Pos.x + moveVec.x, Pos.y + moveVec.y };

		FPOINT direction = moveVec.Normalized();

		float radius = 50.f;
		Ray ray;
		ray.origin = Pos + direction * radius;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
		{
			hitDistance = out.distance;

			FPOINT colliderPos = out.collider->GetWorldPos();
			FPOINT colliderScale = out.collider->GetScale();

			// 2. 방향 벡터
			FPOINT toHit = out.point - colliderPos;

			// 스케일 보정한 방향 추정
			float xRatio = toHit.x / (colliderScale.x * 0.5f);
			float yRatio = toHit.y / (colliderScale.y * 0.5f);

			if (fabs(xRatio) > fabs(yRatio))
				hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
			else
				hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };

			velocity = Reflect(velocity, hitNormal.Normalized());

			velocity *= bounciness;

			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.x) < STOP_THRESHOLD)
				velocity.x = 0.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			//Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < 130.f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
				if (monsterState == MonsterState::ATTACKMOVE)
				{
					monsterState = MonsterState::WAITATTACK;
				}
			}

			// 살짝 밀기 (겹침 방지)

			//Pos = out.point + hitNormal * 0.5f;
			Pos = out.point - direction * radius;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void Monster::Detect(GameObject* obj)
{
}

void Monster::DeadEvent(float TimeDelta)
{
}
