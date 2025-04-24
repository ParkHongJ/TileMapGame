#include "pch.h"
#include "ItemDialog.h"
#include "Image.h"
#include "ImageManager.h"
#include "CameraManager.h"

ItemDialog::ItemDialog()
{
}

ItemDialog::~ItemDialog()
{
}

HRESULT ItemDialog::Init()
{
	DialogImage = ImageManager::GetInstance()->FindImage("shopDialogBox");
	MoneyImage = ImageManager::GetInstance()->FindImage("currency_GameOver");
	Pos = { 400, 200 };
	MoneyOffset = { -50, 10 };
	TextOffset = { -20, -10 };
	MoneyScale = { 0.65f, 0.65f };
	return S_OK;
}

void ItemDialog::Release()
{

}

void ItemDialog::Update(float TimeDelta)
{
	isCol ? opacity += opacitySpeed * TimeDelta : opacity -= opacitySpeed * TimeDelta;
	//isCol ? Textopacity += opacitySpeed * TimeDelta : Textopacity -= opacitySpeed * TimeDelta;
	//isCol ? Moneyopacity += opacitySpeed * TimeDelta : Moneyopacity -= opacitySpeed * TimeDelta;

	opacity = min(1.f, max(opacity, 0.f));
	//Textopacity = min(1.f, max(opacity, 0.f));
	//Moneyopacity = min(1.f, max(opacity, 0.f));
}

void ItemDialog::Render(ID2D1RenderTarget* renderTarget)
{
	if (!isInitText)
	{
		InitTextRenderer(renderTarget, L"Consolas", 28.f, D2D1::ColorF::White);
		isInitText = true;
	}

	FPOINT cameraPos = Pos + CameraManager::GetInstance()->GetPos() + FPOINT{0.f, -100.f};
	DialogImage->Render(renderTarget, cameraPos.x, cameraPos.y,1.f,1.f, opacity);
	MoneyImage->Render(renderTarget, cameraPos.x + MoneyOffset.x, cameraPos.y + MoneyOffset.y, MoneyScale.x, MoneyScale.y, opacity);
	std::wstring priceText = std::to_wstring(price);
	RenderText(renderTarget, priceText, cameraPos.x + TextOffset.x, cameraPos.y + TextOffset.y, opacity);
}

HRESULT ItemDialog::InitTextRenderer(ID2D1RenderTarget* renderTarget, const wchar_t* fontFamily, float fontSize, D2D1::ColorF color)
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
		L"Tekton-Bold",  // 또는 "Tekton Pro", "Arial", 원하는 폰트 이름
		NULL,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fontSize,  // 폰트 크기
		L"en-us",
		&textFormat
	);
	if (FAILED(hr)) return hr;

	hr = renderTarget->CreateSolidColorBrush(
		color,
		&textColorBrush
	);

	return hr;
}

void ItemDialog::RenderText(ID2D1RenderTarget* renderTarget, const std::wstring& text, float x, float y, float opacity)
{
	if (renderTarget && textFormat && textColorBrush)
	{
		textColorBrush->SetOpacity(opacity);
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
