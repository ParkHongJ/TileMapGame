#pragma once
#include "GameObject.h"
class Arrow : public GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	virtual void CheckCulling();		// ������ ������ ���� ���� ����(������ ���)

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
	FPOINT acceleration = { 0, 0 };  // ���ӵ�
	float mass = 1.0f;
	FPOINT gravity = { 0, 100.f };  // �߷�
	FPOINT totalForce = { 0, 0 };
	bool useGravity = true;
};

