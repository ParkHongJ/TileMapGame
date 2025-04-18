#pragma once
#include "GameObject.h"
class DummyHongPlayer : public GameObject
{
	GENERATE_BODY(Textures/char_lime.png, 128, 128)
public:
	virtual HRESULT Init();		// 멤버 변수의 초기화, 메모리 할당
	virtual void Release();		// 메모리 해제
	virtual void Update(float TimeDelta) override;		// 프레임 단위로 게임 로직 실행(데이터 계산)

	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// 프레임 단위로 출력(이미지, 텍스트 등)
private:
	float moveSpeed = 200.f;
	bool isFalling = false;

	float JUMP_SPEED = -600.0f;
	float GRAVITY = 2000.0f;
	float MAX_FALL_SPEED = 1000.0f;

	bool isKnockback = false;
	FPOINT knockbackVelocity = { 80.f, 10.f };
	float knockbackTime = 0.7f;



	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // 가속도

	float mass = 1.0f;
	FPOINT gravity = { 0, 9.8f };  // 중력
	bool useGravity = false;
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.5f;

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

