#pragma once
#include "GameObject.h"
class HeavyBlock : public GameObject
{
	GENERATE_BODY(Textures/floormisc.png, 128, 128)
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void LateUpdate(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)

private:
	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

private:
	float objectScale;
	class BoxCollider* collider;
	class Image* heavyBlockImage;

	//중력관련
	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // 가속도
	float mass = 1.0f;
	FPOINT gravity = { 0, 9.8f };  // 중력
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.15f;

	bool useGravity;

};

