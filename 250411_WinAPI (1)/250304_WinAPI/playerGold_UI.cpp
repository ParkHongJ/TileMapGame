#include "pch.h"
#include "playerGold_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "ObjectManager.h"
#include "Character.h"
#include "ImageManager.h"

playerGold_UI::playerGold_UI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
	{
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF(D2D1::ColorF::White));
	}
}

HRESULT playerGold_UI::Init()
{
	playerGoldImage = ImageManager::GetInstance()->FindImage("playerGoldImage");

	Pos = { WINSIZE_X * (18.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	playerGold_value = playerStat->GetGold();
	pastGold_value = playerGold_value;

	return S_OK;
}

void playerGold_UI::Release()
{
}

void playerGold_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);
	if (playerStat->GetPlayerHP() <= 0)
		playerGoldImage = nullptr;

	playerGold_value = playerStat->GetGold();
	//if (KeyManager::GetInstance()->IsOnceKeyDown('G'))
	if(pastGold_value != playerGold_value)
	{
		RequestOpaqueChange();
		pastGold_value = playerGold_value;
	}
}

void playerGold_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (playerGoldImage)
	{
		playerGoldImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		std::wstring hpText = std::to_wstring(playerGold_value);
		RenderText(renderTarget, hpText, Pos.x + 20, Pos.y - 10, defaultOpacity);
	}
}
