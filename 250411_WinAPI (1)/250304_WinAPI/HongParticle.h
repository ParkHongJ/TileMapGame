#pragma once
#include "GameObject.h"
class HongParticle : public GameObject
{
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta);		// 프레임 단위로 게임 로직 실행(데이터 계산)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)

public:
	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // 가속도

	float mass = 1.0f;
	FPOINT gravity = { 0, 98.0f };  // 중력
	bool useGravity = false;
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.4f;

	float RandomRange(float min, float max)
	{
		float r = (float)rand() / RAND_MAX; // 0.0 ~ 1.0
		return min + (max - min) * r;
	}

	void ClampVector(FPOINT& vec, float maxLength)
	{
		float len = sqrtf(vec.x * vec.x + vec.y * vec.y);
		if (len > maxLength)
		{
			float scale = maxLength / len;
			vec.x *= scale;
			vec.y *= scale;
		}
	}

	FPOINT RotateVector(const FPOINT& vec, float degrees)
	{
		float rad = degrees * 3.1415926f / 180.0f;
		float cosA = cosf(rad);
		float sinA = sinf(rad);

		return {
			vec.x * cosA - vec.y * sinA,
			vec.x * sinA + vec.y * cosA
		};
	}

	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

	float Dot(const FPOINT& a, const FPOINT& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	FPOINT Reflect(const FPOINT& velocity, const FPOINT& normal)
	{
		float dot = Dot(velocity, normal);
		return {
			velocity.x - 2.0f * normal.x * dot,
			velocity.y - 2.0f * normal.y * dot
		};
	}

};

