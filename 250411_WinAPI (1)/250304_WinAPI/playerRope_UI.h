#pragma once
#include "UI.h"

class PlayerStatus;
class Image;
class playerRope_UI : public UI
{
private :
	Image* playerRopeImage;
	PlayerStatus* playerStat;
	int playerRope_value;
public:
	virtual HRESULT Init();
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

};

