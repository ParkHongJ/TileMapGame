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
	virtual void TestRender(ID2D1HwndRenderTarget* renderTarget, const FPOINT& cameraPos) ;

	HyoCharacter() {};
	virtual ~HyoCharacter() {};

private:

	FPOINT backGroundPos;
	FPOINT playerPos;
	
	int dir;
	// Image* playerIris;
	Image* testBackGround;
	Image* testCamera;
	PlayerState state;
};

