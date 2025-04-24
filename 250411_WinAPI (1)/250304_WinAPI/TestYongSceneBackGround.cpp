#include "pch.h"
#include "TestYongSceneBackGround.h"
#include "Image.h"
#include "ImageManager.h"


TestYongSceneBackGround::TestYongSceneBackGround()
{

}

HRESULT TestYongSceneBackGround::Init()
{
    image = ImageManager::GetInstance()->FindImage("BackBuffer");
    return S_OK;
}

void TestYongSceneBackGround::Update(float TimeDelta)
{

}

void TestYongSceneBackGround::Render(ID2D1RenderTarget* renderTarget)
{
    image->Render(renderTarget, 0, 0);
    //
}

void TestYongSceneBackGround::Release()
{

}

TestYongSceneBackGround::~TestYongSceneBackGround()
{

}
