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
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	// virtual void TestRender(ID2D1HwndRenderTarget* renderTarget, const FPOINT& cameraPos) ;

	float GetBackGroundWidth() { return mapSizeWidth; } 
	float GetBackGroundHeight() { return mapSizeHeight; }
	void SetIsLookUp(bool isLookUp) { this->isLookUp = isLookUp; }
	void SetIsLookDown(bool isLookDown) { this->isLookDown = isLookDown; }

	HyoCharacter() {};
	virtual ~HyoCharacter() {};

private:

	FPOINT backGroundPos;
	FPOINT playerPos;
	
	float mapSizeHeight;
	float mapSizeWidth;

	float lookUpTime; 
	float lookDownTime; 

	bool isLookUp;
	bool isLookDown;

	int dir;
	// Image* playerIris;
	Image* testBackGround;
	Image* testCamera;
};

