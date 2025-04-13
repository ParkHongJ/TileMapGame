#include "Character.h"

HRESULT Character::Init()
{
	ImageManager::GetInstance()->AddImage("Character_Iris", TEXT("Image/char_iris.bmp"), 512, 512);
	return S_OK;
}

void Character::Release()
{
}

void Character::Update(float TimeDelta)
{
}

void Character::Render(HDC hdc)
{
}
