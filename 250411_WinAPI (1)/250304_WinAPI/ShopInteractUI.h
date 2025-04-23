#pragma once
#include "UI.h"

class Image;
class ShopInteractUI : public UI
{
private:
	Image* goldImage;
	Image* interactKeyImage;
	Image* shopDialogBoxImage;
	FPOINT goldPos;
	int price;
	bool collisionConatact;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	ShopInteractUI() {};
	ShopInteractUI(ID2D1RenderTarget* renderTarget);
	~ShopInteractUI() {};
};