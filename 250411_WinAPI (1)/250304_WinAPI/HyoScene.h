#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "HyoCharacter.h"

class Image;
class Camera;
class HyoCharacter;
class HyoScene : public Scene
{
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	HyoScene() {};
	virtual ~HyoScene() {};

private:
	Image* background;
	HyoCharacter* testBackGround;
	HyoCharacter* testCamera;
	Camera* camera;

	float mapSizeHeight;
	float maptSizeWidth;

};

