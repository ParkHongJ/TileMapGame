#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "Character.h"
#include "CameraManager.h"
#include "IncreaseHP.h"
#include "ImageManager.h"
#include "ItemDialog.h";

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

	BoxCollider* col = new BoxCollider({ 0,0 }, { 50, 50 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	price = 1000;
	if (0 < price)
	{
		dialog = new ItemDialog();
		ObjectManager::GetInstance()->AddObject(RENDER_UI, dialog);
		dialog->SetPrice(price);
		dialog->SetPos(Pos);
	}

	return S_OK;
}

void IncreaseHP::Update(float TimeDelta)
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

void IncreaseHP::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 9, objectScale.x * 0.5f, objectScale.y * 0.5f); // ���ǰ�
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
	desc->PlusPlayerHP(1);;
	price = 0;
	isDialogCol = false;
	dialog->SetCol(isDialogCol);
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