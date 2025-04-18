#include "Image.h"
#include "Collider.h"
#include "IncreaseBomb.h"
#include "TestAnimationObject.h"
#include "CameraManager.h"

IncreaseBomb::IncreaseBomb()
{

}

IncreaseBomb::~IncreaseBomb()
{

}

HRESULT IncreaseBomb::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 500, 300 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void IncreaseBomb::Update(float TimeDelta)
{
}

void IncreaseBomb::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 2); // ÀÓÀÇ°ª
}

void IncreaseBomb::Release()
{
}

void IncreaseBomb::Equip()
{
}

void IncreaseBomb::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatusInfo* desc = (PlayerStatusInfo*)info;
	desc->bombCount += 3;
}

void IncreaseBomb::UnEquip()
{
}

void IncreaseBomb::UnEquip(void* info)
{
}

void IncreaseBomb::Use()
{
}

void IncreaseBomb::DropMove(float TimeDelta)
{

}

void IncreaseBomb::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<TestAnimationObject>())
	{
		SetDestroy();
	}
}
