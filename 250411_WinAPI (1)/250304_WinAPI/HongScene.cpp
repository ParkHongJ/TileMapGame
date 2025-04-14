#include "HongScene.h"
#include "ObjectManager.h"
HRESULT HongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	ObjectManager::GetInstance()->Init();
	return S_OK;
}

void HongScene::Release()
{
	ObjectManager::GetInstance()->Release();
}

void HongScene::Update(float TimeDelta)
{
	ObjectManager::GetInstance()->Update(TimeDelta);
}

void HongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	ObjectManager::GetInstance()->Render(renderTarget);
}
