#include "playerBomb_UI.h"
#include "Image.h"
#include "PlayerStatus.h"

HRESULT playerBomb_UI::Init()
{
	playerBombImage = ImageManager::GetInstance()->FindImage("playerBombImage");
	playerStat = new PlayerStatus();
	playerBomb_value = playerStat->GetBombCount();
	Pos = { WINSIZE_X * (3.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void playerBomb_UI::Release()
{
}

void playerBomb_UI::Update(float TimeDelta)
{
	playerBomb_value = playerStat->GetBombCount();
}

void playerBomb_UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (playerBombImage)
		playerBombImage->Render(renderTarget, Pos.x, Pos.y);
}
