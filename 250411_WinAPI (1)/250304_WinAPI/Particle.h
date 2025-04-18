#pragma once
#include "CommonFunction.h"
class Particle
{
public:
	void Init(string imageStr, FPOINT pos, float angle, float size, float lifeTime, int atlasX, int atlasY);
	virtual void Update(float TimeDelta) ;
	virtual void Render(ID2D1HwndRenderTarget* rt);
	virtual void Release();

	bool IsEnd() const { return isEnd; }
	void AddParticleOption(class IParticleOption* particleOp);
public:
	//���̽�
	FPOINT pos;
	float size;
	float angle;
	float lifeTime;
	float elapsedTime;
	bool isEnd;

	POINT atlas;

	class Image* image;
	vector<class IParticleOption*> options;
};

class IParticleOption {
public:

	virtual bool HandlesRender() const { return false; } // �⺻�� false
	virtual void Update(Particle& particle, float dt) = 0;
	virtual void Render(Particle& particle, ID2D1HwndRenderTarget* rt) {}
	virtual ~IParticleOption() = default;
};

class PhysicsOption : public IParticleOption
{
	//�߷°���
	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // ���ӵ�
	float mass = 1.0f;
	FPOINT gravity = { 0, 1500.0f };  // �߷�
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.55f;

	bool useGravity;

public:

	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

	void Init(FPOINT velocity, float bounciness);
	virtual void Update(Particle& particle, float TimeDelta) override;
	virtual void Render(Particle& particle, ID2D1HwndRenderTarget* rt) override;
};

class HomingLinearOption : public IParticleOption
{
	FPOINT center;         // ��ǥ ����
	float speed;           // �̵� �ӵ�
	FPOINT direction;      // ���� ���� ����
	float angle = 0.0f;    // ȸ�� ���� (����)

	bool initialized = false;

public:
	HomingLinearOption(FPOINT target, float moveSpeed)
		: center(target), speed(moveSpeed) {
	}

	bool HandlesRender() const override { return true; }
	void Update(Particle& p, float dt) override
	{
		if (!initialized)
		{
			FPOINT toTarget = { center.x - p.pos.x, center.y - p.pos.y };
			float len = sqrtf(toTarget.x * toTarget.x + toTarget.y * toTarget.y);
			if (len != 0)
			{
				direction = { toTarget.x / len, toTarget.y / len };
				angle = atan2f(direction.y, direction.x);
			}
			initialized = true;
		}

		// �̵�
		p.pos.x += direction.x * speed * dt;
		p.pos.y += direction.y * speed * dt;

		// �߽� ���� ���� Ȯ�� (�ʹ� ��������� ����)
		FPOINT delta = { center.x - p.pos.x, center.y - p.pos.y };
		if (delta.x * delta.x + delta.y * delta.y < 4.f) // �Ÿ�^2 < 2^2
			p.isEnd = true;
	}

	void Render(Particle& p, ID2D1HwndRenderTarget* rt) override;
};