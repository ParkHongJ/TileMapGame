#include "pch.h"
#include "Gun.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"
#include "GunBullet.h"
#include "CameraManager.h"
Gun::Gun() : bulletCnt(5), fireCoolTime(0.f), fireMaxCoolTime(1.f), fireOffset({0.f, 0.f}), isFire(false), fireCurFrameX(0)
{
	 // 미구현 Gun

	
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

	BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, CollisionMaskType::WORLDOBJECT, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 3;
	bulletCnt = 100;
	fireImage = ImageManager::GetInstance()->FindImage("fx_big");

	fireOffset = { 100.f, 0.f };
	frameSpeed = 60.f;
	//fireOffset = 
	//SetDrop();

	holdOffset = { 0.f, 20.f };
	objectName = OBJECTNAME::GUN;
	return S_OK;
}

void Gun::Update(float TimeDelta)
{
	fireCoolTime -= TimeDelta;
	if (fireCoolTime <= 0.f)
	{
		fireCoolTime = 0.f;
	}

	if (0.f >= fireCoolTime)
	{
		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
		{
			Use();
		}
	}

	FrameUpdate(TimeDelta);

	__super::DropMove(TimeDelta);

	if (isFlip)
	{
		fireOffset.x = -100.f;
	}

	else
	{
		fireOffset.x = 100.f;
	}
	//__super::Update(TimeDelta);
}

void Gun::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	//if (ItemState::STATE_EQUIP == itemState)
	if (isFire)
	{
		fireImage->FrameRender(renderTarget, cameraPos.x + fireOffset.x, cameraPos.y + fireOffset.y, fireCurFrameX, 0, 0.5f, 0.5f, isFlip); // 임의값
	}

	holdImage->FrameRender(renderTarget, cameraPos.x , cameraPos.y , curFrameIndexX, curFrameIndexY, 0.75f, 0.75f, isFlip); // 임의값
	//else
	{
		//dropImage->FrameRender(renderTarget, Pos.x, Pos.y, curFrameIndexX, curFrameIndexY); // 임의값
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
	//SetDrop();
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
	if (IsPlayerDropItem(obj) || 0.f == velocity.x || 0.f == velocity.y)
	{
		return;
	}

	if (OBJECTNAME::TILE == obj->GetObjectName())
	{
		return;
	}

	obj->SetDestroy();
	//objectRenderId = RENDER_HOLD;
	//interactState = INTERACTSTATE::INTERACT_UNABLE;
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
	FPOINT offset = { 150.f,0.f };

	if (isFlip)
	{
		offset.x *= -1;
	}

	float angle = 30.f; // 30 ~ -30

	for (int i = 0; i < 5; ++i)
	{
		GunBullet* temp = new GunBullet();
		ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_ITEM, temp);
		
		temp->SetPos(Pos + offset);

		if (isFlip)
		{
			temp->SetDrop(1300.f, angle + 180.f);
		}

		else
		{
			temp->SetDrop(1300.f, angle);
		}
		
		angle -= 15.f;
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
