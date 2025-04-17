#include "TaeScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Character.h"
#include "TimerManager.h"
#include "CollisionManager.h"
#include "Collider.h"
#include "GameManager.h"
#include "ObjectManager.h"


HRESULT TaeScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
  
    /*backGround = new Image();
    if (FAILED(backGround->Init(TEXT("Image/Character/background.bmp"), 2048, 2048)))
    {
        MessageBox(g_hWnd,
            TEXT("backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }*/

    yellow = new Character();

    if (yellow)
        yellow->Init();


    // Collider register

     yellowCollider = new BoxCollider(
        { 0.0f , 0.0f },     // Offset
        {50.0f, 50.0f},  // 
        yellow           
    );
    

    yellowCollider->Update(0.f);             // 초기 업데이트로 Min/Max 계산


    ImageManager::GetInstance()->AddImage("CaveTile", L"Textures/CaveTile.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoDown", L"Textures/CaveDecoDown.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoTop", L"Textures/CaveDecoTop.png", renderTarget);
    ImageManager::GetInstance()->AddImage("CaveDecoRight", L"Textures/CaveDecoRight.png", renderTarget);


    GameManager::GetInstance()->Init("Data/map1.tilemap");



    return S_OK;
}

void TaeScene::Release()
{

    if (yellow)
    {
        yellow->Release();
        delete yellow;
        yellow = nullptr;
    }

}

void TaeScene::Update(float TimeDelta)
{
    if (yellow)
        yellow->Update(TimeDelta);

}

void TaeScene::Render(ID2D1HwndRenderTarget* renderTarget)
{

    if (yellow)
        yellow->Render(renderTarget);


    if (yellowCollider)
        yellowCollider->DebugRender(renderTarget);


}


