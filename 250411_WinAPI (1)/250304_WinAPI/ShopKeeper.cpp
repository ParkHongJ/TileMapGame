#include "pch.h"
#include "ShopKeeper.h"
#include "Image.h"
#include "CameraManager.h"
#include "Character.h"

HRESULT ShopKeeper::Init()
{
	shopkeeprImage = ImageManager::GetInstance()->FindImage("shopKeeper");
	objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE , GAME_TILE_SIZE / ATLAS_TILE_SIZE };
	Pos = { WINSIZE_X / 2.0f, WINSIZE_Y / 2.0f - 50.f};
	/*playerPos = ObjectManager::GetInstance()->GetPlayer()->GetPos();
	bPlayerPos_IsLeftSide = (playerPos.x < Pos.x) ? true : false;*/

	return S_OK;
}

void ShopKeeper::Release()
{
}

void ShopKeeper::Update(float TimeDelta)
{
	playerPos = ObjectManager::GetInstance()->GetPlayer()->GetPos();
	bPlayerPos_IsLeftSide = (playerPos.x < Pos.x) ? true : false;
}

void ShopKeeper::Render(ID2D1RenderTarget* renderTarget)
{
	FPOINT pos = Pos + CameraManager::GetInstance()->GetPos();
	if (shopkeeprImage)
	{
			shopkeeprImage->FrameRender(renderTarget, pos.x, pos.y, 0, 0, objectScale.x, objectScale.y, bPlayerPos_IsLeftSide);
	}
}