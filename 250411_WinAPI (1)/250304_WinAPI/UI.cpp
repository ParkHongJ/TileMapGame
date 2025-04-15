#include "UI.h"
#include "Image.h"

HRESULT UI::Init(ID2D1HwndRenderTarget* renderTarget)
{
	/*ImageManager::GetInstance()->AddImage("playerLife", L"Textures/UI/Hud/playerLife.png", renderTarget);
	lifeImage = ImageManager::GetInstance()->FindImage("playerLife");

	ImageManager::GetInstance()->AddImage("bombImage", L"Textures/UI/Hud/bomb.png", );
	bombImage = ImageManager::GetInstance()->FindImage("bombImage");

	ImageManager::GetInstance()->AddImage("ropeImage", L"Textures/UI/Hud/rope.png", );
	ropeImage = ImageManager::GetInstance()->FindImage("ropeImage");

	ImageManager::GetInstance()->AddImage("currencyImage", L"Texture/UI/Hud/currency.png", );
	currencyImage = ImageManager::GetInstance()->FindImage("currencyImage");

	ImageManager::GetInstance()->AddImage("sandGlassImage", L"Texture/UI/Hud/sandGlass.png", );
	sandGlassImage = ImageManager::GetInstance()->FindImage("sandglassImage");

	ImageManager::GetInstance()->AddImage("lightBulb", L"Texture/UI/Hud/lightBulb.png", );
	lightBulb = ImageManager::GetInstance()->FindImage("lightBulb");

	lifeCount = 3;
	bombCount = 0;
	ropeCount = 0;
	currencyCount = 0;*/

	return E_FAIL;
}

void UI::Release()
{

}

void UI::Update(float TimeDelta)
{

}

void UI::Render(ID2D1HwndRenderTarget* renderTarget)
{

}
