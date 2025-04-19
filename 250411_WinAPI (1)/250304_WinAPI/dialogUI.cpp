#include "pch.h"
#include "dialogUI.h"
#include "Image.h"

HRESULT dialogUI::Init()
{
    imageBox = ImageManager::GetInstance()->FindImage("TextDialogBox");
    imageArrow = ImageManager::GetInstance()->FindImage("TextDialogArrow");
    Pos = { WINSIZE_X / 2.0f, (WINSIZE_Y / 2.0f) - (imageBox->GetHeight() / 2.0f) };
    arrowPos = { Pos.x, WINSIZE_Y/2.0f};
    isInteract = true;
    return S_OK;
}

void dialogUI::Release()
{
}

void dialogUI::Update(float TimeDelta)
{
}

void dialogUI::Render(ID2D1HwndRenderTarget* renderTarget)
{
    if(isInteract)
    {
        if (imageBox)
            imageBox->Render(renderTarget, Pos.x, Pos.y);
        if (imageArrow && imageBox)
            imageArrow->Render(renderTarget, arrowPos.x, arrowPos.y);
    }
}

float dialogUI::DistCaculate()
{
    return 0.0f;
}
