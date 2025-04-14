#pragma once
#include "GameObject.h"
#include "config.h"

class Image;
class HyoCharacter : public GameObject
{
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	HyoCharacter() {};
	virtual ~HyoCharacter() {};

private:

	int dir;
	Image* playerIris;
	PlayerState state;
};

