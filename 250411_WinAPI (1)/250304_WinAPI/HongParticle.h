#pragma once
#include "GameObject.h"
class HongParticle : public GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

public:
	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // ���ӵ�

	float mass = 1.0f;
	FPOINT gravity = { 0, 1500.0f };  // �߷�
	bool useGravity = false;
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.55f;

	class Image* blood = nullptr;
	float lifeTime = 1.5f;
	FPOINT scale;


	void AddForce(const FPOINT& force)
	{
		totalForce.x += force.x;
		totalForce.y += force.y;
	}

};

