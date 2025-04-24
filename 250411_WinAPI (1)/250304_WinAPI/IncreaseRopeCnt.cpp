#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "IncreaseRopeCnt.h"
#include "Character.h"
#include "CameraManager.h"
#include "ItemDialog.h"
#include "ImageManager.h"
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
	price = 2000;
	if (0 < price)
	{
		dialog = new ItemDialog();
		ObjectManager::GetInstance()->AddObject(RENDER_UI, dialog);
		dialog->SetPrice(price);
		dialog->SetPos(Pos);
	}
	return S_OK;
}

void IncreaseRopeCnt::Update(float TimeDelta)
{
	if (dialog)
	{
		dialog->SetPos(Pos);
		dialog->SetCol(isDialogCol);
		isDialogCol = false;
	}
	bPhysics = true;
	DropMove(TimeDelta);
}

void IncreaseRopeCnt::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 2, objectScale.x, objectScale.y); // ÀÓÀÇ°ª
}

void IncreaseRopeCnt::Release()
{
}

void IncreaseRopeCnt::Equip()
{
}

void IncreaseRopeCnt::Equip(void* info)
{
	//itemState = ItemState::STATE_EQUIP;
	//PlayerStatus* desc = (PlayerStatus*)info;
	//desc->GetInfo()->ropeCount += 5;

	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->SetRopeCount(desc->GetRopeCount() + 5);
	price = 0;
	isDialogCol = false;
	dialog->SetCol(isDialogCol);
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
		isDialogCol = true;
		if (0 >= price)
		{
			//Equip(player->GetPlayerStatus());
			SetDestroy();
		}
	}

	else if (auto player = obj->GetType<Character>())
	{

	}
}
