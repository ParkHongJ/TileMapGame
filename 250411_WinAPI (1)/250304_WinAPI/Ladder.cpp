#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Ladder.h"


Ladder::Ladder()
{
	image = ImageManager::GetInstance()->FindImage("floor_cave");
	Pos = { 1100,150 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100, 40}, CollisionMaskType::ITEM, this);
	interactState = INTERACTSTATE::INTERACT_ABLE;
	objectName = OBJECTNAME::LADDER;
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
}

Ladder::~Ladder()
{

}

HRESULT Ladder::Init()
{
    return S_OK;
}

void Ladder::Update(float TimeDelta)
{

}

void Ladder::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
	// 0 ~ 4까지가 최대, 2는 뺄까
	//image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 0, 0.75f, 0.75f); // 임의값

	//float Height = image->GetHeight() / 12 * 0.75f;
	//image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 0); // 임의값
	//image->FrameRender(renderTarget, cameraPos.x, cameraPos.y + Height, 4, 1); // 임의값
	//image->FrameRender(renderTarget, cameraPos.x, cameraPos.y + Height * 2, 4, 2); // 임의값
	//image->FrameRender(renderTarget, cameraPos.x, cameraPos.y + Height * 3, 4, 3); // 임의값
	//image->FrameRender
}

void Ladder::Release()
{

}

void Ladder::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<Character>())
	{
		//player->SetPos({ Pos.x, player->GetPos().y });
		//SetDestroy();
	}

}

