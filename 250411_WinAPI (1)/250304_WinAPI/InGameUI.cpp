#include "InGameUI.h"
#include "Image.h"

HRESULT InGameUI::Init(ID2D1HwndRenderTarget* renderTarget)
{
	ImageManager::GetInstance()->AddImage("playerLife", L"Textures/UI/Hud/playerLife.png", renderTarget);
	playerLifeImage = ImageManager::GetInstance()->FindImage("playerLife");

	ImageManager::GetInstance()->AddImage("playerLifeRunsOut", L"Textures/UI/Hud/playerLIfeRunsOut.png", renderTarget);
	playerLifeRunsOutImage = ImageManager::GetInstance()->FindImage("playerLifeRunsOut");

	ImageManager::GetInstance()->AddImage("bombImage", L"Textures/UI/Hud/bomb.png", renderTarget);
	bombImage = ImageManager::GetInstance()->FindImage("bombImage");

	ImageManager::GetInstance()->AddImage("ropeImage", L"Textures/UI/Hud/rope.png", renderTarget);
	ropeImage = ImageManager::GetInstance()->FindImage("ropeImage");

	ImageManager::GetInstance()->AddImage("currencyImage", L"Textures/UI/Hud/currency.png", renderTarget);
	currencyImage = ImageManager::GetInstance()->FindImage("currencyImage");

	ImageManager::GetInstance()->AddImage("sandGlassImage", L"Textures/UI/Hud/sandGlass.png", renderTarget);
	sandGlassImage = ImageManager::GetInstance()->FindImage("sandGlassImage");

	ImageManager::GetInstance()->AddImage("currencyBackground", L"Textures/UI/Hud/currencyBackground.png", renderTarget);
	currencyBackgroundImage = ImageManager::GetInstance()->FindImage("currencyBackground");

	ImageManager::GetInstance()->AddImage("lightBulb", L"Textures/UI/Hud/lightBulb.png", renderTarget);
	lightBulbImage = ImageManager::GetInstance()->FindImage("lightBulb");

	lifeCount = 3;
	bombCount = 0;
	ropeCount = 0;
	currencyCount = 0;
	currentStage = "";

	shouldDrawPlayerUI = true;

	float x_pos_divide_factor = 25.0f;

	lifeImagePos = { WINSIZE_X * (1.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	bombImagePos = { WINSIZE_X * (3.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	ropeImagePos = { WINSIZE_X * (5.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	currencyImagePos = { WINSIZE_X * (18.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	currencyBackgroundImagePos = { WINSIZE_X * (20.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	sandGlassImagePos = { WINSIZE_X * (22.0f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };
	lightBulbImagePos = { WINSIZE_X * (23.5f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void InGameUI::Release()
{

}

void InGameUI::Update(float TimeDelta)
{

}

void InGameUI::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if(shouldDrawPlayerUI)
	{
		if (playerLifeImage)
			playerLifeImage->Render(renderTarget, lifeImagePos.x, lifeImagePos.y);
		if (playerLifeImage && lifeCount == 0)
			playerLifeRunsOutImage->Render(renderTarget, lifeImagePos.x, lifeImagePos.y);
		if (bombImage)
			bombImage->Render(renderTarget, bombImagePos.x, bombImagePos.y);
		if (ropeImage)
			ropeImage->Render(renderTarget, ropeImagePos.x, ropeImagePos.y);
		if (currencyImage)
			currencyImage->Render(renderTarget, currencyImagePos.x, currencyImagePos.y);
		if (currencyBackgroundImage)
			currencyBackgroundImage->Render(renderTarget, currencyBackgroundImagePos.x, currencyBackgroundImagePos.y);
		if (sandGlassImage)
			sandGlassImage->Render(renderTarget, sandGlassImagePos.x, sandGlassImagePos.y);
		if (lightBulbImage)
			lightBulbImage->Render(renderTarget, lightBulbImagePos.x, lightBulbImagePos.y);
	}
}
