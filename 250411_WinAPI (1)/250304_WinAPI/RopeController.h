#pragma once
#include "GameObject.h"
#include "config.h"
class Rope;

class RopeController : public GameObject
{
public:
	virtual HRESULT Init() override;
	virtual void Update(float TimeDelta) override;
	virtual void Render(ID2D1RenderTarget* renderTarget) override;
	virtual void Release() override;
	virtual void Detect(GameObject* obj) override;

private:
	int CurIndex;
	vector<Rope*> Ropes;
};

