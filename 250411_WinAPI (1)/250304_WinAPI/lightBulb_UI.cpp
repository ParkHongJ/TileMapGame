#include "pch.h"
#include "lightBulb_UI.h"
#include "Image.h"
#include "ImageManager.h"

HRESULT lightBulb_UI::Init()
{
	image = ImageManager::GetInstance()->FindImage("lightBulbImage");
	Pos = { WINSIZE_X * (23.5f / x_pos_divide_factor), WINSIZE_Y * (1.0f / 12.0f) };

	return S_OK;
}

void lightBulb_UI::Release()
{
}

void lightBulb_UI::Update(float TimeDelta)
{

}

void lightBulb_UI::Render(ID2D1RenderTarget* renderTarget)
{
	if (image)
		image->Render(renderTarget, Pos.x, Pos.y, 1.0f, 1.0f, defaultOpacity);
}
