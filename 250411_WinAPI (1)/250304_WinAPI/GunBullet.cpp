#include "pch.h"
#include "Image.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "GunBullet.h"
#include "ParticleManager.h"
#include "Particle.h"
#include "ImageManager.h"
#include "Monster.h"

GunBullet::GunBullet() : image(nullptr)
{

}

GunBullet::~GunBullet()
{

}

HRESULT GunBullet::Init()
{
	image = ImageManager::GetInstance()->FindImage("items");

	BoxCollider* col = new BoxCollider({ 0,0 }, { 20,20 }, CollisionMaskType::WORLDOBJECT, this);
	interactState = INTERACTSTATE::INTERACT_ABLE;
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };

	//SetDrop();
	return S_OK;
}

void GunBullet::Update(float TimeDelta)
{
	Move(TimeDelta);
	//Pos.x += 30.f * TimeDelta;

}

void GunBullet::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
	image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 14, objectScale.x, objectScale.y);
}

void GunBullet::Release()
{
}

void GunBullet::Detect(GameObject* obj)
{
	// 이펙트 필요
	for (int i = 0; i < 3; i++)
	{
		FPOINT randPos = { RandomRange(-10, 10.f), RandomRange(-10, 10.f) };
		Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos + randPos, 0.f, 30.f, 0.15f, 5, 6);

		PhysicsOption* physicsOp = new PhysicsOption;
		SizeOption* sizeOp = new SizeOption(0.04f);
		TrailOption* trailOp = new TrailOption("Effect", 0.02f, 0.2f);

		//float angleRad = RandomRange(-3.141592 / 4.0f, 3.141592 / 4.0f);
		float angleRad = RandomRange(0.0f, 3.141592f * 2.0f); // 0 ~ 360도
		//float speed = RandomRange(350.f, 375.0f);            // 속도도 랜덤
		float speed = RandomRange(450.f, 575.0f);            // 속도도 랜덤

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

	if (false == IsDestroyed())
	{
		SetDestroy();
	}

	if (false == obj->IsDestroyed())
	{
		if (OBJECTNAME::TILE != obj->GetObjectName())
		{
			// 가라
			
			if (Monster* monster = dynamic_cast<Monster*>(obj))
			{
				monster->Damaged();
			}
			//obj->SetDestroy();
		}
	}
}

void GunBullet::Move(float TimeDelta)
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

			FPOINT perturbedNormal = RotateVector(hitNormal, RandomRange(-50.f, 50.f));
			velocity = Reflect(velocity, /*perturbedNormal.Normalized()*/hitNormal.Normalized());

			velocity *= bounciness;

			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.x) < STOP_THRESHOLD)
				velocity.x = 0.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 350.f);

			if (velocity.Length() < 130.f)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
				bPhysics = false;
			}

			// 살짝 밀기 (겹침 방지)

			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void GunBullet::SetDrop(float speed, float angle)
{
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

