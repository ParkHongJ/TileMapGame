#include "pch.h"
#include "Image.h"
#include "Item.h"
#include "Character.h"
#include "CollisionManager.h"
#include "CommonFunction.h"


Item::Item() : 
price(0), holdImage(nullptr), 
dropImage(nullptr), moveDir({ 0,1 }), movePower({ 0.f,0.f }),
moveReverseDir({ 1,1 }),prePos({ 0,0 }), RayDis(35.f),
itemState(ItemState::STATE_UNEQUIP), startFrameIndexX(0), startFrameIndexY(0), endFrameIndexX(0), endFrameIndexY(0), frameSpeed(1.f), elipsedTime(0.f), isFlip(false)
{
	objectRenderId = RENDER_ITEM;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
	bPhysics = true;
}

Item::~Item()
{
	
}

HRESULT Item::Init()
{
	return S_OK;
}

void Item::SetImage(string input)
{
	holdImage= ImageManager::GetInstance()->FindImage(input);
}

void Item::Update(float TimeDelta)
{
	DropMove(TimeDelta);
	movePower = { -50.f, 90.f }; // Test
	//Pos += {30.f * TimeDelta, 10.f * TimeDelta};
	if (ItemState::STATE_UNEQUIP == itemState)
	{
		DropTime += TimeDelta;
		DropMove(TimeDelta);
	}

	prePos = Pos;

	FrameUpdate(TimeDelta);
}

void Item::Render(ID2D1RenderTarget* renderTarget)
{
	//image->FrameRender(renderTarget, Pos.x, Pos.y, 0, 5); // 임의값
}

void Item::Release()
{

}

void Item::Equip()
{
	ChangeState(ItemState::STATE_EQUIP);
	objectRenderId = RENDER_HOLD;
	CollisionManager::GetInstance()->ChangeMaskType(CollisionMaskType::WORLDOBJECT, CollisionMaskType::ITEM,this);
	bPhysics = false;
	price = 0;
}

void Item::Equip(void* info)
{
	ChangeState(ItemState::STATE_EQUIP);
	objectRenderId = RENDER_HOLD;
	CollisionManager::GetInstance()->ChangeMaskType(CollisionMaskType::WORLDOBJECT, CollisionMaskType::ITEM, this);
	bPhysics = false;
	price = 0;
}

void Item::Equip(GameObject* owner)
{
	ChangeState(ItemState::STATE_EQUIP);
	objectRenderId = RENDER_HOLD;
	CollisionManager::GetInstance()->ChangeMaskType(CollisionMaskType::WORLDOBJECT, CollisionMaskType::ITEM, this);
	bPhysics = false;
	price = 0;
}

void Item::UnEquip()
{
	ChangeState(ItemState::STATE_UNEQUIP);
	objectRenderId = RENDER_ITEM;
	CollisionManager::GetInstance()->ChangeMaskType(CollisionMaskType::ITEM, CollisionMaskType::WORLDOBJECT, this);
	bPhysics = true;

	//movePower = { 500.f, 300.f }; // Test
}

void Item::UnEquip(void* info)
{
	ChangeState(ItemState::STATE_UNEQUIP);
	objectRenderId = RENDER_ITEM;
	CollisionManager::GetInstance()->ChangeMaskType(CollisionMaskType::ITEM, CollisionMaskType::WORLDOBJECT, this);
	bPhysics = true;
	//movePower = { 500.f, 300.f }; // Test
}

void Item::Use()
{
	if (ItemState::STATE_EQUIP == itemState)
	{
		return;
	}
}

void Item::Use(void* info)
{
	if (ItemState::STATE_EQUIP == itemState)
	{
		return;
	}
}

void Item::Detect(GameObject* obj)
{
	//objectRenderId = RENDER_HOLD;
	//interactState = INTERACTSTATE::INTERACT_UNABLE;
}

void Item::FrameUpdate(float TimeDelta)
{
	if (startFrameIndexX != endFrameIndexX ||
		startFrameIndexY != endFrameIndexY)
	{
		elipsedTime += frameSpeed * TimeDelta;
		//curFrameIndexX = (int)elipsedTime - holdImage->GetWidth()
		//if (elipsedTime > endFrameTime)
		//{

		//}
	}

	else
	{
		curFrameIndexX = startFrameIndexX;
		curFrameIndexY = startFrameIndexY;
	}
}

