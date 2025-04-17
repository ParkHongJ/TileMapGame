#pragma once 
#include "Scene.h"

class Image;
class JinScene : public Scene
{
private:
	Image* tempImage;
	POINT pos;
	int life;
	int bomb;
	int rope;
	int currency;
	bool isAlive;
public:
	virtual HRESULT Init(ID2D1HwndRenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	inline int GetPlayerLife() const { return life; };
	inline int GetPlayerBomb() const { return bomb; };
	inline int GetPlayerRope() const { return rope; };
	inline int GetPlayerCurrency() const { return currency; };
	inline bool isPlayerAlive() const { return isAlive; };

	JinScene() {};
	virtual ~JinScene() {};
};

