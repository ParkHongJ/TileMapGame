#pragma once
#include "GameObject.h"
class HeavyBlock : public GameObject
{
	GENERATE_BODY(Textures/floormisc.png, 128, 128)
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)

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

	//�߷°���
	FPOINT velocity;
	FPOINT acceleration = { 0, 0 };  // ���ӵ�
	float mass = 1.0f;
	FPOINT gravity = { 0, 9.8f };  // �߷�
	bool bPhysics = false;
	FPOINT totalForce = { 0, 0 };
	float bounciness = 0.15f;

	bool useGravity;

};

