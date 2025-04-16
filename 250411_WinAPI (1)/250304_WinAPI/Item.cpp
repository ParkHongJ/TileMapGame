#include "Image.h"
#include "Item.h"


Item::Item() : isLock(false), isHold(false), price(0), image(nullptr)
{

}

Item::~Item()
{

}

HRESULT Item::Init()
{
	return S_OK;
}

void Item::Update(float TimeDelta)
{
	
}

void Item::Render(ID2D1HwndRenderTarget* renderTarget)
{
	image->Render(renderTarget, Pos.x, Pos.y);
}

void Item::Release()
{

}

void Item::Equip()
{
	ChangeState(ItemState::STATE_EQUIP);
}

void Item::Equip(void* info)
{
	ChangeState(ItemState::STATE_EQUIP);
}

void Item::UnEquip()
{
	ChangeState(ItemState::STATE_UNEQUIP);
}

void Item::UnEquip(void* info)
{
	ChangeState(ItemState::STATE_UNEQUIP);
}

void Item::Use()
{

}

void Item::ChangeState(ItemState state)
{
	switch (state)
	{
	case ItemState::STATE_UNEQUIP:
		isHold = false;
		break;
	case ItemState::STATE_EQUIP:
		isLock = false;
		isHold = true;
		break;
	}
}
