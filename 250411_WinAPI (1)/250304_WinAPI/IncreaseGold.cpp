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

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 40 }, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void IncreaseGold::Update(float TimeDelta)
{

}

void IncreaseGold::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 15, 0, 0.5f, 0.5f); // 임의값
}

void IncreaseGold::Release()
{

}

void IncreaseGold::Equip()
{

}

void IncreaseGold::Equip(void* info)
{
	if (ItemState::STATE_EQUIP == itemState)
	{
		// 두 번 충돌 방지 임시 코드
		return;
	}

	itemState = ItemState::STATE_EQUIP;
	PlayerStatusInfo* desc = (PlayerStatusInfo*)info;
	desc->gold += gold;
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

}

void IncreaseGold::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<Character>())
	{
		SetDestroy();
	}

	else if (auto player = obj->GetType<Character>())
	{

	}

}
