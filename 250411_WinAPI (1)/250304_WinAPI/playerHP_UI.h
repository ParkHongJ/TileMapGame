#pragma once
#include "UI.h"

class PlayerStatus;
class Image;
class playerHP_UI : public UI
{
private:
	PlayerStatus* playerStat;
	Image* playerLifeImage;
	Image* playerLifeRunsOutImage;
	int playerHP_value;
	bool isAlive;

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	
	playerHP_UI() {};
	playerHP_UI(ID2D1HwndRenderTarget* renderTarget);
	virtual ~playerHP_UI() {};
};

