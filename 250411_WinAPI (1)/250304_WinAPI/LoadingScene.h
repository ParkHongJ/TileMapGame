#pragma once
#include "GameObject.h"

class Image;
class LoadingScene : public GameObject
{
private:
	Image* bg;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	virtual ~LoadingScene() {};

};

