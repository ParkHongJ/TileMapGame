#include "pch.h"
#include "PlayerSelectScene.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Image.h"

#define D3DX_PI 3.141592f
HRESULT PlayerSelectScene::Init(ID2D1HwndRenderTarget* renderTarget)
{


    blackbg = ImageManager::GetInstance()->FindImage("BlackBG");
    charMenu = ImageManager::GetInstance()->FindImage("Char_Menu");

    buttonZ = ImageManager::GetInstance()->FindImage("Buttons");
    buttonESC = ImageManager::GetInstance()->FindImage("Buttons");


    charDoorBack2 = charDoorBack = charDoor = ImageManager::GetInstance()->FindImage("Char_Menu_Door");
    charMenuTopBack = charMenuTop = charMenuBottom = ImageManager::GetInstance()->FindImage("Char_Menu_Disp");
   
    torchEffectFront = ImageManager::GetInstance()->FindImage("TorchEffect");
    fumeEffect = ImageManager::GetInstance()->FindImage("FumeEffect");

    flamePos = { 208,158 };

    doorY = 180;
    sceneOpacity = 1.0f;
    screencharMenuScale = { {1080 / 1920.f }, {500 / 1080.f} };
    charMenuPos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
    screenTopBottomScale = { {450/1408.f},{ 225/768.f} };



    isFadingOut = false;
    fadeTimer = 0.0f;
    flameFrameIndex = 1;




    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

    pDWriteFactory->CreateTextFormat(
        L"Tekton-Bold",  // �Ǵ� "Tekton Pro", "Arial", ���ϴ� ��Ʈ �̸�
        NULL,
        DWRITE_FONT_WEIGHT_BOLD,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        36.0f,  // ��Ʈ ũ��
        L"en-us",
        &pTextFormat
    );


    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    // �ؽ�Ʈ �귯��

    float r = 83 / 255.0f;
    float g = 56 / 255.0f;
    float b = 28 / 255.0f;
    float a = 1.0f;
    renderTarget->CreateSolidColorBrush(D2D1::ColorF(r,g,b,a), &pWhiteBrush);


    return S_OK;
}

void PlayerSelectScene::Release()
{
}

void PlayerSelectScene::Update(float TimeDelta)
{


    auto km = KeyManager::GetInstance();

    if (buttonZ)
    {
        if (km->IsOnceKeyDown('Z'))
        {
            buttonZ = nullptr;
        }

    }
    
    if (km->IsOnceKeyDown('Z') && !buttonZ) {
        isFadingOut = true;
        fadeTimer = 0.0f;
    }

    if (isFadingOut) {
        fadeTimer += TimeDelta;
        float t = min(fadeTimer / fadeDuration, 1.0f);
        sceneOpacity = 1.0f - t;
        if (t >= 1.0f) {
            isFadingOut = false;
            sceneOpacity = 0.0f;

            SceneManager::GetInstance()->ChangeScene("����");
        }
    }


    if (!buttonZ)
    {
        doorMoveTimer += TimeDelta;
        float t = min(doorMoveTimer / doorMoveDuration, 1.0f); // 0 ~ 1 ����
        doorY = doorStartY * (1.0f - t) + doorTargetY * t; // ���� ����

        if (t >= 1.0f)
        {
            isDoorOpening = false;
            doorY = doorTargetY;
        }
    }


    globalTime2 += TimeDelta;
    fumeSpawnTimer2 += TimeDelta;

    if (fumeSpawnTimer2 >= FUME_SPAWN_INTERVAL2) {
        fumeSpawnTimer2 = 0.0f;

        // Torch: ���� �� ����
        int torchCount = 2 + rand() % 2;
        for (int i = 0; i < 3; ++i) {
            TorchParticle tp;
            tp.pos = { flamePos.x + (rand() % 20 - 15), flamePos.y + (rand() % 15 - 10) };

            // �ణ �������� ��� ��ä�� ���� (�߽� ���� 100��, 60�� ����)
            float angle = (100.0f + (rand() % 50 - 30)) * (D3DX_PI / 180.0f); // 70~130��
            float speed = 70.0f + rand() % 20;

            tp.velocity = { cosf(angle) * speed, -fabsf(sinf(angle) * speed) };  // ���� ������
            float t = (rand() / (float)RAND_MAX);
            tp.initialScale = 0.05f + powf(1.0f - t, 2.0f) * (0.2f - 0.001f);
            tp.scale = tp.initialScale;
            tp.opacity = 1.0f;
            tp.lifetime = 1.3f + (rand() % 10) / 100.f;
            tp.age = 0.0f;
            tp.frameIndex = 1;
            tp.waveOffset = (rand() % 628) / 100.0f;
            torchParticles2.push_back(tp);
        }

        // Fume: ���� �� ����
        int fumeCount = 1;
        for (int i = 0; i < 2; ++i) {
            FumeParticle f;
            f.pos = { flamePos.x + (rand() % 21 - 10), flamePos.y + (rand() % 11 - 5) };

            // �ణ �������� ��� ��ä�� ���� (60�� ����, �߽� ������ 110��)
            float angle = (100.0f + (rand() % 50 - 30)) * (D3DX_PI / 180.0f); // 80~140�� ����
            float speed = 70.0f + rand() % 20;
            f.velocity = { cosf(angle) * speed, -fabsf(sinf(angle) * speed) };

            f.scale = 0.05f;
            f.opacity = 0.7f;
            f.lifetime = 1.5f + (rand() % 50) / 50.f;
            f.age = 0.0f;
            f.waveOffset = (rand() % 628) / 100.0f;
            fumes2.push_back(f);
        }
    }

    for (auto& tp : torchParticles2) {
        float waveX = sin(globalTime2 * 10.0f + tp.waveOffset) * 2.0f;
        tp.pos.x += (tp.velocity.x + waveX) * TimeDelta;
        tp.pos.y += tp.velocity.y * TimeDelta;
        tp.age += TimeDelta;

        tp.opacity = (1.0f - (tp.age / tp.lifetime)) * sceneOpacity;
        tp.scale = tp.initialScale * expf(-3.0f * (tp.age / tp.lifetime));

        // ������ ��ȭ: ���� ������ ���� ���� �� ���� ����������
        int frameProgress = static_cast<int>((tp.age / tp.lifetime) * 7); // 0 ~ 6
        tp.frameIndex = min(7, 1 + frameProgress); // ������ (1~7)
    }

    for (auto& f : fumes2) {
        float waveX = sin(globalTime2 * 2.0f + f.waveOffset) * 4.0f;
        f.pos.x += (f.velocity.x + waveX) * TimeDelta;
        f.pos.y += f.velocity.y * TimeDelta;
        f.age += TimeDelta;
        f.opacity = (1.0f - (f.age / f.lifetime)) * sceneOpacity;
        f.scale += TimeDelta * 0.05f;
    }
    fumes2.erase(std::remove_if(fumes2.begin(), fumes2.end(),
        [](const FumeParticle& f) { return f.age > f.lifetime; }),
        fumes2.end());







}

