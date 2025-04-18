#include "TreasureChest.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"
#include "CameraManager.h"
#include "IncreaseGold.h"
#include "Tile.h"

TreasureChest::TreasureChest()
{
}

TreasureChest::~TreasureChest()
{
}

HRESULT TreasureChest::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 700, 100 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 0;
	return S_OK;
}

void TreasureChest::Update(float TimeDelta)
{

}

void TreasureChest::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 0); // 임의값
}

void TreasureChest::Release()
{

}

void TreasureChest::Equip()
{

}

void TreasureChest::Equip(void* info)
{
}

void TreasureChest::Equip(GameObject* owner)
{
}

void TreasureChest::UnEquip()
{
}

void TreasureChest::UnEquip(void* info)
{
}

void TreasureChest::Use()
{
}

void TreasureChest::Use(void* info)
{
}

void TreasureChest::Detect(GameObject* obj)
{
	if (auto temp = obj->GetType<Tile>())
	{
		return;
	}

	IncreaseGold* temp = new IncreaseGold();
	ObjectManager::GetInstance()->AddObject(RENDER_ITEM, temp);
	temp->SetPos(Pos); // 가라
	SetDestroy();
	// 플레이어 어택이나 WorldItem이면 부숴져라.
	//if (auto player = obj->GetType<Character>() || auto worldItem = obj->GetType<Character>() )
	//{
	//	SetDestroy();
	//}
}

void TreasureChest::DropMove(float TimeDelta)
{
}

void TreasureChest::DropMoveX(float TimeDelta)
{
}

void TreasureChest::DropMoveY(float TimeDelta)
{
}
