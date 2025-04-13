#pragma once
#include "GameObject.h"
#include "config.h"

class Character : public GameObject
{
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(HDC hdc) override;

	Character() {};
	virtual ~Character() {};
};

