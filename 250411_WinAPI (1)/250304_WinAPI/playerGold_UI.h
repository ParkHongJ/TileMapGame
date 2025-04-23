#pragma once
#include "UI.h"

class PlayerStatus;
class Image;
class playerGold_UI : public UI
{
private:
	Image* playerGoldImage;
	PlayerStatus* playerStat;
	unsigned int playerGold_value;
	unsigned int pastGold_value;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	playerGold_UI() {};
	playerGold_UI(ID2D1RenderTarget* renderTarget);
	~playerGold_UI() {};
};

