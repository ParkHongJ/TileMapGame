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
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
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



	FPOINT elbow = { 100.f, 0 };
	FPOINT hand = { 200.f, 0 };

	FPOINT worldElbow; // ���� ���� ��ǥ
	FPOINT worldHand;
	float len1;       // �����ǥ ����
	float len2;
	FPOINT targetPos; // ��ǥ ����

	class Image* img;
	bool bExplosion = false;
	float CurrentFrame = 0.f;
	int CurrentIndex = 0;

	void SolveIK2Bone(FPOINT root, FPOINT& elbow, FPOINT& hand, const FPOINT& target, float len1, float len2);

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

	class BoxCollider* collider;
};



//float len1 = 50; // upper arm
//float len2 = 50; // forearm
//
//SolveIK2Bone(root, elbow, hand, target, len1, len2);