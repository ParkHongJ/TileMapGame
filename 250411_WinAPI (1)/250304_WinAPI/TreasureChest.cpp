#include "pch.h"
#include "TreasureChest.h"
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

TreasureChest::TreasureChest()
{
}

TreasureChest::~TreasureChest()
{
}

HRESULT TreasureChest::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 700, 100 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, CollisionMaskType::WORLDOBJECT,this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 0;

	holdOffset = { 0.f, -10.f };
	return S_OK;
}

void TreasureChest::Update(float TimeDelta)
{
	if (isFlip)
	{
		holdOffset.x = -30.f;
	}

	else
	{
		holdOffset.x = 30.f;
	}

	DropMove(TimeDelta);
}

void TreasureChest::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos + holdOffset;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 0, objectScale.x, objectScale.y, isFlip); // 임의값
}

void TreasureChest::Release()
{

}

void TreasureChest::Equip()
{
	__super::Equip();
}

void TreasureChest::Equip(void* info)
{
}

void TreasureChest::Equip(GameObject* owner)
{
	__super::Equip(owner);
}

void TreasureChest::UnEquip()
{
}

void TreasureChest::UnEquip(void* info)
{
}

void TreasureChest::Use()
{

}

void TreasureChest::Use(void* info)
{
}

void TreasureChest::Detect(GameObject* obj)
{
	if (auto temp = obj->GetType<Tile>())
	{
		return;
	}

	if (auto temp = obj->GetType<Character>())
	{
		return;
	}

	DeadEvent();

	// 플레이어 어택이나 WorldItem이면 부숴져라.
	//if (auto player = obj->GetType<Character>() || auto worldItem = obj->GetType<Character>() )
	//{
	//	SetDestroy();
	//}
}

void TreasureChest::DeadEvent()
{
	for (int i = 0; i < 5; i++)
	{
		FPOINT randPos = { RandomRange(-10, 10.f), RandomRange(-10, 10.f) };
		Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, 0.f, 30.f, 0.15f, 4, 1);

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

void TreasureChest::DropMove(float TimeDelta)
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

void TreasureChest::DropMoveX(float TimeDelta)
{
}

void TreasureChest::DropMoveY(float TimeDelta)
{
}
