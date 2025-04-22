#pragma once
#include "Scene.h"

class Image;
class Npc_InteractUI : public Scene
{
private:
	Image* npcDialogBoxImage;
	Image* npcDialogArrowImage;
	Image* npcShopBoxImage;
	Image* goodsCurrencyImage;
	Image* interactButtonImage;
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
};

