#pragma once
#include "Scene.h"

class JinScene;
class Image;
class playerUI : public Scene
{
private:
	JinScene* tempChar;
	Image* playerLifeImage;
	Image* playerLifeRunsOutImage;
	Image* bombImage;
	Image* ropeImage;
	Image* currencyImage;
	Image* sandGlassImage;
	Image* currencyBackgroundImage;
	Image* lightBulbImage;			//�������� �� �Ķ�����

	FPOINT lifeImagePos;
	FPOINT bombImagePos;
	FPOINT ropeImagePos;
	FPOINT currencyImagePos;
	FPOINT sandGlassImagePos;
	FPOINT currencyBackgroundImagePos;
	FPOINT lightBulbImagePos;

	string currentStage;
	int lifeCount;
	int bombCount;
	int ropeCount;
	int currencyCount;

	bool isAlive;

public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget);
	virtual void Release();		
	virtual void Update(float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);

	inline void SetLifeCount(int setValue) { lifeCount = setValue; if (lifeCount <= 0) lifeCount = 0; };
	inline void SetBombCount(int setValue) { bombCount = setValue; };
	inline void SetRopeCount(int setValue) { ropeCount = setValue; };
	inline void SetCurrency(int setValue) { currencyCount = setValue; };
	inline void SetDrawPlayerUI(bool setValue) { isAlive = setValue; };

	playerUI() {};
	virtual ~playerUI() {};
};

