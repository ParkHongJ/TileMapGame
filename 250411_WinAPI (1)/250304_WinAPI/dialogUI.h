#pragma once
#include "UI.h"

class Image;
class dialogUI : public UI
{
private:
	Image* imageBox;
	Image* imageArrow;
	FPOINT arrowPos;
	float playerPosX;
	float playerToCharDist;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	float DistCaculate();
	inline float SetPlayerXpos(float SetValue) { playerPosX = SetValue; };
};