void PlayerSelectScene::LateUpdate(float TimeDelta)
{
}

void PlayerSelectScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
    if (blackbg)
    {
        blackbg->Render(renderTarget, charMenuPos.x, charMenuPos.y, 1.0f, 1.0f);
    }
    if (charDoorBack2)
    {
        charDoorBack2->FrameRender(renderTarget, 319, 180, 1, 1, 0.28f, 0.25f, sceneOpacity);
    }
    if (charDoorBack)
    {
        charDoorBack->FrameRender(renderTarget, 319, 180, 0, 1, 0.28f, 0.25f, sceneOpacity);

    }
    if (charDoor)
    {
        charDoor->FrameRender(renderTarget, 319, doorY, 0, 0, 0.28f, 0.25f, sceneOpacity);
    }

    if (charMenu)
    {
        charMenu->Render(renderTarget, charMenuPos.x, charMenuPos.y, screencharMenuScale.x, screencharMenuScale.y, sceneOpacity);
    }
    if (charMenuTopBack)
    {
        charMenuTopBack->FrameRender(renderTarget, WINSIZE_X / 2, 50, 0, 1, screenTopBottomScale.x * 0.8, screenTopBottomScale.y * 0.8, sceneOpacity);
    }
    if (charMenuTop)
    {
        charMenuTop->FrameRender(renderTarget, WINSIZE_X / 2, 50, 0, 0, screenTopBottomScale.x * 0.8, screenTopBottomScale.y * 0.8, sceneOpacity);
    }
    if (charMenuBottom)
    {
        charMenuBottom->FrameRender(renderTarget, WINSIZE_X/2, 450, 0, 2, screenTopBottomScale.x * 0.8, screenTopBottomScale.y * 0.8,sceneOpacity);
    }


    if (torchEffectFront)
    {
        for (auto& f : fumes2)
        {
            fumeEffect->FrameRender(renderTarget, f.pos.x, f.pos.y, 1, 2, f.scale, f.scale, f.opacity, false);
        }
        for (auto& tp : torchParticles2)
        {
            torchEffectFront->FrameRender(renderTarget, tp.pos.x, tp.pos.y, tp.frameIndex, 0, tp.scale, tp.scale, tp.opacity, false);
        }
    }
   



    if (buttonZ)
    {
        buttonZ->FrameRender(renderTarget,319, 180, 0, 9, 0.15f, 0.15f, sceneOpacity);
    }


    // TEXT

    float centerX = WINSIZE_X / 2;
    float imgFrameHeight = 768.0f / 3.0f;  // �� �������� ����
    float scaleY = screenTopBottomScale.y * 0.8f;
    float imgPosY = 50.0f;  // charMenuTop�� Y ��ġ

    float imgHeightScaled = imgFrameHeight * scaleY;

    // �ؽ�Ʈ�� ��� ���߱� ���� rect ���
    float textRectHeight = 100.0f;
    float textCenterY = imgPosY; // �̹����� �߾� �������� �������ǹǷ�, �̰� �߾� Y

    D2D1_RECT_F layoutRect = D2D1::RectF(
        centerX - 300.0f,                 // ����
        textCenterY - textRectHeight / 2.0f, // ����: �߾ӿ��� ���� ����
        centerX + 300.0f,                 // ������
        textCenterY + textRectHeight / 2.0f  // �Ʒ���: �߾ӿ��� ���� �Ʒ���
    );

    // �ؽ�Ʈ ���ڿ�
    const wchar_t* text = L"CHOOSE ADVENTURER";

    pWhiteBrush->SetOpacity(sceneOpacity);

    // �ؽ�Ʈ ���
    renderTarget->DrawText(
        text,
        wcslen(text),
        pTextFormat,
        &layoutRect,
        pWhiteBrush,
        D2D1_DRAW_TEXT_OPTIONS_NONE,
        DWRITE_MEASURING_MODE_NATURAL
    );






}
