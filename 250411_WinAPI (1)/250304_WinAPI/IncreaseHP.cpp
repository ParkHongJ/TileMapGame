#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "Character.h"
#include "CameraManager.h"
#include "IncreaseHP.h"

IncreaseHP::IncreaseHP()
{

}

IncreaseHP::~IncreaseHP()
{

}

HRESULT IncreaseHP::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("turkey");
	holdImage = ImageManager::GetInstance()->FindImage("turkey");

	Pos = { 800, 200 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 20 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void IncreaseHP::Update(float TimeDelta)
{
	DropMove(TimeDelta);
}

void IncreaseHP::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 10, objectScale.x * 0.5f, objectScale.y * 0.5f); // ÀÓÀÇ°ª
}

void IncreaseHP::Release()
{

}

void IncreaseHP::Equip()
{

}

void IncreaseHP::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->PlusPlayerHP(1);
}

void IncreaseHP::UnEquip()
{

}

void IncreaseHP::UnEquip(void* info)
{

}

void IncreaseHP::Use()
{

}

void IncreaseHP::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}

void IncreaseHP::Detect(GameObject* obj)
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