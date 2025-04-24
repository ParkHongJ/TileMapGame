#include "pch.h"
#include "playerRope_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "ObjectManager.h"
#include "Character.h"
#include "ImageManager.h"


playerRope_UI::playerRope_UI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF::White);
}

HRESULT playerRope_UI::Init()
{
	playerRopeImage = ImageManager::GetInstance()->FindImage("playerRopeImage");
	
	Pos = { WINSIZE_X * (5.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	//playerRope_value = playerStat->GetRopeCount();

	return S_OK;
}

void playerRope_UI::Release()
{
}

void playerRope_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);
	//playerRope_value = playerStat->GetRopeCount();
	if (KeyManager::GetInstance()->IsOnceKeyDown('R'))
		RequestOpaqueChange();
}

void playerRope_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (playerRopeImage)
	{
		playerRopeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		std::wstring hpText = std::to_wstring(playerRope_value);
		RenderText(renderTarget, hpText, Pos.x + 15, Pos.y + 5, defaultOpacity);
	}
}
