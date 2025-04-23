#include "pch.h"
#include "HeavyBlock.h"
#include "CommonFunction.h"
#include "Collider.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "Image.h"
#include "GameManager.h"
HRESULT HeavyBlock::Init()
{
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;

	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::TILE, this);
	heavyBlockImage = ImageManager::GetInstance()->FindImage("Trap");
	Pos.x = 650.f;
	Pos.y = 200.f;

	bPhysics = false;
	useGravity = true;

	//objectRenderId = RENDERORDER::RENDER_UI;
	
	objectName = OBJECTNAME::HEAVYBLOCK;

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
		acceleration = { 0.f,0.f };
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

		Ray rayLeft;
		Ray rayRight;

		rayLeft.origin = Pos;
		rayLeft.origin.x -= GAME_TILE_SIZE * 0.5f;
		rayLeft.origin.y += GAME_TILE_SIZE * 0.5f;
		rayLeft.direction = moveVec.Normalized();

		rayRight.origin = Pos;
		rayRight.origin.x += GAME_TILE_SIZE * 0.5f;
		rayRight.origin.y += GAME_TILE_SIZE * 0.5f;
		rayRight.direction = moveVec.Normalized();

		float moveLength = moveVec.Length();
		float hitDistance;

		RaycastHit outLeft;
		RaycastHit outRight;

		bGround = CollisionManager::GetInstance()->RaycastType(rayLeft, moveLength, outLeft, CollisionMaskType::TILE, this) ||
			CollisionManager::GetInstance()->RaycastType(rayRight, moveLength, outRight, CollisionMaskType::TILE, this);

		if (bGround)
		{
			//충돌했으면 중력의 영향을 잠시 초기화
			totalForce.x = 0.0f;
			totalForce.y = 0.0f;

			hitDistance = min(outLeft.distance, outRight.distance);

			const float STOP_THRESHOLD = 150.f;
			if (fabs(velocity.y) < STOP_THRESHOLD)
				velocity.y = 0.f;

			// 보정 위치
			Pos += rayRight.direction * hitDistance;

			ClampVector(velocity, 150.f);

			if (velocity.Length() <= STOP_THRESHOLD)
			{
				velocity = { 0.f, 0.f };
				useGravity = false;
			}
		}
		else
		{
			useGravity = true;
			Pos = nextPos;
		}
	}
}

void HeavyBlock::LateUpdate(float TimeDelta)
{
}

void HeavyBlock::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	heavyBlockImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 0, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
}

void HeavyBlock::Detect(GameObject* obj)
{
	if (velocity.y >= 50.f)
	{
		obj->SetDestroy();
		int a = 10;
	}
}

bool HeavyBlock::OnPush(FPOINT dir, float strength)
{
	if (bGround == false)
	{
		return false;
	}
	// 방향 제한 가능 (예: 좌우만)
	if (fabs(dir.y) > 0.5f) return false;

	velocity = dir * strength;

	float timeDelta = TimerManager::GetInstance()->GetDeltaTime(L"60Frame");

	FPOINT nextPos = Pos + velocity * timeDelta;

	int tileIndexX = static_cast<int>(nextPos.x / GAME_TILE_SIZE);
	int tileIndexY = static_cast<int>(nextPos.y / GAME_TILE_SIZE);

	//이동 가능
	if (GameManager::GetInstance()->HasTile(tileIndexX, tileIndexY) == false)
	{
		Pos = nextPos;
		return true;
	}
	return false;
}
