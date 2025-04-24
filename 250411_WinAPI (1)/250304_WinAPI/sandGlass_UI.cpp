#include "pch.h"
#include "sandGlass_UI.h"
#include "Image.h"
#include "PlayerStatus.h"
#include "Character.h"
#include "ImageManager.h"

#include <iomanip>
#include <sstream>

sandGlass_UI::sandGlass_UI(ID2D1RenderTarget* renderTarget)
{
	if (renderTarget)
		InitTextRenderer(renderTarget, L"Consolas", 20.0f, D2D1::ColorF::White);
}


HRESULT sandGlass_UI::Init()
{
	image = ImageManager::GetInstance()->FindImage("sandGlassImage");
	Pos = { WINSIZE_X * (20.5f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	playerStat = ObjectManager::GetInstance()->GetPlayer()->GetPlayerStatus();
	playTimeMin = 0;
	playTimeSec = 0.0f;
	return S_OK;
}

void sandGlass_UI::Release()
{
}

void sandGlass_UI::Update(float TimeDelta)
{
	if(0 < playerStat->GetPlayerHP())
	{
		playTimeSec += TimeDelta;
		if (playTimeSec >= 60)
		{
			playTimeSec = 0;
			playTimeMin++;
		}
	}
	else
	{
		image = nullptr;
	}
}

void sandGlass_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (image)
	{
		image->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);

		std::wstringstream timeStream;
		timeStream << std::setw(2) << std::setfill(L'0') << playTimeMin << L":"
			<< std::setw(2) << std::setfill(L'0') << static_cast<int>(playTimeSec);
		std::wstring timeText = timeStream.str();

		RenderText(renderTarget, timeText, Pos.x + 15, Pos.y - 14, defaultOpacity);
	}
}
