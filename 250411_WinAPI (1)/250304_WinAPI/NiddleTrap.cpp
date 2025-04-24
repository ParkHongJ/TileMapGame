#include "pch.h"
#include "NiddleTrap.h"
#include "Collider.h"
#include "Image.h"
#include "CameraManager.h"
HRESULT NiddleTrap::Init()
{
	objectScale = GAME_TILE_SIZE / ATLAS_TILE_SIZE;
	collider = new BoxCollider({ 0.f,0.f }, { GAME_TILE_SIZE, GAME_TILE_SIZE }, CollisionMaskType::WORLDOBJECT, this);

	Pos.x = 650.f;
	Pos.y = 230.f;

	NiddleImage = ImageManager::GetInstance()->FindImage("floor_cave");

	return S_OK;
}

void NiddleTrap::Release()
{
}

void NiddleTrap::Update(float TimeDelta)
{
}

void NiddleTrap::LateUpdate(float TimeDelta)
{
}

void NiddleTrap::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos();
	NiddleImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 5, 9, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);

	if (bHit)
	{
		NiddleImage->Render(renderTarget, cameraPos.x, cameraPos.y, objectScale, objectScale, 5, 10, ATLAS_TILE_SIZE, ATLAS_TILE_SIZE);
	}
}

void NiddleTrap::CheckCulling()
{
}

void NiddleTrap::Detect(GameObject* obj)
{
	FPOINT objectVelocity = obj->GetVelocity();

	if (objectVelocity.y >= 200.f)
	{
		int a = 10;
		bHit = true;
	}
}
