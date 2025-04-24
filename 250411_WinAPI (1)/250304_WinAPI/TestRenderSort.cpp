#include "pch.h"
#include "TestRenderSort.h"
#include "Image.h"
#include "ImageManager.h"

HRESULT TestRenderSort::Init()
{
	Pos = { 0,0 };
	image = ImageManager::GetInstance()->FindImage("monstersbig03");
	return S_OK;
}

void TestRenderSort::Update(float TimeDelta)
{
}

void TestRenderSort::Render(ID2D1RenderTarget* renderTarget)
{
	image->Render(renderTarget, 0, 0);
}

void TestRenderSort::Release()
{
}
