#pragma once
#include "GameObject.h"
class DummyHongPlayer : public GameObject
{
	GENERATE_BODY(Textures/char_lime.png, 128, 128)
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta) override;		// ������ ������ ���� ���� ����(������ ���)

	virtual void LateUpdate(float TimeDelta) override;
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
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
	FPOINT acceleration = { 0, 0 };  // ���ӵ�

	float mass = 1.0f;
	FPOINT gravity = { 0, 9.8f };  // �߷�
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

