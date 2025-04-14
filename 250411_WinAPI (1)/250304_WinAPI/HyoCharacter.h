#pragma once
#include "GameObject.h"
#include "config.h"

class Image;
class HyoCharacter : public GameObject
{
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	HyoCharacter() {};
	virtual ~HyoCharacter() {};

private:

	FPOINT pos; 
	int dir;
	Image* playerIris;

	PlayerState state;
};

