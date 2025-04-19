#include "pch.h"
#include "Gun.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"
#include "GunBullet.h"
#include "CameraManager.h"
Gun::Gun() : bulletCnt(5), fireCoolTime(0.f), fireMaxCoolTime(1.f), fireOffset({0.f, 0.f}), isFire(false), fireCurFrameX(0)
{
	 // �̱��� Gun

	
}

Gun::~Gun()
{
}

HRESULT Gun::Init()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 700, 100 };

	moveDir = { 0,1 };

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 3;
	bulletCnt = 5;
	fireImage = ImageManager::GetInstance()->FindImage("fx_big");

	fireOffset = { 100.f, 0.f };
	frameSpeed = 60.f;
	//fireOffset = 
	SetDrop();
	return S_OK;
}

void Gun::Update(float TimeDelta)
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		Use();
	}

	fireCoolTime -= TimeDelta;
	FrameUpdate(TimeDelta);
	if (fireCoolTime <= 0.f)
	{
		fireCoolTime = 0.f;
	}

	// 
	__super::DropMove(TimeDelta);
	//__super::Update(TimeDelta);
}

void Gun::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	//if (ItemState::STATE_EQUIP == itemState)
	if (isFire)
	{
		fireImage->FrameRender(renderTarget, cameraPos.x + fireOffset.x, cameraPos.y + fireOffset.y, fireCurFrameX, 0, 0.5f, 0.5f); // ���ǰ�
	}

	holdImage->FrameRender(renderTarget, cameraPos.x , cameraPos.y , curFrameIndexX, curFrameIndexY); // ���ǰ�
	//else
	{
		//dropImage->FrameRender(renderTarget, Pos.x, Pos.y, curFrameIndexX, curFrameIndexY); // ���ǰ�
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

}

void Gun::Equip(GameObject* owner)
{
	__super::Equip(owner);
}

void Gun::UnEquip()
{
	__super::UnEquip();
	SetDrop();
}

void Gun::UnEquip(void* info)
{
	__super::UnEquip(info);
}

void Gun::Use()
{
	if (0 < bulletCnt)
	{
		--bulletCnt;
		fireCoolTime = fireMaxCoolTime;
		isFire = true;
		Fire();
	}
	//Pos = { 1000, 1000 }; // Test
}

void Gun::Use(void* info)
{
	if (0 < bulletCnt)
	{
		--bulletCnt;
		fireCoolTime = fireMaxCoolTime;
		isFire = true;
		Fire();
	}

	//__super::Use(info);
}

void Gun::Detect(GameObject* obj)
{
	objectRenderId = RENDER_HOLD;
	interactState = INTERACTSTATE::INTERACT_UNABLE;
	//__super::Detect(obj);

}

void Gun::FrameUpdate(float TimeDelta)
{
	if (isFire)
	{
		elipsedTime += frameSpeed * TimeDelta;
		fireCurFrameX = elipsedTime;
		if (3 < fireCurFrameX)
		{
			fireCurFrameX = elipsedTime = 0;
			isFire = false;
		}
	}
}

void Gun::Fire()
{
	for (int i = 0; i < 5; ++i)
	{
		GunBullet* temp = new GunBullet();
		ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_ITEM, temp);
		temp->SetPos(Pos);
		temp->SetDrop();
	}
	
	//fx_big
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
