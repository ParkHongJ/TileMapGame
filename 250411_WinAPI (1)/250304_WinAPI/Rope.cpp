#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Rope.h"

Rope::Rope()
{
    objectScale = { GAME_TILE_SIZE / ATLAS_TILE_SIZE, GAME_TILE_SIZE / ATLAS_TILE_SIZE };
}

Rope::~Rope()
{

}

HRESULT Rope::Init()
{
    image = image = ImageManager::GetInstance()->FindImage("Tae_Player");
    // ´ó´óÀÌ 12, 9, ~ 15, 9 / 0, 12 ~ 9, 12
    Pos = { 700, 350 };
    return S_OK;
}

void Rope::Update(float TimeDelta)
{
    if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
    {

    }
    CurFrameX = 0;
    CurFrameY = 12;
}

void Rope::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 0, 12, objectScale.x, objectScale.y); // ÀÓÀÇ°ª
}

void Rope::Release()
{

}

void Rope::Detect(GameObject* obj)
{

}
