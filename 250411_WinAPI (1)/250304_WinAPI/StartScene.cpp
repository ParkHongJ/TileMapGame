#include "pch.h"
#include "StartScene.h"
#include "ImageManager.h"
#include "KeyManager.h"
#include "Image.h"
#define D3DX_PI 3.141592f

HRESULT StartScene::Init(ID2D1HwndRenderTarget* renderTarget)
{
	// UI (TaeGwan)
	ImageManager::GetInstance()->AddImage("Menu_Title", L"Textures/UI/Menu/menu_title.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Menu_Title_Char", L"Textures/UI/Menu/menu_titlegal.png", renderTarget);
	ImageManager::GetInstance()->AddImage("BlackBG", L"Textures/UI/Menu/blackbg.png", renderTarget);
	ImageManager::GetInstance()->AddImage("TorchEffect", L"Textures/fx_small.png", 8, 8, renderTarget);
	ImageManager::GetInstance()->AddImage("FumeEffect", L"Textures/fx_small.png", 8, 8, renderTarget);
	ImageManager::GetInstance()->AddImage("Buttons", L"Textures/hud_controller_buttons.png", 10, 10, renderTarget);
	ImageManager::GetInstance()->AddImage("Char_Menu", L"Textures/menu_charsel.png", renderTarget);
	ImageManager::GetInstance()->AddImage("Char_Menu_Disp", L"Textures/menu_disp.png",1,3, renderTarget);
	ImageManager::GetInstance()->AddImage("Char_Menu_Door", L"Textures/menu_chardoor.png",2,2, renderTarget);




	
	menu_title = ImageManager::GetInstance()->FindImage("Menu_Title");
	menu_title_char = ImageManager::GetInstance()->FindImage("Menu_Title_Char");
	blackbg = ImageManager::GetInstance()->FindImage("BlackBG");
	torchEffectBack = ImageManager::GetInstance()->FindImage("TorchEffect");
	torchEffectFront = ImageManager::GetInstance()->FindImage("TorchEffect");
	fumeEffect = ImageManager::GetInstance()->FindImage("FumeEffect");
	enter = ImageManager::GetInstance()->FindImage("Buttons");


	screenTitleScale = { {1080 / 1920.f }, {500 / 1080.f} };
	screenCharScale = { {500 / 1024.f}, {500 / 1024.f} };

	menuTitlePos = { WINSIZE_X / 2, WINSIZE_Y / 2 };
	menuTitleCharPos = { 250, 250 };
	flamePos = { 290,180 };

	opacity = 1.0f;
	isFadingOut = false;
	fadeTimer = 0.0f;
	opacity = 1.0f;
	flameFrameIndex = 1;



	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&pDWriteFactory));

	pDWriteFactory->CreateTextFormat(
		L"Tekton-Bold",  // 또는 "Tekton Pro", "Arial", 원하는 폰트 이름
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		48.0f,  // 폰트 크기
		L"en-us",
		&pTextFormat
	);


	pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	// 텍스트 브러시
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush);


	return S_OK;
}

void StartScene::Release() 
{
	torchParticles.clear();
	torchParticles.shrink_to_fit();

	fumes.clear();
	fumes.shrink_to_fit();

}

