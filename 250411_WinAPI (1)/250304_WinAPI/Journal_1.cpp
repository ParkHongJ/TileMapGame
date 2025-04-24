#include "pch.h"
#include "Journal_1.h"
#include "Image.h"

#include "ImageManager.h"
HRESULT Journal_1::Init()
{
    imageRatio = ResolutionRatio();
    isInteract = false;
    imageJournalPage = ImageManager::GetInstance()->FindImage("journalPage");
    imageJournalPhoto = ImageManager::GetInstance()->FindImage("journal_1_Photo");
    Pos = { WINSIZE_X/2, WINSIZE_Y/2 };
    PhotoPos = { Pos.x, Pos.y - (imageJournalPhoto->GetHeight() * 0.125f)};

    return S_OK;
}

void Journal_1::Release()
{
}

void Journal_1::Update(float TimeDelta)
{
}

void Journal_1::Render(ID2D1RenderTarget* renderTarget)
{
    if(isInteract)
    {
        if (imageJournalPage)
            imageJournalPage->Render(renderTarget,
                Pos.x, Pos.y,
                imageRatio.x, imageRatio.y);

        if (imageJournalPage && imageJournalPhoto)
            imageJournalPhoto->Render(renderTarget,
                PhotoPos.x, PhotoPos.y,
                imageRatio.x, imageRatio.y);
    }
}
