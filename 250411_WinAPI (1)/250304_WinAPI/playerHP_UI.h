#pragma once
#include "UI.h"

class PlayerStatus;
class Image;
class playerHP_UI : public UI
{
private:
	PlayerStatus* playerStat;
	Image* playerLifeImage;
	Image* playerLifeRunsOutImage;
	int playerHP_value;
	bool isAlive;
	/*float defaultOpacity;	//투명화 관련 기능 전부 U 클래스로 옮김
	float opacityDuraion;
	float opacityTimer;
	bool isOpaque;*/

public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;

	/*void SetOpaque();
	void DisableOpaque();*/

	playerHP_UI() {};
	virtual ~playerHP_UI() {};
};

