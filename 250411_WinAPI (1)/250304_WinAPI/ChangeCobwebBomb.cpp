#include "pch.h"
#include "Image.h"
#include "Collider.h"
#include "ChangeCobwebBomb.h"
#include "Character.h"
#include "CameraManager.h"
#include "ImageManager.h"

ChangeCobwebBomb::ChangeCobwebBomb()
{
	
}

ChangeCobwebBomb::~ChangeCobwebBomb()
{

}

HRESULT ChangeCobwebBomb::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 800, 300 };

	//BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, this);
	BoxCollider* col = new BoxCollider({ 0,0 }, { 40, 40 }, CollisionMaskType::ITEM, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ONCE;
	interactState = INTERACTSTATE::INTERACT_ABLE;
	return S_OK;
}

void ChangeCobwebBomb::Update(float TimeDelta)
{
	DropMove(TimeDelta);
}

void ChangeCobwebBomb::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 2, 2, objectScale.x * 0.75f, objectScale.y * 0.75f); // ÀÓÀÇ°ª
}

void ChangeCobwebBomb::Release()
{
}

void ChangeCobwebBomb::Equip()
{
}

void ChangeCobwebBomb::Equip(void* info)
{
	itemState = ItemState::STATE_EQUIP;
	PlayerStatus* desc = (PlayerStatus*)info;
	desc->GetInfo()->IsCobweb = true;
}

void ChangeCobwebBomb::UnEquip()
{
}

void ChangeCobwebBomb::UnEquip(void* info)
{
}

void ChangeCobwebBomb::Use()
{
}

void ChangeCobwebBomb::DropMove(float TimeDelta)
{
}

void ChangeCobwebBomb::Detect(GameObject* obj)
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
