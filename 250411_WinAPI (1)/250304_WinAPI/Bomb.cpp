#include "Bomb.h"
#include "Image.h"
#include "Collider.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init()
{

	dropImage = ImageManager::GetInstance()->FindImage("items");
	holdImage = ImageManager::GetInstance()->FindImage("items");

	Pos = { 200, 200 };

	//BoxCollider* col = new BoxCollider({ 0,0 }, { 100,100 }, this);
	return S_OK;
}

void Bomb::Update(float TimeDelta)
{
	__super::Update(TimeDelta);
	//DropMove(TimeDelta);

}

void Bomb::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//__super::Render(renderTarget);
	if (true == isHold)
	{
		holdImage->FrameRender(renderTarget, Pos.x, Pos.y, 0, 5); // 임의값
	}

	else
	{
		dropImage->FrameRender(renderTarget, Pos.x, Pos.y, 0, 5); // 임의값
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
	Pos = { 1000, 1000 }; // Test
}

void Bomb::DropMove(float TimeDelta)
{
	__super::DropMove(TimeDelta);
}
