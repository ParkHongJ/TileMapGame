#include "pch.h"
#include "UI.h"
#include "Image.h"

HRESULT UI::Init()
{

	return S_OK;
}

void UI::Release()
{
	textColorBrush.Reset();
	textFormat.Reset();
	writeFactory.Reset();
}

void UI::Update(float TimeDelta)
{
	if (requestOpaqueChange)
	{
		SetOpaque();
		requestOpaqueChange = false;
	}
	if (isOpaque)
	{
		opacityTimer += TimeDelta;
		if (opacityTimer >= opacityDuration)
		{
			DisableOpaque();
		}
	}
}

void UI::Render(ID2D1HwndRenderTarget* renderTarget)
{
}

void UI::SetOpaque()
{
	isOpaque = true;
	defaultOpacity = 1.0f;
	opacityTimer = 0.0f;
}

void UI::DisableOpaque()
{
	isOpaque = false;
	defaultOpacity = 0.5f;
	opacityTimer = 0.0f;
}

FPOINT UI::ResolutionRatio()
{
	FPOINT ratio = { 0.0f, 0.0f };
	Image* standardImage = ImageManager::GetInstance()->FindImage("GameOverPage");
	ratio.x = WINSIZE_X / static_cast<float>(standardImage->GetWidth());
	ratio.y = WINSIZE_Y / static_cast<float>(standardImage->GetHeight());

	return ratio;
}

HRESULT UI::InitTextRenderer(ID2D1HwndRenderTarget* renderTarget, const wchar_t* fontFamily, float fontSize, D2D1::ColorF color)
{
	if (!renderTarget)
		return E_FAIL;

	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		&writeFactory
	);
	if (FAILED(hr)) return hr;

	hr = writeFactory->CreateTextFormat(
		fontFamily,
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,
		L"ko-KR",
		&textFormat
	);
	if (FAILED(hr)) return hr;

	hr = renderTarget->CreateSolidColorBrush(
		color,
		&textColorBrush
	);

	return hr;
}

void UI::RenderText(ID2D1HwndRenderTarget* renderTarget, const std::wstring& text, float x, float y)
{
	if (renderTarget && textFormat && textColorBrush)
	{
		D2D1_RECT_F layoutRect = D2D1::RectF(x, y, x + 200.0f, y + 50.0f); // 텍스트 영역 설정
		renderTarget->DrawText(
			text.c_str(),
			text.length(),
			textFormat.Get(),
			layoutRect,
			textColorBrush.Get()
		);
	}
}

void UI::RenderText(ID2D1HwndRenderTarget* renderTarget, const std::wstring& text, D2D1_RECT_F layoutRect)
{
	if (renderTarget && textFormat && textColorBrush)
	{
		renderTarget->DrawText(
			text.c_str(),
			text.length(),
			textFormat.Get(),
			layoutRect,
			textColorBrush.Get()
		);
	}
}

UI::UI()
{
	x_pos_divide_factor = 25.0f;

	defaultOpacity = 0.5f;		//기본 반투명
	opacityDuration = 3.0f;		//3초간 또렷해짐
	opacityTimer = 0.0f;		//3초 카운트용 변수
	isOpaque = false;			//기본적으로 반투명
	requestOpaqueChange = false;
}
