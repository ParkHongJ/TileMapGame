#include "TaeScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "KeyManager.h"

HRESULT TaeScene::Init()
{

    ImageManager::GetInstance()->AddImage("Tae_Background", TEXT("Image/Character/background.bmp"), 2048, 2048);

    backGround = new Image();
    if (FAILED(backGround->Init(TEXT("Image/Character/background.bmp"), 2048, 2048)))
    {
        MessageBox(g_hWnd,
            TEXT("backGround.bmp 생성 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }

    yellow = new Character();

    if (yellow)
        yellow->Init();



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

void TaeScene::Render(HDC hdc)
{
    backGround->Render(hdc, 0, 0);

    HPEN hPen = CreatePen(PS_SOLID, 10, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);


    MoveToEx(hdc, 0, 300, NULL);
    LineTo(hdc, 2048, 300);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    if (yellow)
        yellow->Render(hdc);

}



HRESULT Character::Init()
{
    ImageManager::GetInstance()->AddImage("Tae_Player", TEXT("Image/Character/char_yellow.bmp"), 2048, 2048, 16, 16, true, RGB(255, 0, 255));
    playerImage = ImageManager::GetInstance()->FindImage("Tae_Player");


    SetPos({ WINSIZE_X / 2,WINSIZE_Y / 2 - 10 });

    state = PlayerState::IDLE;
    dir = { 0.0f, 0.0f };

    // causally walking & idle
    currFrameInd = { 0,0 };

    currFrameInfo.startFrame = { 0,0 };
    currFrameInfo.endFrame = { 0,0 };
    currMaxFrameInd = { 9,0 };
    frameTime = 0.0f;

    speed = 200.f;
    attackSpeed = 3.0f;
    attackRate = 0.3f;

    isFlip = false;
    isFalling = false;
    isAttakcing = false;

    jumpPressed = false;
    attackPressed = false;



    return S_OK;
}

void Character::Release()
{
    if (playerImage)
    {
        ImageManager::GetInstance()->DeleteImage("Tae_Player");
        playerImage = nullptr;

    }
}

void Character::Update(float TimeDelta)
{
    KeyManager* km = KeyManager::GetInstance();


    // default
    state = PlayerState::IDLE;
    dir = { 0.0f , 0.0f };

   

    if (km->IsStayKeyDown('D'))
    {
        state = PlayerState::MOVE;
        isFlip = false;
        dir.x += 1;
        Pos.x += speed * dir.x * TimeDelta;

        
    }
    if (km->IsStayKeyDown('A'))
    {
        state = PlayerState::MOVE;
        isFlip = true;
        dir.x += -1;
        Pos.x += speed * dir.x * TimeDelta;
    }

    if (km->IsOnceKeyDown(VK_SPACE))
    {
        state = PlayerState::JUMP;
        jumpPressed = true;
    }

    switch (state)
    {
    case PlayerState::IDLE:
        break;
    case PlayerState::MOVE:
        currFrameInfo.startFrame = { 1,0 };
        currFrameInfo.endFrame = { 9,0 };

        break;
    }


    if (dir.x != 0)
    {
        frameTime += TimeDelta;

        if (frameTime >= ANIMATION_FRAME_TIME)
        {
            frameTime = 0.f;

            currFrameInd.x++;
            if (currFrameInd.x >= currFrameInfo.endFrame.x)
                currFrameInd.x = currFrameInfo.startFrame.x;
        }
    }
    else
    {
        currFrameInd.x = 0;
    }

}

void Character::Render(HDC hdc)
{
    TCHAR buf[256];
    wsprintf(buf, TEXT("Render Frame: %d, %d"), currFrameInd.x, currFrameInd.y);
    TextOut(hdc, 100, 200, buf, wcslen(buf));

    playerImage->FrameRender(hdc, Pos.x, Pos.y, currFrameInd.x, currFrameInd.y, isFlip, true);
    
}
