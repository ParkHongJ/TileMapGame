#pragma once
#include "UI.h"

class Image;
class PlayerStatus;

class sandGlass_UI : public UI
{
private:
	Image* image;
	PlayerStatus* playerStat;
	float playTimeSec;
	int playTimeMin;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	inline float GetPlayTimeSec() const { return playTimeSec; }
	inline int GetPlayTimeMin() const { return playTimeMin; }

	sandGlass_UI() {};
	sandGlass_UI(ID2D1RenderTarget* renderTarget);
	virtual ~sandGlass_UI() {};
};

