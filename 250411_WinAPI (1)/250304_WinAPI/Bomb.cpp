#include "pch.h"
#include "Bomb.h"
#include "Image.h"
#include "Collider.h"
#include "CameraManager.h"
#include "CollisionManager.h"

Bomb::Bomb()
{
	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	itemState = ItemState::STATE_UNEQUIP;
	interactState = INTERACTSTATE::INTERACT_ABLE;

	curFrameIndexX = 0;
	curFrameIndexY = 5;

	startFrameIndexX = 0;
	startFrameIndexY = 5;

	endFrameIndexX = 2;
	endFrameIndexY = 5;
	frameSpeed = 3.334f;

	BoxCollider* col = new BoxCollider({ 0,0 }, { 70, 70 }, this);
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init()
{
	Pos = { 200, 200 };
	return S_OK;
}

void Bomb::Update(float TimeDelta)
{
	FrameUpdate(TimeDelta);
	

	explosionTime -= TimeDelta;
	animationTime -= TimeDelta;

	if (0.f >= animationTime)
	{
		frameSpeed = 5.f;
		//frameSpeed += TimeDelta; // 가라
	}

	if (0.f >= explosionTime)
	{
		Explosion();
	}
}

void Bomb::Render(ID2D1HwndRenderTarget* renderTarget)
{
	FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;

	if (ItemState::STATE_EQUIP == itemState)
	{
		holdImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameIndexX, curFrameIndexY);
	}

	else
	{
		dropImage->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameIndexX, curFrameIndexY); // 임의값
	}
}

void Bomb::Release()
{

}

void Bomb::Equip()
{
	__super::Equip();
}

void Bomb::Equip(void* info)
{
	__super::Equip(info);
}

void Bomb::UnEquip()
{
	__super::UnEquip();
}

void Bomb::UnEquip(void* info)
{
	__super::UnEquip(info);
}

void Bomb::Use()
{
	//Pos = { 1000, 1000 }; // Test
}

void Bomb::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}

void Bomb::Detect(GameObject* obj)
{

	//set
}

void Bomb::FrameUpdate(float TimeDelta)
{

	elipsedTime += frameSpeed * TimeDelta;
	curFrameIndexX = int(elipsedTime);

	if (curFrameIndexX > endFrameIndexX)
	{
		curFrameIndexX = elipsedTime = 0;
	}
}

void Bomb::Explosion()
{
	vector<GameObject*> temp;
	CollisionManager::GetInstance()->GetObjectsInCircle(Pos, 200.f, &temp);

	for (auto& iter : temp)
	{
		iter->SetDestroy();
	}

	SetDestroy();
}
