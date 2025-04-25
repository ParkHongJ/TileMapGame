#include "pch.h"
#include "SkeletonItem.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"
#include "CameraManager.h"
#include "IncreaseGold.h"
#include "CollisionManager.h"
#include "Tile.h"
#include "Character.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "ImageManager.h"
#include "Monster.h"

SkeletonItem::SkeletonItem()
{
}

SkeletonItem::~SkeletonItem()
{
}

HRESULT SkeletonItem::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 500, 100 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 50 }, CollisionMaskType::WORLDOBJECT,this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 0;

	holdOffset = { 35.f, 5.f };
	return S_OK;
}

void SkeletonItem::Update(float TimeDelta)
{
	//if (isFlip)
	//{
	//	holdOffset.x = -50.f;
	//}

	//else
	//{
	//	holdOffset.x = 50.f;
	//}

	DropMove(TimeDelta);
}

void SkeletonItem::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 15, 3, objectScale.x , objectScale.y, isFlip); // 임의값
}

void SkeletonItem::Release()
{

}

void SkeletonItem::Equip()
{
	__super::Equip();
}

void SkeletonItem::Equip(void* info)
{
}

void SkeletonItem::Equip(GameObject* owner)
{
	__super::Equip(owner);
}

void SkeletonItem::UnEquip()
{
	__super::UnEquip();
}

void SkeletonItem::UnEquip(void* info)
{
}

void SkeletonItem::Use()
{

}

void SkeletonItem::Use(void* info)
{
}

void SkeletonItem::Detect(GameObject* obj)
{
	if (auto temp = obj->GetType<Tile>())
	{
		return;
	}

	if (auto temp = obj->GetType<Character>())
	{
		return;
	}

	if (IsPlayerDropItem(obj) || 0.f == velocity.x || 0.f == velocity.y)
	{
		return;
	}

	if (auto monster = dynamic_cast<Monster*>(obj))
	{
		monster->SetMonsterHP(monster->GetMonsterHP() - 1);
		if (0 >= monster->GetMonsterHP())
		{
			monster->SetDestroy();
		}
	}

	DeadEvent();

	// 플레이어 어택이나 WorldItem이면 부숴져라.
	//if (auto player = obj->GetType<Character>() || auto worldItem = obj->GetType<Character>() )
	//{
	//	SetDestroy();
	//}
}

void SkeletonItem::DeadEvent()
{
	for (int i = 0; i < 5; i++)
	{
		FPOINT randPos = { RandomRange(-10, 10.f), RandomRange(-10, 10.f) };
		Particle* particle = ParticleManager::GetInstance()->GetParticle("Rubble", Pos + randPos, 0.f, 30.f, 0.15f, 6, 0);

		PhysicsOption* physicsOp = new PhysicsOption;
		SizeOption* sizeOp = new SizeOption(0.04f);
		TrailOption* trailOp = new TrailOption("Effect", 0.02f, 0.2f);

		//float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
		float angleRad = RandomRange(0.0f, 3.141592f * 2.0f); // 0 ~ 360도
		//float speed = RandomRange(350.f, 375.0f);            // 속도도 랜덤
		float speed = RandomRange(850.f, 1175.0f);            // 속도도 랜덤

		velocity =
		{
			sinf(angleRad) * speed,
			-cosf(angleRad) * speed  // 135도 (왼쪽 위)
		};

		physicsOp->Init(velocity, 0.3f);
		//physicsOp->Init(velocity, 0.5f);

		particle->AddParticleOption(physicsOp);
		particle->AddParticleOption(sizeOp);
		particle->AddParticleOption(trailOp);
	}

	{
		float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
		FPOINT randPos = { RandomRange(-100, 100.f), RandomRange(-100, 100.f) };

		Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, angleRad, 105.f, 1.f, 1, 2);

		AlphaOption* alphaOp = new AlphaOption(5.0f);
		particle->AddParticleOption(alphaOp);
	}
	IncreaseGold* temp = new IncreaseGold();
	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
	temp->SetPos(Pos); // 가라
	temp->SetDrop(300.f, 90.f, 0.3f, {0,400.f}); // 진짜 구린데 어떡하냐 이거
	SetDestroy();
}

void SkeletonItem::DropMove(float TimeDelta)
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

		Ray ray;
		ray.origin = Pos;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
		{
			DeadEvent();
			return;
			//DeadEvent();
			//return;
			//hitDistance = out.distance;

			//FPOINT colliderPos = out.collider->GetWorldPos();
			//FPOINT colliderScale = out.collider->GetScale();

			//// 2. 방향 벡터
			//FPOINT toHit = out.point - colliderPos;

			//// 스케일 보정한 방향 추정
			//float xRatio = toHit.x / (colliderScale.x * 0.5f);
			//float yRatio = toHit.y / (colliderScale.y * 0.5f);

			//if (fabs(xRatio) > fabs(yRatio))
			//	hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
			//else
			//	hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };

			//FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-50.f, 50.f));
			//velocity = Reflect(velocity, /*perturbedNormal.Normalized()*/hitNormal.Normalized());

			//velocity *= bounciness;

			//totalForce.x = 0.0f;
			//totalForce.y = 0.0f;

			//const float STOP_THRESHOLD = 100.f;
			//if (fabs(velocity.x) < STOP_THRESHOLD)
			//	velocity.x = 0.f;
			//if (fabs(velocity.y) < STOP_THRESHOLD)
			//	velocity.y = 0.f;

			//// 보정 위치
			//Pos += ray.direction * hitDistance;

			//ClampVector(velocity, 350.f);

			//if (velocity.Length() < 130.f)
			//{
			//	velocity = { 0.f, 0.f };
			//	useGravity = false;
			//	bPhysics = false;
			//}

			//// 살짝 밀기 (겹침 방지)

			//Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void SkeletonItem::DropMoveX(float TimeDelta)
{

}

void SkeletonItem::DropMoveY(float TimeDelta)
{

}