void StartScene::Update(float TimeDelta)
{
	auto km = KeyManager::GetInstance();
	if (km->IsOnceKeyDown(VK_RETURN)) {
		isFadingOut = true;
		fadeTimer = 0.0f;
	}

	if (isFadingOut) {
		fadeTimer += TimeDelta;
		float t = min(fadeTimer / fadeDuration, 1.0f);
		opacity = 1.0f - t;
		if (t >= 1.0f) {
			isFadingOut = false;
			opacity = 0.0f;

			SceneManager::GetInstance()->ChangeScene("선택");
		}
	}

	globalTime += TimeDelta;
	fumeSpawnTimer += TimeDelta;
	if (fumeSpawnTimer >= FUME_SPAWN_INTERVAL) {
		fumeSpawnTimer = 0.0f;

		// Torch: 생성 수 줄임
		int torchCount = 2 + rand() % 2;
		for (int i = 0; i < 5; ++i) {
			TorchParticle tp;
			tp.pos = { flamePos.x + (rand() % 30 - 15), flamePos.y + (rand() % 20 - 10) };

			// 약간 왼쪽으로 기운 부채꼴 방향 (중심 각도 100도, 60도 범위)
			float angle = (100.0f + (rand() % 60 - 30)) * (D3DX_PI / 180.0f); // 70~130도
			float speed = 70.0f + rand() % 20;

			tp.velocity = { cosf(angle) * speed, -fabsf(sinf(angle) * speed) };  // 위로 빠르게
			float t = (rand() / (float)RAND_MAX);
			tp.initialScale = 0.15f + powf(1.0f - t, 2.0f) * (0.45f - 0.001f);
			tp.scale = tp.initialScale;
			tp.opacity = 1.0f;
			tp.lifetime = 2.8f + (rand() % 10) / 100.f;
			tp.age = 0.0f;
			tp.frameIndex = 1;
			tp.waveOffset = (rand() % 628) / 100.0f;
			torchParticles.push_back(tp);
		}

		// Fume: 생성 수 줄임
		int fumeCount = 1;
		for (int i = 0; i < 5; ++i) {
			FumeParticle f;
			f.pos = { flamePos.x + (rand() % 21 - 10), flamePos.y + (rand() % 11 - 5) };

			// 약간 왼쪽으로 기운 부채꼴 방향 (60도 범위, 중심 각도는 110도)
			float angle = (100.0f + (rand() % 60 - 30)) * (D3DX_PI / 180.0f); // 80~140도 범위
			float speed = 70.0f + rand() % 20;
			f.velocity = { cosf(angle) * speed, -fabsf(sinf(angle) * speed) };

			f.scale = 0.1f;
			f.opacity = 1.0f;
			f.lifetime = 3.0f + (rand() % 50) / 50.f;
			f.age = 0.0f;
			f.waveOffset = (rand() % 628) / 100.0f;
			fumes.push_back(f);
		}
	}

	for (auto& tp : torchParticles) {
		float waveX = sin(globalTime * 10.0f + tp.waveOffset) * 2.0f;
		tp.pos.x += (tp.velocity.x + waveX) * TimeDelta;
		tp.pos.y += tp.velocity.y * TimeDelta;
		tp.age += TimeDelta;

		tp.opacity = (1.0f - (tp.age / tp.lifetime)) * opacity;
		tp.scale = tp.initialScale * expf(-3.0f * (tp.age / tp.lifetime));

		// 프레임 변화: 수명 비율에 따라 점점 더 작은 프레임으로
		int frameProgress = static_cast<int>((tp.age / tp.lifetime) * 7); // 0 ~ 6
		tp.frameIndex = min(7, 1 + frameProgress); // 프레임 (1~7)
	}

	for (auto& f : fumes) {
		float waveX = sin(globalTime * 2.0f + f.waveOffset) * 4.0f;
		f.pos.x += (f.velocity.x + waveX) * TimeDelta;
		f.pos.y += f.velocity.y * TimeDelta;
		f.age += TimeDelta;
		f.opacity = (1.0f - (f.age / f.lifetime)) * opacity;
		f.scale += TimeDelta * 0.05f;
	}
	fumes.erase(std::remove_if(fumes.begin(), fumes.end(),
		[](const FumeParticle& f) { return f.age > f.lifetime; }),
		fumes.end());
}

void StartScene::LateUpdate(float TimeDelta) {}

void StartScene::Render(ID2D1HwndRenderTarget* renderTarget)
{
	if (blackbg) 
		blackbg->Render(renderTarget, menuTitlePos.x, menuTitlePos.y, 1.0f, 1.0f, 1.0f);

	if (menu_title) 
		menu_title->Render(renderTarget, menuTitlePos.x, menuTitlePos.y, screenTitleScale.x, screenTitleScale.y, opacity);

	if (torchEffectBack) 
	{
		for (auto& tp : torchParticles) 
		{
			torchEffectFront->FrameRender(renderTarget, tp.pos.x, tp.pos.y, tp.frameIndex, 0, tp.scale, tp.scale,tp.opacity, false);
		}
	}

	if (menu_title_char)
		menu_title_char->Render(renderTarget, menuTitleCharPos.x, menuTitleCharPos.y, screenCharScale.x, screenCharScale.y, opacity);

	if (torchEffectFront) 
	{
		for (auto& f : fumes) 
		{
			fumeEffect->FrameRender(renderTarget, f.pos.x, f.pos.y, 1, 2, f.scale, f.scale,f.opacity, false);
		}
		for (auto& tp : torchParticles) 
		{
			torchEffectFront->FrameRender(renderTarget, tp.pos.x, tp.pos.y, tp.frameIndex, 0, tp.scale, tp.scale,tp.opacity, false);
		}
	}

	const wchar_t* text = L"Press      to Begin";

	float alpha = 0.5f + 0.5f * sin(globalTime * 2.0f); // 0 ~ 1 사이 반짝임
	pWhiteBrush->SetOpacity(alpha * opacity);

	D2D1_RECT_F layoutRect = D2D1::RectF(
		500, 380, 500 + 400, 380 + 100
	);


	renderTarget->DrawText(
		text,
		wcslen(text),
		pTextFormat,
		&layoutRect,
		pWhiteBrush,
		D2D1_DRAW_TEXT_OPTIONS_NONE,
		DWRITE_MEASURING_MODE_NATURAL
	);

	if (enter)
	{
		enter->FrameRender(renderTarget, 677, 430, 1, 9, 0.15f, 0.15f, alpha);
	}





}

bool StartScene::returnIsSceneEnd()
{
	if (opacity <= 0.0f) return true;
	else return false;
}
