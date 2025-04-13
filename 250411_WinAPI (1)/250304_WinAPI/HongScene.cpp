#include "HongScene.h"
#include "ObjectManager.h"
HRESULT HongScene::Init()
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

void HongScene::Render(HDC hdc)
{
	ObjectManager::GetInstance()->Render(hdc);
}
