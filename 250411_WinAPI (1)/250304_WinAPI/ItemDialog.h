#pragma once
#include "GameObject.h"
class Image;

class ItemDialog : public GameObject
{
public:
	ItemDialog();
	virtual ~ItemDialog();

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	HRESULT InitTextRenderer(ID2D1RenderTarget* renderTarget, const wchar_t* fontFamily, float fontSize, D2D1::ColorF color);
	void RenderText(ID2D1RenderTarget* renderTarget, const std::wstring& text, float x, float y, float opacity);

	inline void SetPrice(int price) { this->price = price; };
	inline void SetCol(bool isCol) { this->isCol = isCol; };

private:
	Image* DialogImage;
	Image* MoneyImage;

	FPOINT MoneyOffset;
	FPOINT TextOffset;
	FPOINT MoneyScale;

	float opacity = 0.f;
	float Textopacity = 0.f;
	float Moneyopacity = 0.f;

	float Maxopacity = 1.f;
	float MaxTextopacity = 1.f;
	float MaxMoneyopacity = 1.f;

	float opacitySpeed = 5.f;

	bool isCol = false;
	bool isInitText = false;

	Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> textColorBrush;

	string itemName;
	int price = 1000;
};
