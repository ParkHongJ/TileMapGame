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
	
	FPOINT pos;			// ī�޶��� ��ġ
	FPOINT target;		// �÷��̾��� ��ġ
	float moveSpeed;	// ī�޶� �ӵ� 
};

