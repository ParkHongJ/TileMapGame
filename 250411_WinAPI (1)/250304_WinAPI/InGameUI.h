#pragma once
#include "Scene.h"

class Image;
class InGameUI : public Scene
{
private:
	Image* playerLifeImage;
	Image* playerLifeRunsOutImage;
	Image* bombImage;
	Image* ropeImage;
	Image* currencyImage;
	Image* sandGlassImage;
	Image* lightBulbImage;			//스테이지 옆 파란전구
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

	inline void SetLifeCount(int setValue) { lifeCount = setValue; if (lifeCount <= 0) lifeCount = 0; };
	inline void SetBombCount(int setValue) { bombCount = setValue; };
	inline void SetRopeCount(int setValue) { ropeCount = setValue; };
	inline void SetCurrency(int setValue) { currencyCount = setValue; };

	InGameUI() {};
	virtual ~InGameUI() {};
};

