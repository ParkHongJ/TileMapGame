#pragma once
#include "GameObject.h"
class Arrow : public GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
	virtual void CheckCulling();		// 프레임 단위로 게임 로직 실행(데이터 계산)

public:
	void SetDirection(bool isFlip) { this->isFlip = isFlip; }
	void Drop(float timeDelta);

	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

	virtual void Detect(GameObject* obj) override;
private:
	class Image* arrowImage;

	class BoxCollider* collider;

	bool isFlip = false;
	float objectScale;

	float moveSpeed = 2500.f;
	//float moveSpeed = 250.f;

	bool bHit = false;
	float hitDelay = 1.0f;
	bool bDrop = false;
	float angle = 0.f;

	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // 가속도
	float mass = 1.0f;
	FPOINT gravity = { 0, 100.f };  // 중력
	FPOINT totalForce = { 0, 0 };
	bool useGravity = true;
};

