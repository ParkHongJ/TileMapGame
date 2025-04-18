#include "pch.h"
#include "HongScene.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "DummyHongPlayer.h"
#include "Image.h"
#include "GameManager.h"
#include "CameraManager.h"


HRESULT HongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	CollisionManager::GetInstance()->Init();
	CollisionManager::GetInstance()->Clear();

	ObjectManager::GetInstance()->ClearObjects();

	ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Border", L"Textures/border_main.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Effect", L"Textures/fx_small.png", renderTarget);

	GameManager::GetInstance()->Init("hongScene");

	//ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_PLAYER, new DummyHongPlayer);

	return S_OK;
}

void HongScene::Release()
{
	//ObjectManager::GetInstance()->Release();
	CollisionManager::GetInstance()->Release();
}

void HongScene::Update(float TimeDelta)
{
	ObjectManager* objMgr =  ObjectManager::GetInstance();//->Update(TimeDelta);
	int a = 10;
	//CollisionManager::GetInstance()->Update(TimeDelta);

	CameraManager::GetInstance()->Update(TimeDelta);
}

void HongScene::LateUpdate(float TimeDelta)
{
	//ObjectManager::GetInstance()->LateUpdate(TimeDelta);
}

void HongScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	//ObjectManager::GetInstance()->Render(renderTarget);
	
#ifdef _DEBUG
	//CollisionManager::GetInstance()->DebugRender(renderTarget);
#endif _DEBUG
}
