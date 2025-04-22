#pragma once
#include "UI.h"

class Image;
class DoorInteractUI : public UI
{
private:
	Image* interactKey;
	Image* doorSprite;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
};

