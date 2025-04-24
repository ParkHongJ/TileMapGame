#include "pch.h"
#include "Bomb.h"
#include "Image.h"
#include "Collider.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "PlayerStatus.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "Tile.h"

Bomb::Bomb()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	itemState = ItemState::STATE_UNEQUIP;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	curFrameIndexX = 0;
	curFrameIndexY = 5;

	startFrameIndexX = 0;
	startFrameIndexY = 5;

	endFrameIndexX = 2;
	endFrameIndexY = 5;
	frameSpeed = 3.334f;

	BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, this);


	radius = 20.f;
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init()
{
	Pos = { 200, 200 };
	return S_OK;
}

void Bomb::Update(float TimeDelta)
{
	FrameUpdate(TimeDelta);
	if (!IsStop)
	{
		DropMove(TimeDelta);
	}
	
	explosionTime -= TimeDelta;
	animationTime -= TimeDelta;

	if (0.f >= animationTime)
	{
		frameSpeed += animAcc * TimeDelta;
		if (1.4f >= explosionTime)
		{
			frameSpeed += 2.f * animAcc * TimeDelta;
		}
		//frameSpeed += TimeDelta; // 가라
	}

	if (0.f >= explosionTime)
	{
		Explosion();
	}
}

void Bomb::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameIndexX, curFrameIndexY, objectScale.x, objectScale.y);

	//if (ItemState::STATE_EQUIP == itemState)
	//{
	//	
	//}

	//else
	//{
	//	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameIndexX, curFrameIndexY); // 임의값
	//}
}

void Bomb::Release()
{

}

void Bomb::Equip()
{
	__super::Equip();
}

void Bomb::Equip(void* info)
{
	__super::Equip(info);
}

void Bomb::UnEquip()
{
	__super::UnEquip();
}

void Bomb::UnEquip(void* info)
{
	PlayerStatus* desc = (PlayerStatus*)info;

	//__super::UnEquip(info);
	IsCobweb = desc->IsCobweb();

	if (IsCobweb)
	{
		curFrameIndexX = 3;
		curFrameIndexY = 5;

		startFrameIndexX = 3;
		startFrameIndexY = 5;

		endFrameIndexX = 5;
		endFrameIndexY = 5;
	}
}

void Bomb::Use()
{
	//Pos = { 1000, 1000 }; // Test
}

void Bomb::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
	//if (bPhysics)
	//{
	//	if (useGravity)
	//	{
	//		AddForce({ gravity.x * mass, gravity.y * mass });
	//	}

	//	// force 제한
	//	ClampVector(totalForce, 450.f);

	//	acceleration = totalForce / mass;
	//	velocity += acceleration * TimeDelta;

	//	FPOINT moveVec = { velocity.x * TimeDelta, velocity.y * TimeDelta };
	//	FPOINT nextPos = { Pos.x + moveVec.x, Pos.y + moveVec.y };

	//	Ray ray;
	//	ray.origin = Pos;
	//	ray.direction = moveVec.Normalized();

	//	float moveLength = moveVec.Length();
	//	float hitDistance;
	//	FPOINT hitNormal;

	//	RaycastHit out;

	//	//if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
	//	//if (CollisionManager::GetInstance()->RaycastMyType(ray, moveLength, out, CollisionMaskType::ITEM, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
	//		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE, true, 1.f))//RayTileCheck(ray, moveLength, tiles, hitNormal, hitDistance))
	//	{
	//		if (IsCobweb)
	//		{
	//			IsStop = true;
	//			return;
	//		}
	//		hitDistance = out.distance;

	//		FPOINT colliderPos = out.collider->GetWorldPos();
	//		FPOINT colliderScale = out.collider->GetScale();

	//		// 2. 방향 벡터
	//		FPOINT toHit = out.point - colliderPos;

	//		// 스케일 보정한 방향 추정
	//		float xRatio = toHit.x / (colliderScale.x * 0.5f);
	//		float yRatio = toHit.y / (colliderScale.y * 0.5f);

	//		if (fabs(xRatio) > fabs(yRatio))
	//			hitNormal = { (xRatio < 0 ? -1.f : 1.f), 0.f };
	//		else
	//			hitNormal = { 0.f, (yRatio < 0 ? -1.f : 1.f) };

	//		FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-50.f, 50.f));
	//		velocity = Reflect(velocity, /*perturbedNormal.Normalized()*/hitNormal.Normalized());

	//		velocity *= bounciness;

	//		totalForce.x = 0.0f;
	//		totalForce.y = 0.0f;

	//		const float STOP_THRESHOLD = 100.f;
	//		if (fabs(velocity.x) < STOP_THRESHOLD)
	//			velocity.x = 0.f;
	//		if (fabs(velocity.y) < STOP_THRESHOLD)
	//			velocity.y = 0.f;

	//		// 보정 위치
	//		Pos += ray.direction * hitDistance;

	//		ClampVector(velocity, 350.f);

	//		if (velocity.Length() < 130.f)
	//		{
	//			velocity = { 0.f, 0.f };
	//			useGravity = false;
	//			bPhysics = false;
	//		}

	//		// 살짝 밀기 (겹침 방지)

	//		Pos = out.point + hitNormal * 0.5f;
	//	}
	//	else
	//	{
	//		Pos = nextPos;
	//	}
	//}
}

void Bomb::Detect(GameObject* obj)
{

	//set
}

void Bomb::FrameUpdate(float TimeDelta)
{

	elipsedTime += frameSpeed * TimeDelta;
	curFrameIndexX = int(elipsedTime);
	if (IsCobweb)
	{
		curFrameIndexX += 3;
	}

	if (curFrameIndexX > endFrameIndexX)
	{
		curFrameIndexX = elipsedTime = 0;
	}
}

void Bomb::Explosion()
{
	vector<GameObject*> temp;
	CollisionManager::GetInstance()->GetObjectsInCircle(Pos, explosionRadius, &temp);

	for (auto& iter : temp)
	{
		if (Tile* tile = dynamic_cast<Tile*>(iter))
		{
			tile->Destruction();
		}
			
		else
		{
			iter->SetDestroy();
		}
		//
	}

	SetDestroy();

	for (int i = 0; i < 15; i++)
	{
		{
			FPOINT randPos = { RandomRange(-100, 100.f), RandomRange(-100, 100.f) };
			Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, 0.f, 30.f, 3.f, 1, 0);

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

		//{
		//	Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos, 0.f, 45.f, 0.01f, 4, 1);
		//	PhysicsOption* physicsOp = new PhysicsOption;

		//	float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
		//	float speed = RandomRange(450.f, 555.0f);            // 속도도 랜덤

		//	velocity =
		//	{
		//		sinf(angleRad) * speed,
		//		-cosf(angleRad) * speed  // 135도 (왼쪽 위)
		//	};

		//	physicsOp->Init(velocity, 0.5f);

		//	AlphaOption* alphaOp = new AlphaOption(0.5f);
		//	particle->AddParticleOption(physicsOp);
		//	particle->AddParticleOption(alphaOp);
		//}
	}
}
