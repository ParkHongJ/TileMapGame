#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "Character.h"
#include "CameraManager.h"
#include "GhostJuwel.h"
#include "ImageManager.h"
#include "Monster.h"

GhostJuwel::GhostJuwel() :gold(3000)
{

}

GhostJuwel::~GhostJuwel()
{

}

HRESULT GhostJuwel::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 800, 100 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 20 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void GhostJuwel::Update(float TimeDelta)
{
	DropMove(TimeDelta);
}

void GhostJuwel::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 6, 0, objectScale.x * 0.5f, objectScale.y * 0.5f); // ÀÓÀÇ°ª
}

void GhostJuwel::Release()
{

}

void GhostJuwel::Equip()
{

}

void GhostJuwel::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->GetInfo()->gold += gold;
}

void GhostJuwel::UnEquip()
{

}

void GhostJuwel::UnEquip(void* info)
{

}

void GhostJuwel::Use()
{

}

void GhostJuwel::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}

void GhostJuwel::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<Character>())
	{
		Equip(player->GetPlayerStatus());
		SetDestroy();
		return;
	}

	else if (auto player = obj->GetType<Character>())
	{

	}
}