void Item::DropMove(float TimeDelta)
{
	// mass = 1.f;
	// gravity.y = 980.f;
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

		Ray ray;
		ray.origin = Pos + direction * radius;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE,true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
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
				//useGravity = false;
				//bPhysics = false;
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
	//DropMoveX(TimeDelta);
	//DropMoveY(TimeDelta);
}

void Item::DropMoveX(float TimeDelta)
{
	//Ray ray;
	//ray.origin = Pos;

	//moveDir = {  Pos.x - prePos.x, Pos.y };
	//moveDir.x *= moveReverseDir.x;

	//ray.direction = moveDir.Normalized();
	//
	//FPOINT rayPos = Pos;
	//rayPos.x += RayDis * moveReverseDir.x;
	//float nextX = rayPos.x + moveReverseDir.x * movePower.x * TimeDelta;
	//ray = { rayPos, {moveDir.x, 0.f} };
	//RaycastHit out;
	//bool hit = CollisionManager::GetInstance()->RaycastAll(ray, moveReverseDir.x * movePower.x * TimeDelta, out, true, 1.f, this);

	//if (hit && nextX >= out.point.x)
	//{
	//	// 충돌 지점에서 멈춤

	//	Pos.x = out.point.x - RayDis;
	//	moveReverseDir.x *= -1;
	//	//movePower.x /= 3.f;
	//	//movePower.x = 0.f;
	//}

	//else 
	//{
	//	//Pos.x = nextX - (RayDis * moveReverseDir.x);
	//	//Pos.x = nextX - 35.f;
	//	//Pos.y = nextY - 35.f;
	//	Pos.x = nextX - (RayDis * moveReverseDir.x);
	//}

	//Pos.x = nextX - (RayDis * moveReverseDir.x);

}

void Item::DropMoveY(float TimeDelta)
{
	//Ray ray;
	//ray.origin = Pos;

	//moveDir = { Pos.x, Pos.y - prePos.y };
	//moveDir.y *= moveReverseDir.y;

	//ray.direction = moveDir.Normalized();

	//FPOINT rayPos = Pos;
	//rayPos.y += RayDis * moveReverseDir.y;
	//float nextY = rayPos.y + moveReverseDir.y * movePower.y * TimeDelta;
	//ray = { rayPos, {0.f, moveDir.y} };
	//RaycastHit out;
	//bool hit = CollisionManager::GetInstance()->RaycastAll(ray, moveReverseDir.y * movePower.y * TimeDelta, out, true, 1.f);

	//if (hit && nextY >= out.point.y)
	//{
	//	// 충돌 지점에서 멈춤

	//	Pos.y = out.point.y - RayDis;
	//	//moveReverseDir.y *= -1;
	//	//movePower.y /= 3.f;
	//	DropTime = 0.f;
	//}

	//else
	//{
	//	//Pos.y = nextY - (RayDis * moveReverseDir.y);
	//	//Pos.x = nextX - 35.f;
	//	//Pos.y = nextY - 35.f;

	//	// v0 * sin세타.t - 1/2g*t*t;
	//	Pos.y += (0.5f * gravity * DropTime * DropTime);

	//	//Pos.y = nextY - (RayDis * moveReverseDir.y);
	//}

	

}

void Item::ChangeState(ItemState state)
{
	switch (state)
	{
	case ItemState::STATE_UNEQUIP:
		//equipColCoolTime = equipColMaxCoolTime;
		//owner = nullptr;
		//isHold = false;
		break;
	case ItemState::STATE_EQUIP:
		//isLock = false;
		//isHold = true;
		break;
	}

	itemState = state;
}

void Item::DeadEvent()
{

}

void Item::SetDrop(float speed, float angle, float mass, FPOINT gravity)
{
	//float speed = RandomRange(250.0f, 450.0f);            // 속도도 랜덤
	//float speed = 300.f;            // 속도도 랜덤
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

void Item::SetHoldItemPos(FPOINT pos, bool isFlip)
{
	this->isFlip = isFlip;
	FPOINT Temp = holdOffset;
	if (isFlip)
	{
		Temp.x *= -1;
	}
	SetPos(pos + Temp);
	
}

bool Item::IsPlayerDropItem(GameObject* player)
{
	if (nullptr != dynamic_cast<Character*>(player))
	{
		return this == dynamic_cast<Character*>(player)->GetPreHoldItem();
	}

	return false;
}
