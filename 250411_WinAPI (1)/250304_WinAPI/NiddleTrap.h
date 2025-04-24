#pragma once
#include "GameObject.h"
class NiddleTrap : public GameObject
{
	GENERATE_BODY(Textures/floor_cave.png, 128, 128)
public:
	virtual HRESULT Init();		// ��� ������ �ʱ�ȭ, �޸� �Ҵ�
	virtual void Release();		// �޸� ����
	virtual void Update(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void LateUpdate(float TimeDelta);		// ������ ������ ���� ���� ����(������ ���)
	virtual void Render(ID2D1RenderTarget* renderTarget);	// ������ ������ ���(�̹���, �ؽ�Ʈ ��)
	virtual void CheckCulling();		// ������ ������ ���� ���� ����(������ ���)
	virtual void Detect(GameObject* obj);


private:
	class BoxCollider* collider;
	float objectScale;

	class Image* NiddleImage;

	bool bHit = false;

	GameObject* cachedTrappedObject;

	float downTime = 1.f;
};

