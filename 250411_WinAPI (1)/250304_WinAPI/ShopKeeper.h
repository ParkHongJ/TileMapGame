#pragma once
#include "GameObject.h"

class Image;
class ShopKeeper : public GameObject
{
private:
	Image* shopkeeprImage;
	FPOINT playerPos;
	bool bPlayerPos_IsLeftSide;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
};

