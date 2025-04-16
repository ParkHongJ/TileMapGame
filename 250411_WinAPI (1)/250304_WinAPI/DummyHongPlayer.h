#pragma once
#include "GameObject.h"
class DummyHongPlayer : public GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
private:
	float moveSpeed = 200.f;
	bool isFalling = false;
	FPOINT velocity;

	float JUMP_SPEED = -600.0f;
	float GRAVITY = 2000.0f;
	float MAX_FALL_SPEED = 1000.0f;

	bool isKnockback = false;
	FPOINT knockbackVelocity = { 80.f, 10.f };
	float knockbackTime = 0.7f;
};

