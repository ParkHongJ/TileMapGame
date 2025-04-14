#pragma once 
#include "Scene.h"

class Image;
class JinScene : public Scene
{
private:
	Image* tempImage;
	POINT pos;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	JinScene() {};
	virtual ~JinScene() {};
};

