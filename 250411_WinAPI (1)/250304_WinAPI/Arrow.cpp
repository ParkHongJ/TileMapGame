#include "pch.h"
#include "Arrow.h"
#include "Image.h"
#include "Collider.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "ImageManager.h"
#include "Particle.h"
#include "ParticleManager.h"

HRESULT Arrow::Init()
{
	arrowImage = ImageManager::GetInstance()->FindImage("items");

	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE * 0.5f, GAME_TILE_SIZE * 0.5f }, CollisionMaskType::ITEM, this);

	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;
	velocity = { 0.f,0.f };
	return S_OK;
}

void Arrow::Release()
{

}

void Arrow::Update(float TimeDelta)
{
	//화살은 이동하면서 충돌검사를 해야한다. 타일과.

	if (bHit == false)
	{
		FPOINT trapTopPos = Pos;
		if (isFlip)
		{
			//왼쪽
			trapTopPos.x -= GAME_TILE_SIZE * 0.25f;

			Ray ray = { trapTopPos, { -1.f, 0.f } };
			RaycastHit out;

			if (CollisionManager::GetInstance()->RaycastType(ray, moveSpeed * TimeDelta, out, CollisionMaskType::TILE, true, 0.1f))
			{
				Pos.x = out.point.x + GAME_TILE_SIZE * 0.25f;
				bHit = true;
			}
			else
			{
				Pos.x -= moveSpeed * TimeDelta;
			}
		}
		else
		{
			//오른쪽
			trapTopPos.x += GAME_TILE_SIZE * 0.25f;

			Ray ray = { trapTopPos, { 1.f, 0.f } };
			RaycastHit out;

			if (CollisionManager::GetInstance()->RaycastType(ray, moveSpeed * TimeDelta, out, CollisionMaskType::TILE, true, 0.1f))
			{
				Pos.x = out.point.x - GAME_TILE_SIZE * 0.25f;
				bHit = true;
			}
			else
			{
				Pos.x += moveSpeed * TimeDelta;
			}
		}
	}
	else
	{
		hitDelay -= TimeDelta;
		if (hitDelay <= 0.f && bDrop == false)
		{
			angle = 90.f;
			bDrop = true;
		}
	}
	Drop(TimeDelta);
}

void Arrow::LateUpdate(float TimeDelta)
{
}

void Arrow::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();

	arrowImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 1, 1, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE, 1.f, angle);
}

void Arrow::CheckCulling()
{
}

void Arrow::Drop(float timeDelta)
{
	if (bDrop == false)
		return;

	//아래
	if (useGravity)
	{
		AddForce({ gravity.x * mass, gravity.y * mass });
	}

	// force 제한
	//ClampVector(totalForce, 850.f);

	acceleration = totalForce / mass;
	velocity += acceleration * timeDelta;

	FPOINT moveVec = velocity * timeDelta;
	FPOINT nextPos = Pos + moveVec;

	Ray ray;
	ray.origin = Pos;
	ray.origin.y += GAME_TILE_SIZE * 0.25f;
	ray.direction = moveVec.Normalized();

	float moveLength = moveVec.Length();

	float hitDistance;
	RaycastHit out;

	if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE))
	{
		//충돌했으면 중력의 영향을 잠시 초기화
		totalForce.x = 0.0f;
		totalForce.y = 0.0f;
		useGravity = false;
		angle = 0.f;

		hitDistance = out.distance;

		Pos += ray.direction * hitDistance;

	}
	else
	{

		Pos = nextPos;
	}
}

void Arrow::Detect(GameObject* obj)
{
	if (bDrop)
		return;

	//데미지 주기

	//SetActive(false);

	for (int i = 0; i < 5; i++)
	{
		float radius = 30.f; // 원하는 반경

		float angle = RandomRange(0.f, 360.f); // 또는 rand() % 360
		float dist = RandomRange(0.f, radius); // 균일한 거리

		// 라디안 변환
		float rad = angle * (3.141592f / 180.f);

		// 오프셋 계산
		FPOINT offset = {
			cosf(rad) * dist,
			sinf(rad) * dist
		};

		FPOINT spawnPos = Pos + offset;

		Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", spawnPos, (rand() % 360), 55.f, 2.5f, 4, 1);
		StarOption* starOp = new StarOption(30.f);

		particle->AddParticleOption(starOp);

		//3 6
	}
	{
		Particle* particle = ParticleManager::GetInstance()->GetParticle("Effect", Pos, (rand() % 360), 85.f, 3.5f, 3, 6);
		StarOption* starOp = new StarOption(10.f);

		particle->AddParticleOption(starOp);

		//3 6
	}
	SetDestroy();
}
