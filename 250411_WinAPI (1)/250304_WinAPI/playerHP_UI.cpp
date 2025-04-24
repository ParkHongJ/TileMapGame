#include "pch.h"
#include "playerHP_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "ObjectManager.h"
#include "Character.h"
#include "ImageManager.h"

playerHP_UI::playerHP_UI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
	{
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF(D2D1::ColorF::White));
	}
}

HRESULT playerHP_UI::Init()
{
	playerLifeImage = ImageManager::GetInstance()->FindImage("playerLife");
	playerLifeRunsOutImage = ImageManager::GetInstance()->FindImage("playerLifeRunsOut");

	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	playerHP_value = playerStat->GetPlayerHP();
	pastHP_value = playerHP_value;
	Pos = { WINSIZE_X * (1.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	isAlive = true;

	return S_OK;
}

void playerHP_UI::Update(float TimeDelta)
{
	UI::Update(TimeDelta);

	playerHP_value = playerStat->GetPlayerHP();
	if (playerHP_value <= 0)
		isAlive = false;
	
	//if(playerStat->PicktheHP || LostTheHP)
	//if (KeyManager::GetInstance()->IsOnceKeyDown('H'))
	if(playerHP_value != pastHP_value)
	{
		RequestOpaqueChange();
		pastHP_value = playerHP_value;
	}
	
}

void playerHP_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (playerLifeImage)
	{
		playerLifeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		std::wstring hpText = std::to_wstring(playerHP_value);
		RenderText(renderTarget, hpText, Pos.x + 15, Pos.y + 5, defaultOpacity);

		if (!isAlive)				//»ç¸ÁÇÒ °æ¿ì ±×³É ¶Ç·ÇÇØÁü
		{
			playerLifeImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, 1.0f);
			playerLifeRunsOutImage->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, 1.0f);
		}
	}
}

void playerHP_UI::Release()
{
}
