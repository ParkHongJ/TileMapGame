#pragma once
#include "GameObject.h"

class TestMonster : public GameObject
{
public: 
	TestMonster();
	virtual ~TestMonster();

public:
	virtual HRESULT Init();					
	virtual void Release();					
	virtual void Update(float TimeDelta);	
	virtual void LateUpdate(float TimeDelta);
	virtual void Render(ID2D1RenderTarget* renderTarget);

public:
	virtual void Detect(GameObject* obj);

};

