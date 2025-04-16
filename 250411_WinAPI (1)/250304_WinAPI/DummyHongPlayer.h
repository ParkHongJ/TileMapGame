#pragma once
#include "GameObject.h"
class DummyHongPlayer : public GameObject
{
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
private:
	float moveSpeed = 200.f;
	bool isFalling = false;
	FPOINT velocity;

	float JUMP_SPEED = -600.0f;
	float GRAVITY = 2000.0f;
	float MAX_FALL_SPEED = 1000.0f;

	bool isKnockback = false;
	FPOINT knockbackVelocity = { 80.f, 10.f };
	float knockbackTime = 0.7f;
};

