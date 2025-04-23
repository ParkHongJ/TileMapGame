#include "pch.h"
#include "sandGlass_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "Character.h"

sandGlass_UI::sandGlass_UI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF::White);
}


HRESULT sandGlass_UI::Init()
{
	image = ImageManager::GetInstance()->FindImage("sandGlassImage");
	Pos = { WINSIZE_X * (22.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	return S_OK;
}

void sandGlass_UI::Release()
{
}

void sandGlass_UI::Update(float TimeDelta)
{
	if(0 < playerStat->GetPlayerHP())
		playTime += TimeDelta;
}

void sandGlass_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (image)
	{
		image->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
		std::wstring hpText = std::to_wstring(playTime);
		RenderText(renderTarget, hpText, WINSIZE_X/2.0f, WINSIZE_Y/2.0f, defaultOpacity);
	}
}
