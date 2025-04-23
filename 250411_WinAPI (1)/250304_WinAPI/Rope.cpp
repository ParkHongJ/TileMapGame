#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Rope.h"

Rope::Rope()
{
    BoxCollider* col = new BoxCollider({ 0,0 }, { 64, 64 }, CollisionMaskType::ITEM, this);
    interactState = INTERACTSTATE::INTERACT_ABLE;
    objectName = OBJECTNAME::ROPE;
    objectScale = { GAME_TILE_SIZE / (ATLAS_TILE_SIZE * 1.25f), GAME_TILE_SIZE / (ATLAS_TILE_SIZE * 1.25f) };
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
    //if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
    //{
    //    //if()
    //}
}

void Rope::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, curFrameX, curFrameY, objectScale.x, objectScale.y); // ÀÓÀÇ°ª
}

void Rope::Release()
{

}

void Rope::Detect(GameObject* obj)
{

}
