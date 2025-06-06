#pragma once
#include "UI.h"

class PlayerStatus;
class Image;
class playerBomb_UI : public UI
{
private:
	PlayerStatus* playerStat;
	Image* playerBombImage;
	int playerBomb_value;
	int pastBomb_value;

public:
	virtual HRESULT Init();
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	playerBomb_UI() {};
	playerBomb_UI(ID2D1RenderTarget* renderTarget);

};

