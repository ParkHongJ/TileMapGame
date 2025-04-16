#include "Bomb.h"
#include "Image.h"

Bomb::Bomb()
{
}

Bomb::~Bomb()
{
}

HRESULT Bomb::Init()
{

	image = ImageManager::GetInstance()->FindImage("items");
	return S_OK;
}

void Bomb::Update(float TimeDelta)
{

}

void Bomb::Render(ID2D1HwndRenderTarget* renderTarget)
{
	__super::Render(renderTarget);
}

void Bomb::Release()
{

}

void Bomb::Equip()
{

}

void Bomb::Equip(void* info)
{

}

void Bomb::UnEquip()
{

}

void Bomb::UnEquip(void* info)
{

}

void Bomb::Use()
{

}
