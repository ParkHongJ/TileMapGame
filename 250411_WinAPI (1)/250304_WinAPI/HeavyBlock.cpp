#include "pch.h"
#include "HeavyBlock.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "Image.h"

HRESULT HeavyBlock::Init()
{
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;

	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::TILE, this);
	heavyBlockImage = ImageManager::GetInstance()->FindImage("Trap");
	Pos.x = 650.f;
	Pos.y = 230.f;

	bPhysics = false;
	useGravity = true;
	return S_OK;
}

void HeavyBlock::Release()
{
}

void HeavyBlock::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ADD))
	{
		bPhysics = !bPhysics;
		totalForce = { 0.f,0.f };
	}

	if (bPhysics)
	{
		if (useGravity)
		{
			AddForce({ gravity.x * mass, gravity.y * mass });
		}

		// force 제한
		ClampVector(totalForce, 850.f);

		acceleration = totalForce / mass;
		velocity += acceleration * TimeDelta;

		FPOINT moveVec = velocity * TimeDelta;
		FPOINT nextPos = Pos + moveVec;

		Ray ray;
		ray.origin = Pos;
		ray.origin.y += GAME_TILE_SIZE * 0.5f;
		ray.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;
		FPOINT hitNormal;

		RaycastHit out;

		if (CollisionManager::GetInstance()->RaycastType(ray, moveLength, out, CollisionMaskType::TILE))
		{
			//충돌했으면 중력의 영향을 잠시 초기화
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

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

			const float STOP_THRESHOLD = 100.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			Pos += ray.direction * hitDistance;

			ClampVector(velocity, 450.f);

			if (velocity.Length() < STOP_THRESHOLD && velocity.y > 0.f)
			{
				velocity = { 0.f, 0.f };
			}
			Pos = out.point + hitNormal * 0.5f;
		}
		else
		{
			Pos = nextPos;
		}
	}
}

void HeavyBlock::LateUpdate(float TimeDelta)
{
}

void HeavyBlock::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos();
	heavyBlockImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 0, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
}