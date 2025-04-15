#pragma once
#include "GameObject.h"

class Camera : public GameObject
{
public: 
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update(const FPOINT& playerPos, float TimeDelta);
	virtual void Render(ID2D1HwndRenderTarget* renderTarget) override;

	const FPOINT GetPos() { return pos; }

private: 
	
	FPOINT pos;			// 카메라의 위치
	FPOINT target;		// 플레이어의 위치
	float moveSpeed;	// 카메라 속도 
};

