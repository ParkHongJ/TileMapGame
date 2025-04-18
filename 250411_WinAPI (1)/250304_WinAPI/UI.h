#pragma once
#include "GameObject.h"

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

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	FPOINT ResolutionRatio(Image* image);

	UI() { x_pos_divide_factor = 25.0f;};
	virtual ~UI() {};
};

