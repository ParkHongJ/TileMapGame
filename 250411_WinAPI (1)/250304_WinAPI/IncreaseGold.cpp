#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "Character.h"
#include "CameraManager.h"
#include "IncreaseGold.h"

IncreaseGold::IncreaseGold():gold(1000)
{

}

IncreaseGold::~IncreaseGold()
{

}

HRESULT IncreaseGold::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 800, 100 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 40 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void IncreaseGold::Update(float TimeDelta)
{
	DropMove(TimeDelta);
}

void IncreaseGold::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 15, 0, 0.5f, 0.5f); // ÀÓÀÇ°ª
}

void IncreaseGold::Release()
{

}

void IncreaseGold::Equip()
{

}

void IncreaseGold::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->GetInfo()->gold += gold;
}

void IncreaseGold::UnEquip()
{

}

void IncreaseGold::UnEquip(void* info)
{

}

void IncreaseGold::Use()
{

}

void IncreaseGold::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}

void IncreaseGold::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<Character>())
	{
		Equip(player->GetPlayerStatus());
		SetDestroy();
	}

	else if (auto player = obj->GetType<Character>())
	{

	}

}
