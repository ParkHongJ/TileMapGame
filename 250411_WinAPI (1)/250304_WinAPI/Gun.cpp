#include "Gun.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"

Gun::Gun()
{
}

Gun::~Gun()
{
}

HRESULT Gun::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 500, 400 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, this);

	itemState = ItemState::STATE_UNEQUIP;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void Gun::Update(float TimeDelta)
{
	__super::Update(TimeDelta);
}

void Gun::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (ItemState::STATE_EQUIP == itemState)
	{
		holdImage->FrameRender(renderTarget, Pos.x, Pos.y, 3, 0); // 임의값
	}

	else
	{
		dropImage->FrameRender(renderTarget, Pos.x, Pos.y, 3, 0); // 임의값
	}
}

void Gun::Release()
{
}

void Gun::Equip()
{
	__super::Equip();
}

void Gun::Equip(void* info)
{
	PlayerStatusInfo* desc = (PlayerStatusInfo*)info;
	desc->bombCount += 30;
}

void Gun::Equip(GameObject* owner)
{
	__super::Equip(owner);
}

void Gun::UnEquip()
{
	__super::UnEquip();
}

void Gun::UnEquip(void* info)
{
	__super::UnEquip(info);
}

void Gun::Use()
{
	Pos = { 1000, 1000 }; // Test
}

void Gun::Use(void* info)
{
	__super::Use(info);
}

void Gun::Detect(GameObject* obj)
{

}

void Gun::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}

void Gun::DropMoveX(float TimeDelta)
{
	__super::DropMoveX(TimeDelta);
}

void Gun::DropMoveY(float TimeDelta)
{
	__super::DropMoveY(TimeDelta);
}
