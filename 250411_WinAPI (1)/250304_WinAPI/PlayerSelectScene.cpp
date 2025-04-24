#include "pch.h"
#include "PlayerSelectScene.h"
#include "GameManager.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Image.h"
#include "SceneManager.h"
#define D3DX_PI 3.141592f
HRESULT PlayerSelectScene::Init(ID2D1RenderTarget* renderTarget)
{

    characterKeys = {
    "char_yellow", "char_white", "char_violet", "char_red", "char_pink",
    "char_orange", "char_olive", "char_magenta", "char_lime", "char_lemon",
    "char_khaki", "char_iris", "char_hired", "char_green", "char_gray",
    "char_gold", "char_eggchild", "char_cyan", "char_cinnabar", "char_cerulean",
    "char_blue", "char_black"
    };



    displayCharacter = ImageManager::GetInstance()->FindImage("char_yellow");
    blackbg = ImageManager::GetInstance()->FindImage("BlackBG");
    charMenu = ImageManager::GetInstance()->FindImage("Char_Menu");

    buttonZ = ImageManager::GetInstance()->FindImage("Buttons");
    buttonESC = ImageManager::GetInstance()->FindImage("Buttons");

    selectButtonRight =selectButtonLeft = ImageManager::GetInstance()->FindImage("Char_Button");

    charDoorBack2 = charDoorBack = charDoor = ImageManager::GetInstance()->FindImage("Char_Menu_Door");
    charMenuTopBack = charMenuTop = charMenuBottom = ImageManager::GetInstance()->FindImage("Char_Menu_Disp");
   
    torchEffectFront = ImageManager::GetInstance()->FindImage("TorchEffect");
    fumeEffect = ImageManager::GetInstance()->FindImage("FumeEffect");

    flamePos = { 208,158 };
    currFilp = false;
    doorY = 180;
    sceneOpacity = 1.0f;
    screencharMenuScale = { {1080 / 1920.f }, {500 / 1080.f} };
    charMenuPos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
    screenTopBottomScale = { {450/1408.f},{ 225/768.f} };

    selectButtonLeftPos = {262, 184};
    selectButtonRightPos = {374, 184,};

    newCharCome = false;
    isFadingOut = false;
    fadeTimer = 0.0f;
    flameFrameIndex = 1;
    selectedNum = 0;

    newCharCome = false;
    currCharInd = { 0,0 };
    charSelectBuffTime = 0.4f;
    currSelectBuffTime = 0.0f;
    displayCharacterPos = { 321,180 };

    DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

    pDWriteFactory->CreateTextFormat(
        L"Tekton-Bold",  // 또는 "Tekton Pro", "Arial", 원하는 폰트 이름
        NULL,
        DWRITE_FONT_WEIGHT_BOLD,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        36.0f,  // 폰트 크기
        L"en-us",
        &pTextFormat
    );


    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    // 텍스트 브러시

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

            // key 값 전달
            GameManager::GetInstance()->SetPlayerImageKey(characterKeys[selectedNum]);
            SceneManager::GetInstance()->ChangeScene("게임");
            //GameManager::GetInstance()->TravelToNextScene();
        }
    }

    if (currSelectBuffTime > 0.0f)
    {
        currSelectBuffTime -= TimeDelta;
    }



    if (currSelectBuffTime <= 0.0f && !isSwitchingCharacter && !isFadingOut)
    {
        if (km->IsOnceKeyDown(VK_LEFT))
        {
            selectedNum = (selectedNum - 1 + characterKeys.size()) % characterKeys.size();
            isSwitchingCharacter = true;
            switchToLeft = true;
            currCharInd = { 1, 0 }; // 달리기 시작 프레임
            runAnimTimer = 0.0f;
            currFilp = true;
            selectButtonLeftOffset = -selectButtonInputAmplitude;
            selectButtonInputTimerL = selectButtonInputDuration;
            currSelectBuffTime = charSelectBuffTime;  // 쿨타임 리셋

            wchar_t debugMsg[128];
            swprintf_s(debugMsg, L"[VK_LEFT] selectedNum: %d (%s)\n", selectedNum, characterKeys[selectedNum]);
            OutputDebugStringW(debugMsg);
        }

        if (km->IsOnceKeyDown(VK_RIGHT))
        {
            selectedNum = (selectedNum + 1 + characterKeys.size()) % characterKeys.size();
             isSwitchingCharacter = true;
            switchToLeft = false;
            currCharInd = { 1, 0 }; // 달리기 시작 프레임
            runAnimTimer = 0.0f;
            currFilp = false;
            selectButtonRightOffset = +selectButtonInputAmplitude;
            selectButtonInputTimerR = selectButtonInputDuration;
            currSelectBuffTime = charSelectBuffTime;  // 쿨타임 리셋
            wchar_t debugMsg[128];
            swprintf_s(debugMsg, L"[VK_LEFT] selectedNum: %d (%s)\n", selectedNum, characterKeys[selectedNum]);
            OutputDebugStringW(debugMsg);
        }
    }



    if (isSwitchingCharacter) {
        runAnimTimer += TimeDelta;

        // 도착 체크 먼저
        if (newCharCome && (
            (currFilp && displayCharacterPos.x - 321 <= -TimeDelta * 500.f) ||
            (!currFilp && displayCharacterPos.x - 321 >= TimeDelta * 500.f))) {

            displayCharacterPos.x = 321;
            currCharInd = { 0, 0 };
            isSwitchingCharacter = false;
            newCharCome = false;
            currSelectBuffTime = charSelectBuffTime;
            return;
        }

        // 도착 안 했으면 이동
        displayCharacterPos.x += TimeDelta * (currFilp ? -500.f : 500.f);

        if (runAnimTimer >= runFrameTime) {
            runAnimTimer = 0.0f;
            currCharInd.x++;

            if (currCharInd.x == 4) {

                displayCharacter = ImageManager::GetInstance()->FindImage(characterKeys[selectedNum]);
                newCharCome = true;
                displayCharacterPos.x = currFilp ? 500 : 200;
            }
        }
    }



    selectButtonOscillateTime += TimeDelta;



    if (!buttonZ)
    {
        doorMoveTimer += TimeDelta;
        float t = min(doorMoveTimer / doorMoveDuration, 1.0f); // 0 ~ 1 사이
        doorY = doorStartY * (1.0f - t) + doorTargetY * t; // 선형 보간

        if (t >= 1.0f)
        {
            isDoorOpening = false;
            doorY = doorTargetY;
        }
    }



    if (selectButtonInputTimerL > 0.0f)
    {
        selectButtonInputTimerL -= TimeDelta;
        float t = 1.0f - (selectButtonInputTimerL / selectButtonInputDuration);
        selectButtonLeftOffset *= (1.0f - t);
    }
    else selectButtonLeftOffset = 0.0f;

    if (selectButtonInputTimerR > 0.0f)
    {
        selectButtonInputTimerR -= TimeDelta;
        float t = 1.0f - (selectButtonInputTimerR / selectButtonInputDuration);
        selectButtonRightOffset *= (1.0f - t);
    }
    else selectButtonRightOffset = 0.0f;



    globalTime2 += TimeDelta;
    fumeSpawnTimer2 += TimeDelta;

    if (fumeSpawnTimer2 >= FUME_SPAWN_INTERVAL2) {
        fumeSpawnTimer2 = 0.0f;

        // Torch: 생성 수 줄임
        int torchCount = 2 + rand() % 2;
        for (int i = 0; i < 3; ++i) {
            TorchParticle tp;
            tp.pos = { flamePos.x + (rand() % 20 - 15), flamePos.y + (rand() % 15 - 10) };

            // 약간 왼쪽으로 기운 부채꼴 방향 (중심 각도 100도, 60도 범위)
            float angle = (100.0f + (rand() % 50 - 30)) * (D3DX_PI / 180.0f); // 70~130도
            float speed = 70.0f + rand() % 20;

            tp.velocity = { cosf(angle) * speed, -fabsf(sinf(angle) * speed) };  // 위로 빠르게
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

        // Fume: 생성 수 줄임
        int fumeCount = 1;
        for (int i = 0; i < 2; ++i) {
            FumeParticle f;
            f.pos = { flamePos.x + (rand() % 21 - 10), flamePos.y + (rand() % 11 - 5) };

            // 약간 왼쪽으로 기운 부채꼴 방향 (60도 범위, 중심 각도는 110도)
            float angle = (100.0f + (rand() % 50 - 30)) * (D3DX_PI / 180.0f); // 80~140도 범위
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

        // 프레임 변화: 수명 비율에 따라 점점 더 작은 프레임으로
        int frameProgress = static_cast<int>((tp.age / tp.lifetime) * 7); // 0 ~ 6
        tp.frameIndex = min(7, 1 + frameProgress); // 프레임 (1~7)
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

void PlayerSelectScene::Render(ID2D1RenderTarget* renderTarget)
{
    if (blackbg)
    {
        blackbg->Render(renderTarget, charMenuPos.x, charMenuPos.y, 1.0f, 1.0f);
    }
   
    if (charDoorBack2)
    {
        charDoorBack2->FrameRender(renderTarget, 319, 180, 1, 1, 0.28f, 0.25f, sceneOpacity);
    }
    if (!buttonZ && displayCharacter)
    {
         
        displayCharacter->FrameRender(renderTarget, displayCharacterPos.x, displayCharacterPos.y, currCharInd.x, currCharInd.y, 0.4f, 0.4f,sceneOpacity, currFilp);
        charMenu->Render(renderTarget, charMenuPos.x, charMenuPos.y, screencharMenuScale.x, screencharMenuScale.y, sceneOpacity);

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

    if (!buttonZ)
    {
        float oscillateOffset = sin(selectButtonOscillateTime * 7.0f) * selectButtonOscillateAmplitude;

        if (selectButtonLeft)
        {
            float x = selectButtonLeftPos.x - oscillateOffset - selectButtonLeftOffset;
            selectButtonLeft->FrameRender(renderTarget, x, selectButtonLeftPos.y, 0, 0, 0.4f, 0.4f, sceneOpacity, true);
        }

       

        if (selectButtonRight)
        {
            float x = selectButtonRightPos.x + oscillateOffset + selectButtonRightOffset;
            selectButtonRight->FrameRender(renderTarget, x, selectButtonRightPos.y, 0, 0, 0.4f, 0.4f, sceneOpacity, false);
        }

    }


    // TEXT

    float centerX = WINSIZE_X / 2;
    float imgFrameHeight = 768.0f / 3.0f;  // 한 프레임의 세로
    float scaleY = screenTopBottomScale.y * 0.8f;
    float imgPosY = 50.0f;  // charMenuTop의 Y 위치

    float imgHeightScaled = imgFrameHeight * scaleY;

    // 텍스트를 가운데 맞추기 위한 rect 계산
    float textRectHeight = 100.0f;
    float textCenterY = imgPosY; // 이미지는 중앙 기준으로 렌더링되므로, 이게 중앙 Y

    D2D1_RECT_F layoutRect = D2D1::RectF(
        centerX - 300.0f,                 // 왼쪽
        textCenterY - textRectHeight / 2.0f, // 위쪽: 중앙에서 절반 위로
        centerX + 300.0f,                 // 오른쪽
        textCenterY + textRectHeight / 2.0f  // 아래쪽: 중앙에서 절반 아래로
    );

    // 텍스트 문자열
    const wchar_t* text = L"CHOOSE ADVENTURER";

    pWhiteBrush->SetOpacity(sceneOpacity);

    // 텍스트 출력
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
