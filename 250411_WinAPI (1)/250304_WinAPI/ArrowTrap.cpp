#include "pch.h"
#include "ArrowTrap.h"
#include "Image.h"
#include "CameraManager.h"
#include "CollisionManager.h"
#include "Arrow.h"
#include "ImageManager.h"
#include "ObjectManager.h"
#include "Particle.h"
#include "ParticleManager.h"

HRESULT ArrowTrap::Init()
{
	trapImage = ImageManager::GetInstance()->FindImage("Trap");
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;
	//Pos.x = 250.f;
	//Pos.y = 150.f;

	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::TILE, this);
	return S_OK;
}

void ArrowTrap::Release()
{
}

void ArrowTrap::Update(float TimeDelta)
{
	//아이템, 몬스터, 플레이어 이렇게 3개다.

	if (isFire == false)
	{
		FPOINT trapTopPos = Pos;
		trapTopPos.y -= GAME_TILE_SIZE / 2.f;

		RaycastHit out;
		Ray ray;
		if (bFlipX == false)
		{
			ray = { trapTopPos, { 1.f, 0.f } };
		}
		else
		{
			ray = { trapTopPos, { -1.f, 0.f } };
		}

		//Trigger On
		if (CollisionManager::GetInstance()->RaycastMyType(ray, 200.f, out, CollisionMaskType::ITEM, this))
		{
			Fire();
		}
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		
	}
}

void ArrowTrap::LateUpdate(float TimeDelta)
{
}

void ArrowTrap::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();

	float flipScale = 0.f;

	if (bFlipX)
		flipScale = -1.f;
	else
		flipScale = 1.f;

	trapImage->Render(renderTarget, cameraPos.x, cameraPos.y, flipScale * objectScale, objectScale, 1, 0, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
}

void ArrowTrap::CheckCulling()
{
}

void ArrowTrap::Fire()
{
	Arrow* arrow = new Arrow;
	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, arrow);
	isFire = true;

	FPOINT spawnPos = Pos;


	if (bFlipX)
	{
		spawnPos.x -= GAME_TILE_SIZE * 0.5f;
	}
	else
	{
		//오른쪽
		spawnPos.x += GAME_TILE_SIZE * 0.5f;
	}

	arrow->SetPos(spawnPos);
	arrow->SetDirection(bFlipX);


}
