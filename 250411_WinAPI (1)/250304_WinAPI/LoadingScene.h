#pragma once
#include "Scene.h"

class Image;
class LoadingScene : public Scene
{
private:
	Image* bg;

	ComPtr<ID2D1BitmapRenderTarget> loadingRT;
public:
	virtual HRESULT Init(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;

	virtual ~LoadingScene() {};

};

