#include "pch.h"
#include "ShopInteractUI.h"

HRESULT ShopInteractUI::Init()
{
	shopBoxImage = ImageManager::GetInstance()->FindImage("");
	priceBoxImage = ImageManager::GetInstance()->FindImage("");
	goldImage = ImageManager::GetInstance()->FindImage("playerGoldImage");
	interactKeyImage = ImageManager::GetInstance()->FindImage("");
	shopDialogBoxImage = ImageManager::GetInstance()->FindImage("");

	return S_OK;
}

void ShopInteractUI::Release()
{
}

void ShopInteractUI::Update(float TimeDelta)
{
}

void ShopInteractUI::Render(ID2D1HwndRenderTarget* renderTarget)
{
}
