#pragma once
#include "Scene.h"

class Image;
class UI : public Scene
{
private:
	Image* lifeImage;
	Image* bombImage;
	Image* ropeImage;
	Image* currencyImage;
	Image* sandGlassImage;
	Image* lightBulb;			//�������� �� �Ķ�����
	int lifeCount;
	int bombCount;
	int ropeCount;
	int currencyCount;
	FPOINT lifeImagePos;
	FPOINT bombImagePos;
	FPOINT ropeImagePos;
	FPOINT currencyImagePos;
	FPOINT sandGlassImagePos;
	FPOINT lightBulbImagePos;
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget);
	virtual void Release();		
	virtual void Update(float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);

	UI() {};
	virtual ~UI() {};
};

