#pragma once
#include "GameObject.h"
#include <d2d1.h>
#include <dwrite.h>
#include <wrl/client.h>

enum class UI_Status
{
	PLAYER_UI,
	GAMEOVER_UI,
};

class Image;
class UI : public GameObject
{
private:

protected:
	FPOINT imagePos;
	FPOINT imageRatio;
	float x_pos_divide_factor;
	bool isInteract;
	float defaultOpacity;
	float opacityDuration;
	float opacityTimer;
	bool isOpaque;
	bool requestOpaqueChange;
	Microsoft::WRL::ComPtr<IDWriteFactory> writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> textColorBrush;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	void SetOpaque();
	void DisableOpaque();
	inline void RequestOpaqueChange() { requestOpaqueChange = true; };

	FPOINT ResolutionRatio();

	HRESULT InitTextRenderer(ID2D1RenderTarget* renderTarget, const wchar_t* fontFamily, float fontSize, D2D1::ColorF color);
	void RenderText(ID2D1RenderTarget* renderTarget, const std::wstring& text, float x, float y);
	void RenderText(ID2D1RenderTarget* renderTarget, const std::wstring& text, D2D1_RECT_F layoutRect);

	UI();
	virtual ~UI() {};
};

