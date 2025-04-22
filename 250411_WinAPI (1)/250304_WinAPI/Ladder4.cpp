#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Ladder4.h"

Ladder4::Ladder4()
{

}

Ladder4::~Ladder4()
{

}

HRESULT Ladder4::Init()
{
    Pos = { 1100,510 };
    objectName = OBJECTNAME::LADDER;
    return S_OK;
}

void Ladder4::Update(float TimeDelta)
{
}

void Ladder4::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 3); // ÀÓÀÇ°ª
}

void Ladder4::Release()
{
}
