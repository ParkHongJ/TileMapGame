#include "pch.h"
#include "ShopInteractUI.h"
#include "Image.h"

ShopInteractUI::ShopInteractUI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
	{
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF(D2D1::ColorF::White));
	}
}

HRESULT ShopInteractUI::Init()
{
	goldImage = ImageManager::GetInstance()->FindImage("playerGoldImage");
	interactKeyImage = ImageManager::GetInstance()->FindImage("");
	shopDialogBoxImage = ImageManager::GetInstance()->FindImage("shopDialogBox");
	Pos = { WINSIZE_X / 2.0f, WINSIZE_Y / 2.0f };		//������ RectPos.X/2.0f, RectPos.y
	goldPos = { Pos.x - (shopDialogBoxImage->GetWidth()*0.3f), Pos.y + 7 };							//������ RectPos.X/2.0f, RectPos.y
	price = 15000;		//�����ۿ��� ���� �޾ƿ���
	collisionConatact = false;	//�������̶� �÷��̾ ��Ҵ���
	return S_OK;
}

void ShopInteractUI::Release()
{
}

void ShopInteractUI::Update(float TimeDelta)
{
}

void ShopInteractUI::Render(ID2D1RenderTarget* renderTarget)
{
	if(collisionConatact)
	{
		if (shopDialogBoxImage && goldImage)
		{
			shopDialogBoxImage->Render(renderTarget, Pos.x, Pos.y);
			goldImage->Render(renderTarget, goldPos.x, goldPos.y);
			std::wstring hpText = std::to_wstring(price);
			RenderText(renderTarget, hpText, goldPos.x + 20, goldPos.y - 10, 1.0f);
		}
	}
}
