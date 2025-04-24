#include "pch.h"
#include "playerBomb_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "ObjectManager.h"
#include "Character.h"
#include "ImageManager.h"

playerBomb_UI::playerBomb_UI(ID2D1RenderTarget* renderTarget)
{
	if(renderTarget)
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF::White);
}

HRESULT playerBomb_UI::Init()
{
	playerBombImage = ImageManager::GetInstance()->FindImage("playerBombImage");
	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	playerBomb_value = playerStat->GetBombCount();
	pastBomb_value = playerBomb_value;
	Pos = { WINSIZE_X * (3.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void playerBomb_UI::Release()
{
}

void playerBomb_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);

	if (playerStat->GetPlayerHP() <= 0)
	{
		playerBombImage = nullptr;
	}

	playerBomb_value = playerStat->GetBombCount();
	//if(playerStat->pickedTheBomb || useTheBomb)
 	//if (KeyManager::GetInstance()->IsOnceKeyDown('B'))
	if (playerBomb_value != pastBomb_value)
	{
		RequestOpaqueChange();
		pastBomb_value = playerBomb_value;
	}

}

void playerBomb_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (playerBombImage)
	{
		playerBombImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		std::wstring hpText = std::to_wstring(playerBomb_value);
		RenderText(renderTarget, hpText, Pos.x + 15, Pos.y + 5, defaultOpacity);
	}
}

