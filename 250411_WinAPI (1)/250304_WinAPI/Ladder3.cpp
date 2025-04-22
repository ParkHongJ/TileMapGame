#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Ladder3.h"

Ladder3::Ladder3()
{

}

Ladder3::~Ladder3()
{

}

HRESULT Ladder3::Init()
{
    Pos = { 1100,390 };
    objectName = OBJECTNAME::LADDER;
    return S_OK;
}

void Ladder3::Update(float TimeDelta)
{
}

void Ladder3::Render(ID2D1RenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 2); // ÀÓÀÇ°ª
}

void Ladder3::Release()
{
}
