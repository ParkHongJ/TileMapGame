#pragma once
#include "GameObject.h"
#include <vector>

class Missile;
class MissileManager : public GameObject
{
private:
	vector<Missile*> vecMissiles;
	vector<Missile*>::iterator iterMissiles;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;
};

