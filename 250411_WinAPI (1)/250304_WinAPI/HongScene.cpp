#include "HongScene.h"
#include "ObjectManager.h"
#include "CollisionManager.h"
#include "DummyHongPlayer.h"
#include "Image.h"
#include "GameManager.h"


HRESULT HongScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	CollisionManager::GetInstance()->Init();
	//ObjectManager::GetInstance()->Init();

	ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
	ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Border", L"Textures/border_main.png", renderTarget);

	GameManager::GetInstance()->Init("Data/map1.tilemap");

	//ObjectManager::GetInstance()->AddObject(RENDERORDER::RENDER_PLAYER, new DummyHongPlayer);

	return S_OK;
}

void HongScene::Release()
{
	ObjectManager::GetInstance()->Release();
	CollisionManager::GetInstance()->Release();
}

void HongScene::Update(float TimeDelta)
{
	//ObjectManager::GetInstance()->Update(TimeDelta);
	//CollisionManager::GetInstance()->Update(TimeDelta);
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
