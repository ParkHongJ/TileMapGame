#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "IncreaseRopeCnt.h"
#include "Character.h"
#include "CameraManager.h"

IncreaseRopeCnt::IncreaseRopeCnt()
{

}

IncreaseRopeCnt::~IncreaseRopeCnt()
{

}

HRESULT IncreaseRopeCnt::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 500, 300 };

	//BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, this);
	BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_UNABLE;
	return S_OK;
}

void IncreaseRopeCnt::Update(float TimeDelta)
{
	DropMove(TimeDelta);
}

void IncreaseRopeCnt::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 2, objectScale.x, objectScale.y); // 임의값
}

void IncreaseRopeCnt::Release()
{
}

void IncreaseRopeCnt::Equip()
{
}

void IncreaseRopeCnt::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->GetInfo()->ropeCount += 5;
}

void IncreaseRopeCnt::UnEquip()
{

}

void IncreaseRopeCnt::UnEquip(void* info)
{

}

void IncreaseRopeCnt::Use()
{

}

void IncreaseRopeCnt::DropMove(float TimeDelta)
{

}

void IncreaseRopeCnt::Detect(GameObject* obj)
{
	if (auto player = obj->GetType<Character>())
	{
		// 상점일 경우 추가 분기 필요
		Equip(player->GetPlayerStatus());
		SetDestroy();
	}

	else if (auto player = obj->GetType<Character>())
	{

	}
}
