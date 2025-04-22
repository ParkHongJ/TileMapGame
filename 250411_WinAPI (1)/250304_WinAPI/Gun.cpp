#include "pch.h"
#include "Gun.h"
#include "Image.h"
#include "Collider.h"
#include "PlayerStatus.h"
#include "GunBullet.h"
#include "CameraManager.h"
Gun::Gun() : bulletCnt(5), fireCoolTime(0.f), fireMaxCoolTime(1.f), fireOffset({0.f, 0.f}), isFire(false), fireCurFrameX(0)
{
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
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

	BoxCollider* col = new BoxCollider({ 0,0 }, { 70,30 }, CollisionMaskType::WORLDOBJECT, this);

	itemState = ItemState::STATE_UNEQUIP;
	itemType = ItemType::TYPE_ALWAYS;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	endFrameIndexX = startFrameIndexX = curFrameIndexX = 0;
	endFrameIndexY = startFrameIndexY = curFrameIndexY = 3;
	bulletCnt = 100;
	fireImage = ImageManager::GetInstance()->FindImage("fx_big");

	fireOffset = { 50.f, -5.f };
	frameSpeed = 60.f;
	//fireOffset = 
	//SetDrop();

	acceleration = { 0, 0 };  // 가속도
	totalForce = { 0.f,0.f };

	useGravity = true;

	bPhysics = true;
	holdOffset = { 20.f, 20.f };
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
		isFire = false;
		//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
		//{
		//	Use();
		//}
	}

	FrameUpdate(TimeDelta);

	__super::DropMove(TimeDelta);

	if (isFlip)
	{
		fireOffset.x = -80.f;
	}

	else
	{
		fireOffset.x = 80.f;
	}
	//__super::Update(TimeDelta);
}

void Gun::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	//if (ItemState::STATE_EQUIP == itemState)
	if (isFire)
	{
		fireImage->FrameRender(renderTarget, cameraPos.x + fireOffset.x, cameraPos.y + fireOffset.y, fireCurFrameX, 0, objectScale.x * 0.5f, objectScale.y * 0.5f, isFlip); // 임의값
	}
	
	holdImage->FrameRender(renderTarget, cameraPos.x , cameraPos.y , curFrameIndexX, curFrameIndexY, objectScale.x , objectScale.y , isFlip); // 임의값
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
	if (0 < bulletCnt && !isFire)
	{
		--bulletCnt;
		fireCoolTime = fireMaxCoolTime;
		isFire = true;
		Fire();
	}
}

void Gun::Use(void* info)
{
	if (0 < bulletCnt && !isFire)
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
	FPOINT offset = { 50.f,0.f };

	if (isFlip)
	{
		offset.x *= -1;
	}

	float angle = 20.f;

	for (int i = 0; i < 5; ++i)
	{
		GunBullet* temp = new GunBullet();
		ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_ITEM, temp);
		
		float spreadRadius = 30.f;

		// 원 안의 랜덤 위치
		float randAngle = RandomRange(0.f, 360.f); // 도 단위
		float randDist = RandomRange(0.f, spreadRadius);

		float rad = randAngle * (3.141592f / 180.f);
		FPOINT randomOffset = {
			cosf(rad) * randDist,
			sinf(rad) * randDist
		};

		FPOINT spawnPos = Pos + offset + randomOffset;
		temp->SetPos(spawnPos);

		if (isFlip)
		{
			temp->SetDrop(1500.f, angle + 180.f);
		}

		else
		{
			temp->SetDrop(1500.f, angle);
		}
		
		angle -= 10.f;
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
