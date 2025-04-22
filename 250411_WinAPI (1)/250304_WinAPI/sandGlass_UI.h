#pragma once
#include "UI.h"

class Image;
class PlayerStatus;

class sandGlass_UI : public UI
{
private:
	Image* image;
	PlayerStatus* playerStat;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

};

