#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Ladder2.h"

Ladder2::Ladder2()
{

}

Ladder2::~Ladder2()
{

}

HRESULT Ladder2::Init()
{
    Pos = { 1100,270 };
    objectName = OBJECTNAME::LADDER;
    return S_OK;
}

void Ladder2::Update(float TimeDelta)
{
}

void Ladder2::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 1); // ÀÓÀÇ°ª
}

void Ladder2::Release()
{
}
