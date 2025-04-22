#pragma once
#include "GameObject.h"
class ArrowTrap : public GameObject
{
	GENERATE_BODY(Textures/floormisc.png, 128, 128)
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1HwndRenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	virtual void CheckCulling();		// ������ ������ ���� ���� ����(������ ���)

private:
	void Fire();

	class Image* trapImage;
	class BoxCollider* collider;

	float objectScale;


	bool isFire = false;
};

