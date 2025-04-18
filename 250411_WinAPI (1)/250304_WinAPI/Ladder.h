#pragma once
#include "GameObject.h"
class Image;
class Ladder : public GameObject
{
public:
	Ladder();
	virtual ~Ladder();

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Detect(GameObject* obj) override;

	void InstallLadder(FPOINT pos, int cnt);
private:
	Image* image;
	int CurFrameX;
	int CurFrameY;
	int MaxFrameX;

};

