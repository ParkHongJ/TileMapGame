#include "pch.h"
#include "Character.h"
#include "Collider.h"
#include "CameraManager.h"
#include "Image.h"
#include "Ladder1.h"

Ladder1::Ladder1()
{

}

Ladder1::~Ladder1()
{

}

HRESULT Ladder1::Init()
{
    Pos = { 1100,150 };
    objectName = OBJECTNAME::LADDER;
    return S_OK;
}

void Ladder1::Update(float TimeDelta)
{
}

void Ladder1::Render(ID2D1HwndRenderTarget* renderTarget)
{
    FPOINT cameraPos = CameraManager::GetInstance()->GetPos() + Pos;
    image->FrameRender(renderTarget, cameraPos.x, cameraPos.y, 4, 0, objectScale.x, objectScale.y); // юсюг╟╙
}

void Ladder1::Release()
{
}
