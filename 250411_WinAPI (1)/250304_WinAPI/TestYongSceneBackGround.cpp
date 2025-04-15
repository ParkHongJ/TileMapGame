#include "TestYongSceneBackGround.h"
#include "Image.h"

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

void TestYongSceneBackGround::Render(ID2D1HwndRenderTarget* renderTarget)
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
