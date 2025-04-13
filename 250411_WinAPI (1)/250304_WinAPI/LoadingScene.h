#pragma once
#include "Scene.h"

class Image;
class LoadingScene : public Scene
{
private:
	Image* bg;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	virtual ~LoadingScene() {};

};

